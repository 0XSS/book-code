#include "stdafx.h"

void AsyncUnload(IN PDRIVER_OBJECT DriverObject);
NTSTATUS AsyncCreateClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS AsyncDefaultHandler(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS AsyncRead(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
VOID
CustomDpc(
    __in struct _KDPC  *Dpc,
    __in_opt PVOID  DeferredContext,
    __in_opt PVOID  SystemArgument1,
    __in_opt PVOID  SystemArgument2
);

typedef struct _Device_Extension_ {
    LIST_ENTRY IrpList;

    KTIMER timer;
    LARGE_INTEGER liDueTime;
    KDPC dpc;

} DevExt;

#ifdef __cplusplus
    extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath);
#endif

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath)
{
    UNICODE_STRING DeviceName, Win32Device;
    PDEVICE_OBJECT DeviceObject = NULL;
    NTSTATUS status;
    unsigned i;
    DevExt *pDevExt = NULL;
    HANDLE hThread;
    OBJECT_ATTRIBUTES ObjectAttributes;
    CLIENT_ID  CID;
    RtlInitUnicodeString(&DeviceName, L"\\Device\\Async0");
    RtlInitUnicodeString(&Win32Device, L"\\DosDevices\\Async0");

    for (i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
        DriverObject->MajorFunction[i] = AsyncDefaultHandler;

    DriverObject->MajorFunction[IRP_MJ_CREATE] = AsyncCreateClose;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = AsyncCreateClose;
    DriverObject->MajorFunction[IRP_MJ_READ] = AsyncRead;
    DriverObject->DriverUnload = AsyncUnload;
    //ע�����һ���Զ�����չ����СΪsizeof(DevExt)
    status = IoCreateDevice(DriverObject,
                            sizeof(DevExt),
                            &DeviceName,
                            FILE_DEVICE_UNKNOWN,
                            0,
                            FALSE,
                            &DeviceObject);

    if (!NT_SUCCESS(status))
        return status;

    if (!DeviceObject)
        return STATUS_UNEXPECTED_IO_ERROR;

    DeviceObject->Flags |= DO_DIRECT_IO;
    DeviceObject->AlignmentRequirement = FILE_WORD_ALIGNMENT;
    status = IoCreateSymbolicLink(&Win32Device, &DeviceName);
    DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;
    pDevExt = (DevExt*)DeviceObject->DeviceExtension;
    //��ʼ��Irp����
    InitializeListHead(&pDevExt->IrpList);
    //��ʼ����ʱ��
    KeInitializeTimer(&pDevExt->timer);
    //��ʼ��DPC
    KeInitializeDpc(&pDevExt->dpc, (PKDEFERRED_ROUTINE)CustomDpc, pDevExt);
    //���ö�ʱʱ��Ϊ3s
    pDevExt->liDueTime = RtlConvertLongToLargeInteger(-30000000);
    //������ʱ��
    KeSetTimer(&pDevExt->timer, pDevExt->liDueTime, &pDevExt->dpc);
    return STATUS_SUCCESS;
}


void AsyncUnload(IN PDRIVER_OBJECT DriverObject)
{
    UNICODE_STRING Win32Device;
    DevExt *pDevExt = (DevExt *)DriverObject->DeviceObject->DeviceExtension;
    KeCancelTimer(&pDevExt->timer);
    RtlInitUnicodeString(&Win32Device, L"\\DosDevices\\Async0");
    IoDeleteSymbolicLink(&Win32Device);
    IoDeleteDevice(DriverObject->DeviceObject);
}

NTSTATUS AsyncCreateClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}

NTSTATUS AsyncDefaultHandler(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    Irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return Irp->IoStatus.Status;
}

NTSTATUS AsyncRead(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    NTSTATUS status;
    PIO_STACK_LOCATION pSP = IoGetCurrentIrpStackLocation(Irp);
    DevExt *pDevExt = (DevExt*)DeviceObject->DeviceExtension;
    //����IRPΪPending�������Ժ���ɸ�IRP
    IoMarkIrpPending(Irp);
    //��IRP���뵽�Զ���������
    InsertTailList(&pDevExt->IrpList, &Irp->Tail.Overlay.ListEntry);
    //����Pending��ע�ⷵ�ظ�I/O��������ֵ�����IRP��Pending��־λһ��
    //������IoMarkIrpPending�ͷ���ֵҪһ��
    return STATUS_PENDING;
}


VOID
CustomDpc(
    __in struct _KDPC  *Dpc,
    __in_opt PVOID  DeferredContext,
    __in_opt PVOID  SystemArgument1,
    __in_opt PVOID  SystemArgument2
)
{
    PIRP pIrp;
    DevExt *pDevExt = (DevExt*)DeferredContext;
    PVOID pBuf = NULL;
    ULONG uBuf = 0;
    PIO_STACK_LOCATION pSP = NULL;
    KdPrint(("[CustomDpc]\n"));

    do {
        if (!pDevExt) {
            KdPrint(("!pDevExt\n"));
            break ;
        }

        //���δ��IRP�����Ƿ�Ϊ��
        if (IsListEmpty(&pDevExt->IrpList)) {
            KdPrint(("IsListEmpty\n"));
            break ;
        }

        //
        //��IRP������ȡ��һ��IRP����ɸ�IRP
        //
        KdPrint(("[CustomDpc] Dequeue one irp from IrpList and complete it!\n"));
        pIrp = (PIRP)RemoveHeadList(&pDevExt->IrpList);

        if (!pIrp) {
            break ;
        }

        pIrp = (PIRP)CONTAINING_RECORD(pIrp, IRP, Tail.Overlay.ListEntry);
        pSP = IoGetCurrentIrpStackLocation(pIrp);
        KdPrint(("CustomDpc irp:0x%08x\n", pIrp));
        //��������Ķ�дI/O��ʽΪֱ��I/O
        pBuf = MmGetSystemAddressForMdl(pIrp->MdlAddress);

        if (NULL == pBuf) {
            pIrp->IoStatus.Status = STATUS_UNSUCCESSFUL;
            pIrp->IoStatus.Information = 0;
            IoCompleteRequest(pIrp, IO_NO_INCREMENT);
            break;
        }

        uBuf = pSP->Parameters.Read.Length;
        KdPrint(("CustomDpc Read uBuf:%d\n", uBuf));
        //֧��5�ֽ����µĶ�����
        uBuf = uBuf > 5 ? 5 : uBuf;
        //������������
        RtlCopyMemory(pBuf, "hello", uBuf);
        pIrp->IoStatus.Status = STATUS_SUCCESS;
        pIrp->IoStatus.Information = uBuf;
        //��ɸ�IRP
        IoCompleteRequest(pIrp, IO_NO_INCREMENT);
        KdPrint(("[CustomDpc] finish complete!\n"));
    } while (FALSE);

    KdPrint(("Set Next Timer.\n"));
    //�������ö�ʱ��
    KeSetTimer(&pDevExt->timer, pDevExt->liDueTime, &pDevExt->dpc);
}