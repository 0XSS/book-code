#include "stdafx.h"

PCALLBACK_OBJECT pCallback;
PVOID  pRegisterHandle = NULL;

VOID
CallbackFunction(
    IN PVOID CallbackContext,
    IN PVOID Argument1,
    IN PVOID Argument2
);

void ClientUnload(IN PDRIVER_OBJECT DriverObject);

#ifdef __cplusplus
    extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath);
#endif

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath)
{
    NTSTATUS status;
    UNICODE_STRING DemoCallback;
    OBJECT_ATTRIBUTES oa;
    DriverObject->DriverUnload = ClientUnload;

    do {
        //�򿪻ص�����
        RtlInitUnicodeString(&DemoCallback, L"\\Callback\\DemoCallback");
        InitializeObjectAttributes(&oa, &DemoCallback, OBJ_CASE_INSENSITIVE | OBJ_PERMANENT, NULL, NULL);
        //ָ��ExCreateCallback��Create����ΪFALSE�����򿪻ص���������Ǵ���
        status = ExCreateCallback(&pCallback, &oa, FALSE, TRUE);

        if (!NT_SUCCESS(status)) {
            KdPrint(("Client: ExCreateCallback Failure!Status:0x%08x\n", status));
            break;
        }

        KdPrint(("Client ����pCallback: 0x%08x\n", pCallback));
        //��ûص�����ע��һ���ص�����
        pRegisterHandle = ExRegisterCallback(pCallback, (PCALLBACK_FUNCTION)CallbackFunction, NULL);

        if (NULL == pRegisterHandle) {
            KdPrint(("Client: ExRegisterCallback Failure!\n"));
            ObDereferenceObject(pCallback);
            break;
        }

        //����Ҫ�ص�����ʱ���������ô��������ٵ�0ʱ�����ᱻɾ������
        ObDereferenceObject(pCallback);
    } while (FALSE);

    return STATUS_SUCCESS;
}

void ClientUnload(IN PDRIVER_OBJECT DriverObject)
{
    if (pRegisterHandle != NULL)
        ExUnregisterCallback(pRegisterHandle);
}

//
//�ص�����
//  ���г���򿪻��߹ر�Callback��������ʱ��Callback���������ô˺�����
//  ������ֻ�Ǽ򵥵����Argument1��Argument2��Argument1��Argument2��
//�����ɴ����ص�������������򣨱����м�Callback�����壺
//  Argument1:IRP���������루IRP_MJ_CREATE����IRP_MJ_CLOSE����
//  Argument2:�򿪻�ر�callback�����Ľ�����
//
VOID
CallbackFunction(
    IN PVOID CallbackContext,
    IN PVOID Argument1,
    IN PVOID Argument2
)
{
    KdPrint(("Client CallbackFunction���� Argument1: %s Argument2:%s\n",
             ((LONG)Argument1 == IRP_MJ_CREATE) ? "IRP_MJ_CREATE" : (((LONG)Argument1 == IRP_MJ_CLOSE) ? "IRP_MJ_CLOSE" : "exception!"),
             Argument2));
    return ;
}