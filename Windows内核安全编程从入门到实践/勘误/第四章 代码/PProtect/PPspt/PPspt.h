// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� PPSPT_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// PPSPT_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef PPSPT_EXPORTS
    #define PPSPT_API __declspec(dllexport)
#else
    #define PPSPT_API __declspec(dllimport)
#endif


#define fPProtect                   0x01<<0
#define fAppRun                     0x01<<1

#define SetBit(_a,_b) (_a |= _b)
#define ClearBit(_a,_b) (_a &= ~_b)

typedef struct _ProMonItem_ {
    WCHAR wProcessName[50];
    int oper;
    WCHAR sha1[21];
    WCHAR wProcessPath[200];
    WCHAR wLastTime[20];

} ProMonItem;

typedef struct _NotifyBlock {
    ULONG   NotifyType;
    WCHAR   CurPro[260];                //��ǰ������

    union {
        struct _ProcessMonitor_ {
            WCHAR    R_Pro[100];
            BOOL     SHAChanged;        //�ڰ������в鿴�Ƿ�SHAֵ���޸�
            char     SHA1[20];          //SHAֵ
        } ProMon;

//      struct _AntiZero_
//      {
//          WCHAR dll[260];
//          ULONG AttackType;
//      }Anti0;
    } u;

} NotifyBlock, *pNotifyBlock;

extern "C"
PPSPT_API int fnPPspt(void);

extern "C"
PPSPT_API BOOL fnFirstRun();

extern "C"
PPSPT_API BOOL fnInitApp(
    IN OUT PVOID* pShareMemory
);

extern "C"
PPSPT_API BOOL fnInitReg(OUT int* flagControl);

extern "C"
PPSPT_API BOOL fnGetAntiZInfo(OUT WCHAR* detailInfo);

extern "C"
PPSPT_API BOOL fnUpdateFlag(
    IN int bit,
    IN BOOL bDirect
);

extern "C"
PPSPT_API int fnGetRegProItemCount();

extern "C"
PPSPT_API BOOL fnGetRegProItem(
    IN int index,
    OUT ProMonItem* item
);



extern "C"
PPSPT_API BOOL fnStartChoose();

extern "C"
PPSPT_API BOOL fnSigVerif();

extern "C"
PPSPT_API BOOL fnGetIcoPath(OUT WCHAR* icoPath);

extern "C"
PPSPT_API BOOL fnSetASLRFlag(BOOL bDirect);

BOOL Add2ProMonRules(
    IN BOOL bAllowRun,
    IN WCHAR *wPath
);

BOOL GetSHA1(
    IN WCHAR *wPath,
    OUT char *SHA1
);

DWORD
WINAPI
InitFnSigVerif(LPVOID lpParameter);

BOOL RegDelnode(HKEY hKeyRoot, WCHAR* lpSubKey);
BOOL
DelRegKey(
    HKEY hKeyRoot,
    WCHAR* lpSubKey
);

/************************************************************************/
/* ��־��ӡ                                                             */
/************************************************************************/
#ifdef Debug

VOID DebugPrintA(
    CHAR *Format,
    ...
);

VOID DebugPrintW(
    WCHAR *Format,
    ...
);

#define DbgPrintA(args) DebugPrintA args
#define DbgPrintW(args) DebugPrintW args

#else

#define DbgPrintA(args)
#define DbgPrintW(args)

#endif