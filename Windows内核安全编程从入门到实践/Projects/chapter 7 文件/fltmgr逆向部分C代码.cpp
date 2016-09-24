\\
\\XP sp3
\\

//
//�������˵����ݽṹ
//
typedef struct _FLT_SERVER_PORT_OBJECT_ {
    PFLT_CONNECT_NOTIFY  ConnectNotify;       //΢�����������Ӵ�������ַ
    PFLT_DISCONNECT_NOTIFY  DisconnectNotify; //�Ͽ����Ӵ�������ַ
    PFLT_MESSAGE_NOTIFY  MessageNotify;       //����Ӧ�ò㷢�͵���Ϣ�ĺ�����ַ
    PFLT_FILTER  Filter;                      //����FltRegisterFilter�õ��Ķ���
    PVOID  Cookie;
    ULONG  Flags;
    ULONG  NumberOfConnections;               //��ǰ���ӵ�����˵�������
    ULONG  MaxConnections;

} FLT_SERVER_PORT_OBJECT, *PFLT_SERVER_PORT_OBJECT;


//
//����һ������ͨ�ŵķ���˿ڣ�
//   ʵ���Ͼ�������һ������ΪFilterConnectionPort�Ķ���
//�������ӵȴ�������ַ����ö���
//
NTSTATUS
FltCreateCommunicationPort(
    IN PFLT_FILTER  Filter,
    OUT PFLT_PORT  *ServerPort,
    IN POBJECT_ATTRIBUTES  ObjectAttributes,
    IN PVOID  ServerPortCookie OPTIONAL,
    IN PFLT_CONNECT_NOTIFY  ConnectNotifyCallback,
    IN PFLT_DISCONNECT_NOTIFY  DisconnectNotifyCallback,
    IN PFLT_MESSAGE_NOTIFY  MessageNotifyCallback,
    IN LONG  MaxConnections
)
{
    NTSTATUS status;
    PFLT_SERVER_PORT_OBJECT pObject;

    //
    //����һЩ�����Ϸ��Լ��
    //
    if (MaxConnections <= 0) {
        return STATUS_INVALID_PARAMETER;
    }

    if (!(ObjectAttributes->Attributes & OBJ_KERNEL_HANDLE)) {
        return STATUS_INVALID_PARAMETER;
    }

    status = FltObjectReference(Filter);

    if (!NT_SUCCESS(status)) {
        return status;
    }

    //����һ������ΪFilterConnectionPortType�Ķ��󣬴�СΪ0x20,
    //���Ӧ�����ݽṹΪFLT_SERVER_PORT_OBJECT
    status = ObCreateObject(0, FilterConnectionPortType, ObjectAttributes, 0, 0, 0x20, 0, 0, &pObject);

    if (!NT_SUCCESS(status)) {
        return status;
    }

    RtlZeroMemory(pObject, 0x20);
    FltpObjectPointerReference(Filter);
    //����һЩ������ַ���Թ�֮�����
    pObject->ConnectNotify = ConnectNotifyCallback;
    pObject->DisconnectNotify = DisconnectNotifyCallback;
    pObject->MessageNotify = MessageNotifyCallback;
    pObject->Filter = Filter;
    pObject->Cookie = ServerPortCookie;
    pObject->MaxConnections = MaxConnections;
    //��ӵ��������
    status = ObInsertObject(pObject, 0, 0x1f0001, 0, 0, ServerPort);
    return status;
}




FilterConnectCommunicationPort:

1:
kd > kb
ChildEBP RetAddr  Args to Child
b28a695c f84af70c 800007e0 00000000 00000000 scanner!ScannerPortConnect + 0x5 [d:\projects\scanner\scanner.c @ 299]
b28a6a0c f849ea56 8235d970 82429078 82429088 fltMgr!FltpOpenCommunicationPort + 0x29c
b28a6a2c f84ab53d 82187648 82429078 82181918 fltMgr!FltpMsgDispatch + 0xa6
b28a6a5c 804f018f 82187648 82429078 82429078 fltMgr!FltpCreate + 0x53
b28a6a6c 805841fa 82187630 821622dc b28a6c04 nt!IopfCallDriver + 0x31
b28a6b4c 805c0444 82187648 00000000 82162238 nt!IopParseDevice + 0xa12
b28a6bc4 805bc9d0 00000000 b28a6c04 00000040 nt!ObpLookupObjectName + 0x53c
b28a6c18 80577033 00000000 00000000 8a6c8401 nt!ObOpenObjectByName + 0xea
b28a6c94 805779aa 0012fe30 00100003 0012fde8 nt!IopCreateFile + 0x407
b28a6cf0 8057a0b4 0012fe30 00100003 0012fde8 nt!IoCreateFile + 0x8e
b28a6d30 8054261c 0012fe30 00100003 0012fde8 nt!NtCreateFile + 0x30
b28a6d30 7c92e4f4 0012fe30 00100003 0012fde8 nt!KiFastCallEntry + 0xfc
0012fda4 7c92d09c 4aa9149b 0012fe30 00100003 ntdll!KiFastSystemCallRet
0012fda8 4aa9149b 0012fe30 00100003 0012fde8 ntdll!ZwCreateFile + 0xc
0012fe34 004012aa 0040b3a4 00000000 00000000 FLTLIB!FilterConnectCommunicationPort + 0x13f
0012ff78 0040180e 00000001 003b2ea0 003b2ef8 scanuser!main + 0xaa [d:\projects\scanner\scanuser.c @ 318]
003b0518 00000000 003b0520 003b0520 003b0528 scanuser!__tmainCRTStartup + 0xfb [f:\dd\vctools\crt_bld\self_x86\crt\src\crt0.c @ 266]


HRESULT
WINAPI
FilterConnectCommunicationPort(
    IN LPCWSTR  lpPortName,
    IN DWORD  dwOptions,
    IN LPVOID  lpContext OPTIONAL,
    IN DWORD  dwSizeOfContext,
    IN LPSECURITY_ATTRIBUTES  lpSecurityAttributes OPTIONAL,
    OUT HANDLE  *hPort
)
{
    ...
    FltpOpenCommunicationPort(fileobj, pIrp);
    ...
}

typedef struct _FLT_MUTEX_LIST_HEAD_ {
    FAST_MUTEX mLock;
    LIST_ENTRY mList;                         //�������Ϣ����
    ULONG mCount;

} FLT_MUTEX_LIST_HEAD;

typedef struct _FLT_MESSAGE_WAITER_QUEUE_ {
    IO_CSQ Csq;
    FLT_MUTEX_LIST_HEAD WaiterQ;               //�����������Ϣ����
    ULONG MinimumWaiterLength;
    KSEMAPHORE Semaphore;                      //�źŵƣ�����Ϣ�����й�
    KEVENT Event;                              //Ӧ�ò�ÿͶ��һ�ν������ø��¼���������FltSendMessage�Ⱥ����еĶ�Ӧ��KeWaitForMultipleObjects������

} FLT_MESSAGE_WAITER_QUEUE, *PFLT_MESSAGE_WAITER_QUEUE;

//
//����ͨ�ŵ���һ�ˡ����ͻ��˶������ݽṹ
//
typedef struct _FLT_PORT_OBJECT_ {
    LIST_ENTRY Links;
    PFLT_SERVER_PORT_OBJECT ServerPort;         //ָ���˷���˿�
    PVOID Cookie;
    FAST_MUTEX Lock;
    FLT_MESSAGE_WAITER_QUEUE MsgQ;              //�����������Ϣ����ȣ�Ӧ�ò����FilterGetMessage�������ϢͶ�ݵ�����
    ULONGULONG MessageId;                       //��ϢID,��1Ϊ��λ����
    KEVENT  DisconnectEvent;
    UCHAR Disconnected;

} FLT_PORT_OBJECT, *PFLT_PORT_OBJECT;


typedef _PORT_CCB_ {
    PFLT_PORT_OBJECT Port;                      //������ͨ�ſͻ��˵ĵ�ַ
    FLT_MUTEX_LIST_HEAD ReplyWaiterList;        //��������Ҫ���reply���������ÿһ����Ԫ����������ΪReplyWaitNode

} PORT_CCB, *PPORT_CCB;

typedef struct _FLT_TYPE_ {
    WORD Signature;
    WORD Size;

} FLT_TYPE, *PFLT_TYPE;

typedef union _FLT_CCB_DATA_ {
    MANAGER_CCB Manager;
    FILTER_CCB Filter;
    INSTANCE_CCB Instance;
    VOLUME_CCB Volume;
    PORT_CCB Port;
} FLT_CCB_DATA, *PFLT_CCB_DATA;

//
//�����ݽṹ���Ա���ܶ���CCB�����ݣ���PORT_CCB��
//�����ݽṹ�������ڶ�Ӧfileobject��FsContext2��
//
typedef struct _FLT_CCB {
    FLT_TYPE Type;
    FLT_CCB_DATA Data;

} FLT_CCB, *PFLT_CCB;

//
//Ӧ�ò����FilterConnectCommunicationPort������øú�����
//�ú�����Ҫ����һ������ΪFilterCommunicationPortType�Ķ������ͻ���ͨ�Ŷ˿ڣ�
//���⣬�������һ��FLT_CCB�ṹ�������ļ������У��Ժ�ͨ������ļ������ܻ��
//reply��Ϣ�����Լ�Ӧ�ó���Ͷ�ݵ���Ϣ����
//
NTSTATUS
FltpOpenCommunicationPort(PFILE_OBJECT fileobj, PIRP pIrp)
{
    NTSTATUS status;
    PFLT_PORT_OBJECT pClientPortObject;
    PFLT_SERVER_PORT_OBJECT pServerPortObject;
    PVOID pBuffer;
    OBJECT_ATTRIBUTES oa;
    HANDLE hClientPortHandle;
    PFLT_CCB pFltccb;

    //
    //�������
    //
    if (fileobj->FileName.Length <= 0) {
        return STATUS_ACCESS_DENIED;
    }

    pBuffer = pIrp->AssociatedIrp.SystemBuffer;

    if (pBuffer == NULL) {
        return STATUS_ACCESS_DENIED;
    }

    if (*(WORD*)((ULONG)pBuffer + 6) < 0x10) {
        return STATUS_ACCESS_DENIED;
    }

    if (*(BYTE*)((ULONG)pBuffer + 5) != 7) {
        return STATUS_ACCESS_DENIED;
    }

    if (RtlCompareMemory((ULONG)pBuffer + 8, "FLTPORT", 7) != 7) {
        return STATUS_ACCESS_DENIED;
    }

    if (*(WORD*)((ULONG)pBuffer + 0x18) + 0x10 > *(WORD*)((ULONG)pBuffer + 6)) {
        return STATUS_ACCESS_DENIED;
    }

    //��ȡͨ�ŷ���˶���
    status = ObReferenceObjectByName(
                 *(UNICODE_STRING*)(ULONG)pBuffer + 0x10, //"ScannerPort"
                 0,
                 0,
                 0x1f0001,
                 FilterConnectionPortType,
                 ExGetPreviousMode(),
                 0,
                 &pServerPortObject
             );

    if (!NT_SUCCESS(status)) {
        return STATUS_ACCESS_DENIED;
    }

    //������������
    pServerPortObject->NumberOfConnections += 1;

    if (pServerPortObject->NumberOfConnections > pServerPortObject->MaxConnections) {
        return STATUS_CONNECTION_COUNT_LIMIT;
    }

    oa.Length = 0x18;
    oa.RootDirectory = NULL;
    oa.ObjectName = NULL;
    oa.Attributes = 0x200;
    oa.SecurityDescriptor = 0;
    oa.SecurityQualityOfService = 0;
    //����һ������ΪFilterCommunicationPortType�Ķ��󣬴���ͨ�ŵĿͻ���
    status = ObCreateObject(
                 0,
                 FilterCommunicationPortType,
                 &oa,
                 0,
                 0,
                 0xc8,
                 0,
                 0,
                 &pClientPortObject
             );

    if (!NT_SUCCESS(status)) {
        return status;
    }

    //
    //��ʼ������ͻ��˶���
    //
    RtlZeroMemory(pClientPortObject, 0xc8);
    pClientPortObject->Lock.Count = 1;
    pClientPortObject->Lock.Owner = NULL;
    pClientPortObject->Lock.Contention = 0;
    KeInitializeEvent(&pClientPortObject->Lock.Event, SynchronizationEvent, false);
    //������ͨ�ŷ���˵Ķ���
    pClientPortObject->ServerPort = pServerPortObject;
    pClientPortObject->MsgQ.WaiterQ.mCount = 0;
    pClientPortObject->MsgQ.WaiterQ.mLock.Count = 1;
    pClientPortObject->MsgQ.WaiterQ.mLock.Owner = NULL;
    pClientPortObject->MsgQ.WaiterQ.mLock.Contention = NULL;
    KeInitializeEvent(&pClientPortObject->MsgQ.WaiterQ.mLock.Event, SynchronizationEvent, false);
    //��ʼ��Ͷ����Ϣ����Ӧ�ó���ÿ����һ��FilterGetMessage�����ڸ�����������һ���ڵ㣨IRP��,
    //��΢������������һ��FltSendMessage��������һ���ýڵ㣨�����IRP��
    pClientPortObject->MsgQ.WaiterQ.mList.Flink = &pClientPortObject->MsgQ.WaiterQ.mList;
    pClientPortObject->MsgQ.WaiterQ.mList.Blink = &pClientPortObject->MsgQ.WaiterQ.mList;
    pClientPortObject->MsgQ.MinimumWaiterLength |= 0xffffffff;
    //��ʼ���źŵƣ��źŵƵ���������ǰͶ�ݵ�mList��IRP������
    KeInitializeSemaphore(&pClientPortObject->MsgQ.Semaphore, 0, 0x7fffffff);
    //��ʼ���¼���ÿͶ��һ�����󣬶������ø��¼����ź�
    KeInitializeEvent(&pClientPortObject->MsgQ.Event, NotificationEvent, false);
    KeInitializeEvent(&pClientPortObject->DisconnectEvent, NotificationEvent, false);
    //��ʼ��IoCsq
    FltpIoCsqInitializeEx(
        &pClientPortObject->MsgQ,
        FltpAddMessageWaiter,
        FltpRemoveMessageWaiter,
        FltpGetNextMessageWaiter,
        FltpAcquireMessageWaiterLock,
        FltpReleaseMessageWaiterLock,
        FltpCancelMessageWaiter
    );
//FltpIoCsqInitializeEx�����ݼ���
//  pClientPortObject->MsgQ.Csq.Type = 3;
//  pClientPortObject->MsgQ.Csq.CsqInsertIrp = FltpAddMessageWaiter;
//  pClientPortObject->MsgQ.Csq.CsqRemoveIrp = FltpRemoveMessageWaiter;
//  pClientPortObject->MsgQ.Csq.CsqPeekNextIrp = FltpGetNextMessageWaiter;
//  pClientPortObject->MsgQ.Csq.CsqAcquireLock = FltpAcquireMessageWaiterLock;
//  pClientPortObject->MsgQ.Csq.CsqReleaseLock = FltpReleaseMessageWaiterLock;
//  pClientPortObject->MsgQ.Csq.CsqCompleteCanceledIrp = FltpCancelMessageWaiter;
//  pClientPortObject->MsgQ.Csq.ReservePointer = NULL;
    //���ͻ��˶�����ӵ��������
    status = ObInsertObject(pClientPortObject, 0, 0x1f0001, 1, 0, &hClientPortHandle);

    if (!NT_SUCCESS(status)) {
        return status;
    }

    //����һ��FLT_CCB�ṹ
    //1.�ýṹ�ᱣ��ͻ��˶���
    //2.�ýṹ�ᱣ��Reply����
    pFltccb = FltpAllocateFltCcb(0x0f10d, 0);

    if (NULL = pFltccb) {
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    //����ͻ��˶���
    pFltccb->Data->Port.Port = pClientPortObject;
    pFltccb->Data->Port.ReplyWaiterList.mCount = 0;
    pFltccb->Data->Port.ReplyWaiterList.mLock.Count = 1;
    pFltccb->Data->Port.ReplyWaiterList.mLock.Owner = NULL;
    pFltccb->Data->Port.ReplyWaiterList.mLock.Contention = 0;
    KeInitializeEvent(&pFltccb->Data->Port.ReplyWaiterList.mLock.Event, SynchronizationEvent, false);
    //��ʼ��Reply�������������������΢������������FltSendMessageϣ����ûظ�������
    //΢��������ÿ����һ��FltSendMessage��ReplyBuffer ������Ϊ�գ������ڸ����������һ������
    //��Ӧ�ó������FilterReplyMessage������ɸ������е�һ������
    pFltccb->Data->Port.ReplyWaiterList.mList.Flink = &pFltccb->Data->Port.ReplyWaiterList.mList;
    pFltccb->Data->Port.ReplyWaiterList.mList.Blink = &pFltccb->Data->Port.ReplyWaiterList.mList;
    status = FltObjectReference(pServerPortObject->Filter);

    if (!NT_SUCCESS(status)) {
        return status;
    }

    //����΢������������˹�����ע������Ӵ�����
    status = pServerPortObject->ConnectNotify(
                 hClientPortHandle,
                 pServerPortObject->Cookie,
                 (ULONG)pBuffer + 0x20,
                 *(WORD*)((ULONG)pBuffer + 0x18),
                 pClientPortObject->Cookie
             );

    if (!NT_SUCCESS(status)) {
        FltObjectDereference(pServerPortObject->Filter);
        return status;
    }

    //�������FLT_CCB�ṹ���ļ�����ṹ��
    fileobj->FsContext2 = pFltccb;
    pClientPortObject->Links.Flink = 0;
    ExAcquireFastMutex(&pServerPortObject->Filter->PortList.mLock);

    if (!pServerPortObject->Filter->PortList.mInvalid) {
        pServerPortObject->Filter->PortList.mCount += 2;
        InsertHeadList(&pServerPortObject->Filter->PortList.mList, &pClientPortObject->Links);
    }

    ExReleaseFastMutex(&pServerPortObject->Filter->PortList.mLock);
    FltObjectDereference(pServerPortObject->Filter);
    return status;
}

//
//��������������FltSendMessage�����Ҳ���ReplyBuffer��Ϊ�յ�ʱ��
//������FLT_CCB��Reply�����в���һ���ýṹ
//
typedef struct _ReplyWaitNode_ {
    LIST_ENTRY entry;
    ULONGULONG msg;     //��ϢID�ţ�������λ��Ϣ
    KEVENT kevent;      //FltSendMessage��ȴ����¼�����Ӧ�ó������FilterReplyMessage�����ø��¼����ź�
    PVOID ReplyBuf;     //FltSendMessage�ṩ�Ļָ���������FilterReplyMessage�������û�����
    ULONG ReplyLen;     //ReplyBuf��С
    NTSTATUS status;
    ULONG UnKnow2;

} ReplyWaitNode, *PReplyWaitNode;

//
//΢�����������øú�����Ӧ�ó�������Ϣ��ReplyBuffer��Ϊ0�����ȴ�Ӧ�ó���Ļظ�
//
NTSTATUS
FltSendMessage(
    __in PFLT_FILTER  Filter,
    __in PFLT_PORT  *ClientPort,
    __in PVOID  SenderBuffer,
    __in ULONG  SenderBufferLength,
    __out_opt PVOID  ReplyBuffer OPTIONAL,
    __inout_opt PULONG  ReplyLength
    __in_opt PLARGE_INTEGER  Timeout
)
{
    NTSTATUS status;
    PFLT_PORT_OBJECT pClientObject;
    PVOID pWaitObject[2];
    PIRP pIrp;
    PMDL pMdlUserAddr;
    PVOID pAddrbyMdl;
    ReplyWaitNode WaitNode;
    FLT_MUTEX_LIST_HEAD *pReplyWaiter;
    ...
    //��ȡͨ�ſͻ���
    status = ObReferenceObjectByHandle(
                 *ClientPort,
                 0x1f0001,
                 FilterCommunicationPortType,
                 0,
                 &pClientObject,
                 0
             );

    if (!NT_SUCCESS(status)) {
        return status;
    }

    ...
    pWaitObject[0] = &pClientObject->DisconnectEvent;   //ͨ�ŶϿ��¼�
    pWaitObject[1] = &pClientObject->MsgQ.Semaphore;    //�ȴ���Ϣ�źŵ�

    do {
        status = KeWaitForMultipleObjects(
                     2,
                     pWaitObject,
                     WaitAny,
                     Executive,
                     KernelMode,
                     false,
                     Timeout,
                     NULL
                 );

        if (STATUS_TIMEOUT == status) {
            return status;
        } else if (STATUS_WAIT_0 == status) {
            return  STATUS_PORT_DISCONNECTED;
        }

        //�ӵȴ���Ϣ������ȡ��һ�����ʣ���������С��ͬ����IRP��
        //���û���ҵ���С���ʵ�IRP������ȡ�������еĵ�һ��IRP
        pIrp = FltpIoCsqRemoveNextIrp(&pClientObject->MsgQ, SenderBufferLength + 0x10);

        if (pIrp == NULL) {
            ...
            continue;
        }

        //�ж�ȡ����IRP��С�Ƿ���ʣ����ֱ���������
        if (pIrp->Tail.Overlay.CurrentStackLocation->Parameters.Read.Length < SenderBufferLength + 0x10) {
            pIrp->IoStatus.Status = STATUS_BUFFER_TOO_SMALL;
            pIrp->IoStatus.Information = SenderBufferLength + 0x10;
            ExAcquireFastMutex(&pClientObject->MsgQ.WaiterQ.mLock);

            if (pClientObject->MsgQ.MinimumWaiterLength > SenderBufferLength + 0x10) {
                pClientObject->MsgQ.MinimumWaiterLength = SenderBufferLength + 0x10;
                KeClearEvent(&pClientObject->MsgQ.Event);
            }

            ExReleaseFastMutex(&pClientObject->MsgQ.WaiterQ.mLock);
            //������IRP������Ӧ�ó������ṩ�Ļ�����̫С�����ڴ�Ӧ�ó�������Ͷ��һ����С���ʵ�����
            IofCompleteRequest(pIrp, 0);
            pWaitObject[1] = &pClientObject->MsgQ.Event;
            //�ȴ�Ӧ�ó�������Ͷ�ݣ�Ӧ�ó���һ��Ͷ�ݽ�������pClientObject->MsgQ.Event����¼����μ�FilterGetMessage��
            status = KeWaitForMultipleObjects(
                         2,
                         pWaitObject,
                         WaitAny,
                         Executive,
                         KernelMode,
                         false,
                         Timeout,
                         NULL
                     );

            if (status == STATUS_TIMEOUT) {
                return status;
            } else if (status == STATUS_WAIT_0) {
                return STATUS_PORT_DISCONNECTED;
            }

            pIrp = NULL;
            ...
            continue;
        }

        //
        //ʹ��MDLӳ��Ӧ�ó�������Ļ���������΢�����������͵���Ϣ������ӳ���ĵ�ַ��
        //
        if (!pIrp->MdlAddress) {
            //+0x10��Ϊ�˷���FILTER_MESSAGE_HEADER��Ҫ��sizeof(FILTER_MESSAGE_HEADER)==0x10
            pMdlUserAddr = IoAllocateMdl(pIrp->UserBuffer, SenderBufferLength + 0x10, false, true, NULL);
            pIrp->MdlAddress = pMdlUserAddr;

            if (NULL == pMdlUserAddr) {
                pIrp->IoStatus.Status = STATUS_INSUFFICIENT_RESOURCES;
                pIrp->IoStatus.Information = 0;
                IofCompleteRequest(pIrp, 0);
                pIrp = NULL;
                ...
                continue;
            }

            MmProbeAndLockProcessPages(pMdlUserAddr, IoGetRequestorProcess(pIrp), pIrp->RequestorMode, 1);
        }

        if (pIrp->MdlAddress.MdlFlags & 5) {
            pAddrbyMdl = MmMapLockedPagesSpecifyCache(pMdlUserAddr, KernelMode, MmCached, NULL, 0, 0x10);
        } else {
            pAddrbyMdl = pMdlUserAddr->MappedSystemVa;
        }

        if (NULL == pAddrbyMdl) {
            ExRaiseStatus(STATUS_INSUFFICIENT_RESOURCES);
        }

        RtlZeroMemory(pAddrbyMdl, SenderBufferLength = 0x10);
        RtlCopyMemory((ULONG)pAddrbyMdl + 0x10, SenderBuffer, SenderBufferLength);
        pIrp->IoStatus.Status = STATUS_SUCCESS;
        pIrp->IoStatus.Information = SenderBufferLength + 0x10;
        break;
    } while (true);

    //���ReplyBufferΪ�գ���ô�ú������˾ͽ����ˣ�
    //������Ҫ����ָ�����
    if (NULL == ReplyBuffer) {
        IofCompleteRequest(pIrp, 0);
        return status;
    }

    //����һ��ReplyWaitNode�ṹ���ýṹ������һ���ظ����󣬲������浽PORT_CCB�ṹ�Ļظ��ȴ�������
    RtlZeroMemory(&WaitNode, sizeof(ReplyWaitNode));
    WaitNode.UnKnow2 |= 1;
    WaitNode.msg = ++pClientObject->MessageId;
    WaitNode.ReplyBuf = ReplyBuffer;
    WaitNode.ReplyLen = *(ULONG*)ReplyLength;
    KeInitializeEvent(&WaitNode.kevent, NotificationEvent, false);
    ((PFILTER_MESSAGE_HEADER)pAddrbyMdl)->ReplyLength = WaitNode.ReplyLen + 0x10;
    ((PFILTER_MESSAGE_HEADER)pAddrbyMdl)->MessageId = WaitNode.msg;
    pReplyWaiter = &(((PORT_CCB*)((FLT_CCB*)pIrp->Tail.Overlay.CurrentStackLocation->FileObject->FsContext2).Data.Port).ReplyWaiterList);
    ExAcquireFastMutex(&pReplyWaiter->mLock);

    if (pReplyWaiter->mCount & 1 == 0) {
        InsertTailList(&pReplyWaiter->mList, &WaitNode.entry);
    }

    ExReleaseFastMutex(&pReplyWaiter->mLock);
    //���IRP���ȴ�Ӧ�ó���ظ�
    IofCompleteRequest(pIrp, 0);
    //�ȴ��¼���FilterReplyMessage�����ø��¼����ź�
    pWaitObject[1] = &WaitNode.kevent;
    status = KeWaitForMultipleObjects(
                 2,
                 pWaitObject,
                 WaitAny,
                 Executive,
                 KernelMode,
                 false,
                 Timeout,
                 NULL
             );

    if (status == STATUS_TIMEOUT) {
        ...
    } else if (STATUS_WAIT_0 == status) {
        return STATUS_PORT_DISCONNECTED;
    }

    status = WaitNode.status;

    if (ReplyLength) {
        *(ULONG*)ReplyLength = WaitNode.ReplyLen;
    }

    return status;
}



//
//Ӧ�ó�������������Ͷ��һ����Ϣ����ͨ�ſͻ�����Ϣ�ȴ�������
//
HRESULT
WINAPI
FilterGetMessage(
    IN HANDLE  hPort,
    IN OUT PFILTER_MESSAGE_HEADER  lpMessageBuffer,
    IN DWORD  dwMessageBufferSize,
    IN LPOVERLAPPED  lpOverlapped OPTIONAL
)
{
    ...
    //FltpGetMessage�����FltpIoCsqInsertIrpEx��FltpIoCsqInsertIrpEx����FltpAddMessageWaiter
    FltpGetMessage();
    ...
}

//
//FltpAddMessageWaiterֻ�ǽ����������IRP��ӵ�MsgQ.WaiterQ.mList�����У�������MsgQ.Event
//
NTSTATUS
FltpAddMessageWaiter��FLT_MESSAGE_WAITER_QUEUE* pMsgQ, PIRP pIrp�� {
    if (pMsgQ->WaiterQ.mCount & 1 == 0)
    {
        pMsgQ->WaiterQ.mCount += 2;
        InsertTailList(&pMsgQ->WaiterQ.mList, &pIrp->Tail.Overlay.ListEntry);

        if (pIrp->Tail.Overlay.CurrentStackLocation.Parameters.Read.Length > pMsgQ->MinimumWaiterLength) {
            KeSetEvent(&pMsgQ->Event, 0, 0);
        }

        //����һ���źŵ�
        KeReleaseSemaphore(&pMsgQ->Semaphore, 1, 1, 0);
    } else
    {
        return STATUS_PORT_DISCONNECTED;
    }

    return STATUS_PENDING;
};




//
//Ӧ�ó��������������ظ�΢���������Ļظ�����
//
HRESULT
WINAPI
FilterReplyMessage(
    __in HANDLE  hPort,
    __in PFILTER_REPLY_HEADER  lpReplyBuffer,
    __in DWORD  dwReplyBufferSize
)
{
    ...
    FltpFilterReply(...);
    ...
}

//
//�����������һ��΢���������ĵȴ��ظ�����
//
NTSTATUS
FltpFilterReply(PFILE_OBJECT FileObj, PVOID ReplyBuf, ULONG ReplyLen, PULONG Retlen, BOOLEAN InKernel)
{
    NTSTATUS status;
    FLT_MUTEX_LIST_HEAD *pReplyWaiter;
    ReplyWaitNode* pWaitNode;
    ULONG uBufLen;
    *Retlen = 0;

    if (ReplyLen < 0x10) {
        return STATUS_INVALID_PARAMETER;
    }

    if (InKernel == FALSE) {
        ProbeForRead(ReplyBuf, ReplyLen, 4);
    }

    status = ((FILTER_REPLY_HEADER*)ReplyBuf).Status;
    //ReplyWaitNode�����������ļ��������ˣ��μ�FltpOpenCommunicationPort����
    pReplyWaiter = ((PORT_CCB*)(((FLT_CCB*)FileObj->FsContext2).Data.Port))->ReplyWaiterList;
    ExAcquireFastMutex(&pReplyWaiter->mLock);
    pWaitNode = pReplyWaiter->mList.Flink;

    if (pWaitNode == &pReplyWaiter->mList) {
        ExReleaseFastMutex(&pReplyWaiter->mLock);
        return 0xC01C0020;
    }

    //�ҵ�����ɵĵȴ��ظ����󣬲�ȡ��
    do {
        if (pWaitNode->msg == ((FILTER_REPLY_HEADER*)ReplyBuf)->MessageId) {
            pWaitNode->entry.Blink->Flink = pWaitNode->entry.Flink;
            pWaitNode->entry.Flink->Blink = pWaitNode->entry.Blink;
            break;
        }

        pWaitNode = pWaitNode->entry.Flink;
    } while (pWaitNode != &pReplyWaiter->mList);

    ExReleaseFastMutex(&pReplyWaiter->mLock);

    if (pWaitNode == &pReplyWaiter->mList) {
        return 0xC01C0020;
    }

    uBufLen = ReplyLen - 0x10;

    if (uBufLen >= pWaitNode->ReplyLen) {
        uBufLen = pWaitNode->ReplyLen;
    }

    //��仺����
    RtlCopyMemory(pWaitNode->ReplyBuf, (ULONG)ReplyBuf + 0x10, uBufLen);

    if (ReplyLen - 0x10 > uBufLen) {
        status = STATUS_BUFFER_OVERFLOW;
    }

    *Retlen = uBufLen;
    pWaitNode->status = status;
    pWaitNode->ReplyLen = uBufLen;
    //�����¼���FltSendMessage�����ڵȴ����¼�
    KeSetEvent(&pWaitNode->kevent, 0, 0);
    return (status == STATUS_BUFFER_OVERFLOW) ? STATUS_BUFFER_OVERFLOW : STATUS_SUCCESS;
}