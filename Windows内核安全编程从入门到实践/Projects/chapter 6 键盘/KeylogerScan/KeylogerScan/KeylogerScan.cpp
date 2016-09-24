//
//Description:
//  ����������ɨ�輸�ּ��̼�¼��Ϊ�����ָ�������XP sp3ƽ̨�ϲ���ͨ����
//ɨ��ļ��̼�¼��Ϊ������
//              1.���̹�������
//              2.KeyboardClassRead���õ�Hook�Լ�inline hook
//              3.KeyboardClassServiceCallback���õ�Hook�Լ�inline hook
//
//Writen By: ���� 2011/3/30
//
//Last Write Time: 2011/4/1
//
#include "stdafx.h"

#ifdef __cplusplus
    extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath);
#endif

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath)
{
    DriverObject->DriverUnload = KeylogerScanUnload;
    ScanKeyloger();
    return STATUS_SUCCESS;
}

void KeylogerScanUnload(IN PDRIVER_OBJECT DriverObject)
{
    return ;
}


NTSTATUS ScanKeyloger()
{
    NTSTATUS status;
    PContextData pData;
    //��ʼ��һ��ContextData�ṹ�����б����˺ܶ���Ϣ������ɨ��ָ�
    pData = InitContextData();

    if (NULL == pData) {
        KdPrint(("[ScanKeyloger] InitContextData Failure!\n"));
        return STATUS_UNSUCCESSFUL;
    }

    //ɨ����̹����������ָ�
    KdPrint(("\n[ScanKeyloger] ScanKeyboardFilterDriver:\n"));
    status = ScanKeyboardFilterDriver(pData);

    if (NT_SUCCESS(status)) {
        KdPrint(("[ScanKeyloger] ScanKeyboardFilterDriver Finish!\n"));
    } else {
        KdPrint(("[ScanKeyloger] ScanKeyboardFilterDriver Abort!\n"));
    }

    //ɨ��KeyboardClassRead
    KdPrint(("\n[ScanKeyloger] ScanDispatchReadHook:\n"));
    status = ScanDispatchReadHook(pData);

    if (NT_SUCCESS(status)) {
        KdPrint(("[ScanKeyloger] ScanDispatchReadHook Finish!\n"));
    } else {
        KdPrint(("[ScanKeyloger] ScanDispatchReadHook Abort!\n"));
    }

    //ɨ��KeyboardClassServiceCallback
    KdPrint(("\n[ScanKeyloger] ScanServiceCallbackHook:\n"));
    status = ScanServiceCallbackHook(pData);

    if (NT_SUCCESS(status)) {
        KdPrint(("[ScanKeyloger] ScanServiceCallbackHook Finish!\n"));
    } else {
        KdPrint(("[ScanKeyloger] ScanServiceCallbackHook Abort!\n"));
    }

    //�ͷ�ContextData�ṹ
    DeInitContextData(pData);
    return status;
}

PContextData InitContextData()
{
    PContextData pData;
    ULONG uCurKbdClassReadAddr;
    boolean bRet;
    pData = (PContextData)ExAllocatePoolWithTag(NonPagedPool, sizeof(ContextData), 'wjl');

    if (NULL == pData) {
        return NULL;
    }

    RtlZeroMemory(pData, sizeof(ContextData));
    //��ȡ\\Device\\KeyboardClass0�豸����
    pData->KbdClass0DevObj = (PDEVICE_OBJECT)GetKbdClass0DeviceObject();

    if (NULL == pData->KbdClass0DevObj) {
        KdPrint(("[InitContextData] GetKbdClass0DeviceObject Failure!\n"));
        ExFreePoolWithTag(pData, 'wjl');
        pData = NULL;
        return NULL;
    }

    //��ȡKbdclass��������
    pData->KbdClassDrvObj = pData->KbdClass0DevObj->DriverObject;
    //��PE�ļ����ݶ����ڴ��У��Ӷ�����ԭʼ������ַ
    pData->KbdClassDriverFileData = ReadKbdClassDriverFileData();

    if (NULL == pData->KbdClassDriverFileData) {
        KdPrint(("[InitContextData] ReadKbdClassDriverFileData Failure!\n"));
        ExFreePoolWithTag(pData, 'wjl');
        pData = NULL;
        return NULL;
    }

    //��PE�ļ���kbdclass.sys���л�ȡKbdClass��������ں�����DriverEntry����ַ
    bRet = GetReadKbdClassDriverEntryAddrFromPE(pData);

    if (false == bRet) {
        KdPrint(("[InitContextData] GetReadKbdClassDriverEntryAddrFromPE Failure!\n"));
        ExFreePoolWithTag(pData->KbdClassDriverFileData, 'wjl');
        ExFreePoolWithTag(pData, 'wjl');
        pData = NULL;
        return NULL;
    }

    //��PE�ļ��л�ȡKeyboardClassRead�ĵ�ַ
    bRet = GetRealKbdClassReadAddrFromPE(pData);

    if (false == bRet) {
        KdPrint(("[InitContextData] GetRealKbdClassReadAddrFromPE Failure!\n"));
        ExFreePoolWithTag(pData->KbdClassDriverFileData, 'wjl');
        ExFreePoolWithTag(pData, 'wjl');
        pData = NULL;
        return NULL;
    }

    //��PE�ļ��л�ȡKeyboardClassServiceCallback�ĵ�ַ
    bRet = GetKbdServiceCallBackAddrFromPE(pData);

    if (false == bRet) {
        KdPrint(("[InitContextData] GetKbdServiceCallBackAddrFromPE Failure!\n"));
        ExFreePoolWithTag(pData->KbdClassDriverFileData, 'wjl');
        ExFreePoolWithTag(pData, 'wjl');
        pData = NULL;
        return NULL;
    }

    KdPrint(("[InitContextData] Init Finish! KbdClass Info:\n\
DriverObject: 0x%08x\n\
DeviceObject: 0x%08x\n\
KeyboardClassRead: 0x%08x\n\
KeyboardClassServiceCallback: 0x%08x\n",
             pData->KbdClassDrvObj, pData->KbdClass0DevObj,
             pData->KbdClassReadAddr, pData->KbdServiceCallbackAddr
            ));
    return pData;
}

VOID DeInitContextData(PContextData Data)
{
    if (!Data) {
        return ;
    }

    if (Data->KbdClassDriverFileData) {
        ExFreePoolWithTag(Data->KbdClassDriverFileData, 'wjl');
        Data->KbdClassDriverFileData = NULL;
    }

    ExFreePoolWithTag(Data, 'wjl');
    return ;
}

PDEVICE_OBJECT GetKbdClass0DeviceObject()
{
    NTSTATUS status;
    UNICODE_STRING usKbdClass0;
    PDEVICE_OBJECT pDevObj;
    PFILE_OBJECT pFileObject;
    HANDLE hFile;
    OBJECT_ATTRIBUTES oa;
    IO_STATUS_BLOCK ioStatus;
    RtlInitUnicodeString(&usKbdClass0, L"\\Device\\KeyboardClass0");
    InitializeObjectAttributes(&oa, &usKbdClass0, OBJ_CASE_INSENSITIVE, NULL, NULL);
    status = ZwCreateFile(
                 &hFile,
                 GENERIC_READ,
                 &oa,
                 &ioStatus,
                 NULL,
                 FILE_ATTRIBUTE_NORMAL,
                 FILE_SHARE_READ,
                 FILE_OPEN,
                 FILE_SYNCHRONOUS_IO_NONALERT,
                 NULL,
                 0
             );

    if (!NT_SUCCESS(status)) {
        KdPrint(("[GetKbdClass0DeviceObject] ZwCreateFile Failure!err:0x%08x\n",
                 status));
        return NULL;
    }

    status = ObReferenceObjectByHandle(
                 hFile,
                 GENERIC_READ,
                 *IoFileObjectType,
                 KernelMode,
                 (PVOID*)&pFileObject,
                 NULL
             );

    if (!NT_SUCCESS(status)) {
        KdPrint(("[GetKbdClass0DeviceObject] ObReferenceObjectByHandle Failure!err:0x%08x\n",
                 status));
        ZwClose(hFile);
        return NULL;
    }

    ObDereferenceObject(pFileObject);
    ZwClose(hFile);
    pDevObj = pFileObject->DeviceObject;
    return pDevObj;
}

//
//����Ƿ��м��̹��������������ֱ��ժ��
//
NTSTATUS ScanKeyboardFilterDriver(PContextData Data)
{
    PDEVICE_OBJECT pDevObj;
    //AttachedDevice��¼�˰������ϵ��豸����
    pDevObj = Data->KbdClass0DevObj->AttachedDevice;

    if (pDevObj) {
        KdPrint(("[ScanKeyboardFilterDriver] Find Keyboard Filter Driver:%wZ\n",
                 &pDevObj->DriverObject->DriverName));
        IoDetachDevice(Data->KbdClass0DevObj);
    }

    return STATUS_SUCCESS;
}

//
//��������ַ��������������õ����inlinehook���
//
NTSTATUS ScanDispatchReadHook(PContextData Data)
{
    ULONG uReadEntry, uFnRVA;
    //
    //1.���õ���
    //
    //��ȡ�ڴ��еĶ����������õ�
    uReadEntry = (ULONG)&Data->KbdClassDrvObj->MajorFunction[IRP_MJ_READ];

    //�Ա��ڴ��еĶ���������ַ�ʹ�PE�ļ��л�ȡ���Ķ������ַ�Ƿ�һ��
    //�����һ�£�˵����Hook�ˣ��ָ���
    if (*(ULONG*)uReadEntry != Data->KbdClassReadAddr) {
        KdPrint(("[ScanDispatchReadHook] KbdClass Dispatch Hooked!Recover...\n"));
        InterlockedExchange((LONG*)uReadEntry, Data->KbdClassReadAddr);
    }

    KdPrint(("[ScanDispatchReadHook] Scan call entry hook finish.\n"));
    //
    //2.inlinehook��Ⲣ�ָ�
    //
    uFnRVA = (ULONG)Data->KbdClassReadAddr - (ULONG)Data->KbdClassDrvObj->DriverStart;
    ScanInlineHook(Data, (PVOID)uFnRVA);
    return STATUS_SUCCESS;
}

//
//���KeyboardClassServiceCallback�������������õ����inlinehook���
//
NTSTATUS ScanServiceCallbackHook(PContextData Data)
{
    ULONG uFnRVAAddr = 0;
    uFnRVAAddr = Data->KbdServiceCallbackAddr - (ULONG)Data->KbdClassDrvObj->DriverStart;
    //1.���õ���
    ScanKbdServiceCallbackEntryHook(Data);
    //2.inlinehook���
    ScanInlineHook(Data, (PVOID)uFnRVAAddr);
    return true;
}

//KeyboardClassServiceCallback�����ĵ��õ��⺯��
boolean ScanKbdServiceCallbackEntryHook(PContextData Data)
{
    PDRIVER_OBJECT pDrvObj;
    PDEVICE_OBJECT pDevObj;
    ULONG uEntryAddr;
    UNICODE_STRING usI8042;
    //��������֧��PS/2����
    RtlInitUnicodeString(&usI8042, L"\\Driver\\i8042prt");
    pDevObj = (PDEVICE_OBJECT) * (ULONG*)((ULONG)Data->KbdClass0DevObj->DeviceObjectExtension + 0x18);
    pDrvObj = pDevObj->DriverObject;

    if (RtlCompareUnicodeString(&pDrvObj->DriverName, &usI8042, true))
        return false;

    //���¼��̣������жϣ���ִ��DPC����I8042KeyboardIsrDpc��
    //��I8042KeyboardIsrDpc�����У��������KeyboardClassServiceCallback��
    //ͨ�������ã����õ�ΪDeviceExtension+0xf8��
    uEntryAddr = (ULONG)pDevObj->DeviceExtension + 0xf8;

    //�Ա��ڴ��е��õ㱣��ĵ�ַ�ʹ�PE�ļ��л�ȡ��KeyboardClassServiceCallback��ַ�Ƿ�һ��
    //�����һ�£��ָ���
    if (*(ULONG*)uEntryAddr != Data->KbdServiceCallbackAddr) {
        InterlockedExchange((LONG*)uEntryAddr, Data->KbdServiceCallbackAddr);
    }

    return true ;
}

//
//inlinehook��⣬ʹ��libdasm������
//
boolean ScanInlineHook(PContextData Data, PVOID FnRVA)
{
    PIMAGE_DOS_HEADER pDosHdr;
    PIMAGE_NT_HEADERS pNtHdr;
    ULONG uFnRAW = 0;
    INSTRUCTION inst;
    ULONG uFnLen = 0, uCodeLen = 0, uOffset = 0;
    PVOID pCode = NULL;
    char cInstCode[50] = {0};
    int i = 0;
    boolean bRecover = false;
    pDosHdr = (PIMAGE_DOS_HEADER)Data->KbdClassDriverFileData;

    if (IMAGE_DOS_SIGNATURE != pDosHdr->e_magic) {
        return false;
    }

    pNtHdr = (PIMAGE_NT_HEADERS)((ULONG)pDosHdr + pDosHdr->e_lfanew);
    //����������ַת��Ϊ�ļ�ƫ��
    uFnRAW = RVA2RAW(Data->KbdClassDriverFileData, (ULONG)FnRVA);
    //��ȡ����⺯���ĳ���
    uFnLen = GetFunctionLen((ULONG)Data->KbdClassDriverFileData + uFnRAW, 0xc2);
    pCode = ExAllocatePoolWithTag(PagedPool, uFnLen, 'wjl');

    if (NULL == pCode) {
        return false;
    }

    //������⺯�����뿽����������ں˻������У������ض�λ��
    RtlCopyMemory(pCode, (PVOID)((ULONG)Data->KbdClassDriverFileData + uFnRAW), uFnLen);
    //�ض�λ��ƫ��ֵ���ڴ���ʵ�ʼ��صĵ�ַ��PE�ļ��е���ѡ���ص�ַ֮��
    uOffset = (ULONG)Data->KbdClassDrvObj->DriverStart - pNtHdr->OptionalHeader.ImageBase;
    //����PE�ļ��е��ض�λĿ¼��Ϣʵʩ�ض�λ����
    RelocCode(Data->KbdClassDriverFileData, FnRVA, (BYTE*)pCode, uFnLen, uOffset);
    //
    //���ض�λ��Ĵ�����������࣬���ڴ�����Ӧ�Ĵ���ȶԣ�����һ�£�
    //�ָ���
    //
    uFnLen = 0;

    do {
        uCodeLen = get_instruction(&inst, (BYTE*)((ULONG)pCode + uFnLen), MODE_32);

        if (0 == uCodeLen) {
            break;
        }

        if (uCodeLen != RtlCompareMemory(
                (PVOID)((ULONG)pCode + uFnLen),
                (PVOID)((ULONG)Data->KbdClassDrvObj->DriverStart + (ULONG)FnRVA + uFnLen),
                uCodeLen)
           ) {
            get_instruction_string(&inst, FORMAT_INTEL, 0, cInstCode, 50);
            KdPrint(("[ScanInlineHook] Find InlineHooke!\n"));
            KdPrint(("Clean Code: %x %s\n", uFnLen, cInstCode));
            get_instruction(&inst, (BYTE*)((ULONG)Data->KbdClassDrvObj->DriverStart + (ULONG)FnRVA + uFnLen), MODE_32);
            get_instruction_string(&inst, FORMAT_INTEL, 0, cInstCode, 50);
            KdPrint(("Dirty Code: %x %s\n", uFnLen, cInstCode));
            bRecover = true;
            break;
        }

        if ((inst.opcode == 0xc2) || (inst.opcode == 0xc3)) {
            break;
        }

        uFnLen += uCodeLen;
    } while (i++ < 1000);

    //�ָ�����
    if (true == bRecover) {
        KdPrint(("Start to Recover whole Function...\n"));
        RecoverKnrlMemory((PVOID)((ULONG)Data->KbdClassDrvObj->DriverStart + (ULONG)FnRVA), pCode, uFnLen);
    }

    ExFreePoolWithTag(pCode, 'wjl');
    KdPrint(("[ScanInlineHook] Scan inlinehook finish!\n"));
    return true;
}

//
//�ָ��ڴ棬���ɾ��Ĵ��븲����Ĵ��롣
//��Ҫע����ǣ���������һ����Σ���ԣ�����IRQL�������ж�ֻ�ܱ�֤
//����CPU�İ�ȫ�����ڶ�����������ָ�һ���ʱ��������������
//���ܳ����������������ָ��ʺ�С��
//
boolean RecoverKnrlMemory(PVOID Dest, PVOID Source, ULONG Offset)
{
    MDL *pMdl;
    PVOID pMdlAddr = NULL;
    ULONG uFnLen = 0;
    KIRQL OldIrql;
    uFnLen = GetFunctionLen((ULONG)Dest, 0xc2);
    //ʹ��MDL�޸Ĵ���
    pMdl = IoAllocateMdl(Dest, uFnLen, FALSE, FALSE, NULL);

    if (pMdl == NULL)
        return false;

    __try {
        MmProbeAndLockPages(pMdl, KernelMode, IoModifyAccess);
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        IoFreeMdl(pMdl);
        return false;
    }

    pMdlAddr = MmGetSystemAddressForMdl(pMdl);
    //����IRQL����ֹ�̷ַ߳�
    OldIrql = KeRaiseIrqlToDpcLevel();
    //���ο������жϣ����������жϡ�
    //������֤�ڻָ�KeyboardClassServiceCallback�����в����ܼ����ж�Ӱ�졣
    _asm cli
    memmove((PVOID)((ULONG)pMdlAddr + Offset), (PVOID)((ULONG)Source + Offset), uFnLen - Offset);
    _asm sti
    KeLowerIrql(OldIrql);
    MmUnlockPages(pMdl);
    IoFreeMdl(pMdl);
    return true;
}