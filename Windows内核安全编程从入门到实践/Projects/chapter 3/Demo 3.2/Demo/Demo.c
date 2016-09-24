//
//ͷ�ļ�
//
#include "ntddk.h"

//
//IO ControlCode
//
#define IOCTL_BUFFERED_IO  \
    CTL_CODE(FILE_DEVICE_UNKNOWN,0x800,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_INDIRECT_IO  \
    CTL_CODE(FILE_DEVICE_UNKNOWN,0x801,METHOD_IN_DIRECT,FILE_ANY_ACCESS)
#define IOCTL_NEITHER_IO   \
    CTL_CODE(FILE_DEVICE_UNKNOWN,0x802,METHOD_NEITHER ,FILE_ANY_ACCESS)
#define IOCTL_SHARE_MEMORY   \
    CTL_CODE(FILE_DEVICE_UNKNOWN,0x803,METHOD_BUFFERED,FILE_ANY_ACCESS)
//
//��������
//
void DemoUnload(IN PDRIVER_OBJECT DriverObject);
NTSTATUS DemoCreateClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS DemoReadWrite(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS DemoDevControl(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS DemoDefaultHandler(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);

//
//������ں���
//
NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath)
{
    UNICODE_STRING DeviceName, Win32Device;
    PDEVICE_OBJECT DeviceObject = NULL;
    NTSTATUS status;
    unsigned i;
    KdPrint(("[DriverEntry]\n"));
    RtlInitUnicodeString(&DeviceName, L"\\Device\\Demo0");
    RtlInitUnicodeString(&Win32Device, L"\\DosDevices\\Demo0");

    for (i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
        DriverObject->MajorFunction[i] = DemoDefaultHandler;

    //IRP������
    DriverObject->MajorFunction[IRP_MJ_CREATE] = DemoCreateClose;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = DemoCreateClose;
    DriverObject->MajorFunction[IRP_MJ_READ] = DemoReadWrite;
    DriverObject->MajorFunction[IRP_MJ_WRITE] = DemoReadWrite;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DemoDevControl;
    DriverObject->DriverUnload = DemoUnload;
    //�����豸,ָ��10�ֽڴ�С���Զ����������ڴ��Ӧ�ó������豸�Ľ�����Ϣ
    status = IoCreateDevice(DriverObject,
                            10,
                            &DeviceName,
                            FILE_DEVICE_UNKNOWN,
                            0,
                            FALSE,
                            &DeviceObject);

    if (!NT_SUCCESS(status))
        return status;

    if (!DeviceObject)
        return STATUS_UNEXPECTED_IO_ERROR;

    //��ʼ����10���ֽ�����
    memset(DeviceObject->DeviceExtension, 'A', 10);
    //�����豸�Ķ�д��ʽ
    //DeviceObject->Flags |= DO_BUFFERED_IO;
    //DeviceObject->Flags |= DO_DIRECT_IO;
    status = IoCreateSymbolicLink(&Win32Device, &DeviceName);
    DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;
    return STATUS_SUCCESS;
}

//
//����ж�غ�����֧��������ж�ز���
//
void DemoUnload(IN PDRIVER_OBJECT DriverObject)
{
    UNICODE_STRING Win32Device;
    KdPrint(("[DemoUnload]\n"));
    RtlInitUnicodeString(&Win32Device, L"\\DosDevices\\Demo0");
    //ɾ����������
    IoDeleteSymbolicLink(&Win32Device);
    //ɾ���������豸
    IoDeleteDevice(DriverObject->DeviceObject);
}

//
//�򿪡��ر���������
//
NTSTATUS DemoCreateClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    //���IRP
    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}

//
//Ĭ�ϵ�IRP��������
//
NTSTATUS DemoDefaultHandler(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    Irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return Irp->IoStatus.Status;
}

//
//��д����
//  д����Ĵ���ֻ�Ǽ򵥵����豸�Զ��������п������ݣ�
//  ������Ĵ���ֻ�Ǽ򵥵ش��豸�Զ��������ж�ȡ���ݡ�
//  ע�⣬���֧��10�ֽ�
//
NTSTATUS DemoReadWrite(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    NTSTATUS status = STATUS_SUCCESS;
    PIO_STACK_LOCATION pSP = IoGetCurrentIrpStackLocation(Irp);
    PVOID pBuffer = NULL;
    BOOLEAN bNeither = FALSE;
    ULONG uLen = 0;
    KdPrint(("[DemoReadWrite]\n"));

    if (DeviceObject->Flags & DO_BUFFERED_IO) {
        KdPrint(("Flags: DO_BUFFERED_IO\n"));
        pBuffer = Irp->AssociatedIrp.SystemBuffer;
    } else if (DeviceObject->Flags & DO_DIRECT_IO) {
        KdPrint(("Flags: DO_DIRECT_IO\n"));
        pBuffer = MmGetSystemAddressForMdl(Irp->MdlAddress);
    } else {
        KdPrint(("Flags: Neither \n"));
        bNeither = TRUE;
        pBuffer = Irp->UserBuffer;
    }

    switch (pSP->MajorFunction) {
    case IRP_MJ_READ:
        uLen = pSP->Parameters.Read.Length ;
        uLen = uLen > 10 ? 10 : uLen;
        KdPrint(("IRP_MJ_READ Read Len: %d\n", pSP->Parameters.Read.Length));

        if (FALSE == bNeither) {
            RtlCopyMemory(pBuffer, DeviceObject->DeviceExtension, uLen);
        } else {
            //���߽Բ���ʽ��Ҫ��֤����������Ч��
            _try {
                ProbeForWrite(pBuffer, uLen, 4);
                RtlCopyMemory(pBuffer, DeviceObject->DeviceExtension, uLen);
            }
            _except(EXCEPTION_EXECUTE_HANDLER) {
                KdPrint(("IRP_MJ_READ exception!\n"));
                status = STATUS_UNSUCCESSFUL;
            }
        }

        break;

    case IRP_MJ_WRITE:
        uLen = pSP->Parameters.Write.Length;
        uLen = uLen > 10 ? 10 : uLen;
        KdPrint(("IRP_MJ_WRITE Write Len: %d\n", pSP->Parameters.Write.Length));

        if (FALSE == bNeither) {
            RtlCopyMemory(DeviceObject->DeviceExtension, pBuffer, uLen);
        } else {
            _try {
                ProbeForRead(pBuffer, uLen, 4);
                RtlCopyMemory(DeviceObject->DeviceExtension, pBuffer, uLen);
            }
            _except(EXCEPTION_EXECUTE_HANDLER) {
                KdPrint(("IRP_MJ_WRITE exception!\n"));
                status = STATUS_UNSUCCESSFUL;
            }
        }

        break;
    }

    Irp->IoStatus.Status = status;
    Irp->IoStatus.Information = uLen;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return status;
}

//
//�Զ����������
//  ͬ��Ҳ����Ӧ�ó�����豸����֮�俽�����ݣ�
//  �������Ϊ10�ֽڡ�
//
NTSTATUS DemoDevControl(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    NTSTATUS status = STATUS_SUCCESS;
    PIO_STACK_LOCATION pSP = IoGetCurrentIrpStackLocation(Irp);
    ULONG uControlCode = pSP->Parameters.DeviceIoControl.IoControlCode;
    PVOID pInBuf = NULL, pOutBuf = NULL;
    ULONG uInLen = 0, uOutLen = 0;
    PVOID pSysAddr = NULL;
    PMDL pMdl;
    KdPrint(("[DemoDevControl]\n"));
    uInLen = pSP->Parameters.DeviceIoControl.InputBufferLength;
    uInLen = uInLen > 10 ? 10 : uInLen;
    uOutLen = pSP->Parameters.DeviceIoControl.OutputBufferLength;
    uOutLen = uOutLen > 10 ? 10 : uOutLen;
    KdPrint(("uInLen: %d uOutLen: %d\n",
             pSP->Parameters.DeviceIoControl.InputBufferLength,
             pSP->Parameters.DeviceIoControl.OutputBufferLength));

    switch (uControlCode) {
    case IOCTL_BUFFERED_IO:
        KdPrint(("IOCTL_BUFFERED_IO\n"));
        pInBuf = pOutBuf = Irp->AssociatedIrp.SystemBuffer;

        if (uInLen) {
            RtlCopyMemory(DeviceObject->DeviceExtension, pInBuf, uInLen);
        }

        if (uOutLen) {
            RtlCopyMemory(pOutBuf, DeviceObject->DeviceExtension, uOutLen);
        }

        break;

    case IOCTL_INDIRECT_IO:
        KdPrint(("IOCTL_INDIRECT_IO\n"));
        pInBuf = Irp->AssociatedIrp.SystemBuffer;
        pOutBuf = MmGetSystemAddressForMdl(Irp->MdlAddress);

        if (uInLen) {
            RtlCopyMemory(DeviceObject->DeviceExtension, pInBuf, uInLen);
        }

        if (uOutLen) {
            RtlCopyMemory(pOutBuf, DeviceObject->DeviceExtension, uOutLen);
        }

        break;

    case IOCTL_NEITHER_IO:
        KdPrint(("IOCTL_NEITHER_IO\n"));
        pInBuf = pSP->Parameters.DeviceIoControl.Type3InputBuffer;
        pOutBuf = Irp->UserBuffer;
        _try {
            if (uInLen)
            {
                ProbeForRead(pInBuf, uInLen, 4);
                RtlCopyMemory(DeviceObject->DeviceExtension, pInBuf, uInLen);
            }

            if (uOutLen)
            {
                ProbeForWrite(pOutBuf, uOutLen, 4);
                RtlCopyMemory(pOutBuf, DeviceObject->DeviceExtension, uOutLen);
            }
        }
        _except(EXCEPTION_EXECUTE_HANDLER) {
            KdPrint(("exception!\n"));
            status = STATUS_UNSUCCESSFUL;
        }
        break;

    case IOCTL_SHARE_MEMORY:
        pOutBuf = Irp->AssociatedIrp.SystemBuffer;
        //���豸��չ��Ϊ�����ڴ��ַ����СΪ10�ֽ�
        pSysAddr = DeviceObject->DeviceExtension;
        pMdl = IoAllocateMdl(pSysAddr, 10, FALSE, FALSE, NULL);

        if (NULL == pMdl) {
            KdPrint(("IOCTL_SHARE_MEMORY IoAllocateMdl Failure!\n"));
            status = STATUS_UNSUCCESSFUL;
            break;
        }

        MmBuildMdlForNonPagedPool(pMdl);
        *(ULONG*)pOutBuf = MmMapLockedPagesSpecifyCache(
                               pMdl,
                               UserMode,
                               MmNonCached,
                               NULL,
                               FALSE,
                               NormalPagePriority
                           );

        if (NULL == *(ULONG*)pOutBuf) {
            KdPrint(("IOCTL_SHARE_MEMORY MmMapLockedPagesSpecifyCache Failure\n"));
            status = STATUS_UNSUCCESSFUL;
            IoFreeMdl(pMdl);
            break;
        }

        KdPrint(("IOCTL_SHARE_MEMORY SysBuf:0x%08x UserBuf:0x%08x\n",
                 pSysAddr, *(ULONG*)pOutBuf));
        break;

    default:
        status = STATUS_UNSUCCESSFUL;
        break;
    }

    Irp->IoStatus.Status = status;
    Irp->IoStatus.Information = uOutLen;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return status;
}