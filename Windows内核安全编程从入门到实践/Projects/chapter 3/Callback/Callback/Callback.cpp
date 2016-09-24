//
//Description:
//  ����������չʾ�ص�����ļ�ʹ�÷�������XP SP3ƽ̨�ϲ���ͨ����
//  ʹ��vs2008+visualDDK+WDK7100����
//
//Writen By: ���� 2011/4/17
//
//Last Write Time: 2011/4/18
//
#include "stdafx.h"

PCALLBACK_OBJECT pCallback = NULL;

void CallbackUnload(IN PDRIVER_OBJECT DriverObject);
NTSTATUS CallbackCreateClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS CallbackDefaultHandler(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);

#ifdef __cplusplus
    extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath);
#endif

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath)
{
    UNICODE_STRING DeviceName, Win32Device;
    PDEVICE_OBJECT DeviceObject = NULL;
    NTSTATUS status;
    unsigned i;
    UNICODE_STRING DemoCallback;
    OBJECT_ATTRIBUTES  oa;
    RtlInitUnicodeString(&DeviceName, L"\\Device\\Callback0");
    RtlInitUnicodeString(&Win32Device, L"\\DosDevices\\Callback0");

    for (i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
        DriverObject->MajorFunction[i] = CallbackDefaultHandler;

    DriverObject->MajorFunction[IRP_MJ_CREATE] = CallbackCreateClose;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = CallbackCreateClose;
    DriverObject->DriverUnload = CallbackUnload;
    status = IoCreateDevice(DriverObject,
                            0,
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
    status = IoCreateSymbolicLink(&Win32Device, &DeviceName);
    DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;
    //
    //����һ���ص�����
    //  ע��ص������Ƿ���\CallbackĿ¼��
    //
    RtlInitUnicodeString(&DemoCallback, L"\\Callback\\DemoCallback");
    //ע��һ��Ҫָ��OBJ_PERMANENT������ExCreateCallback�޷��ɹ�����һ���������ֵĻص�����
    InitializeObjectAttributes(&oa, &DemoCallback, OBJ_CASE_INSENSITIVE | OBJ_PERMANENT , NULL, NULL);
    status = ExCreateCallback(&pCallback, &oa, TRUE, TRUE);

    if (!NT_SUCCESS(status))
        KdPrint(("Callback ExCreateCallback Failure!status: 0x%08x\n", status));

    KdPrint(("Callback ����pCallback: 0x%08x\n", pCallback));
    return STATUS_SUCCESS;
}

void CallbackUnload(IN PDRIVER_OBJECT DriverObject)
{
    UNICODE_STRING Win32Device;
    RtlInitUnicodeString(&Win32Device, L"\\DosDevices\\Callback0");
    IoDeleteSymbolicLink(&Win32Device);
    IoDeleteDevice(DriverObject->DeviceObject);

    //�����Ļص�������Ҫ����ObDereferenceObject����һ�����ô��������ô���Ϊ0��ɾ������
    if (pCallback)
        ObDereferenceObject(pCallback);
}

//
//���г���򿪻��߹رձ�������ʱ�򣬽�֪ͨ������ص�����ע��ĺ�����
//
NTSTATUS CallbackCreateClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    PIO_STACK_LOCATION pSP = IoGetCurrentIrpStackLocation(Irp);
    char* pImageName = NULL;

    if (pCallback) {
        //��ȡ������,ʹ��Ӳ���룺0x174
        pImageName = (char*)((ULONG)PsGetCurrentProcess() + 0x174);
        //
        //֪ͨע�ắ��
        //arg1:�������루IRP_MJ_READ����IRP_MJ_WRITE��
        //arg2:��ǰ���̵Ľ�����
        //
        ExNotifyCallback(pCallback, (PVOID)pSP->MajorFunction, pImageName);
    }

    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}

NTSTATUS CallbackDefaultHandler(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    Irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return Irp->IoStatus.Status;
}
