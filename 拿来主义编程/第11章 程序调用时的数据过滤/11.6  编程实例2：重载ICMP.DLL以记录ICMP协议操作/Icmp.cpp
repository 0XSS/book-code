// Icmp.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Icmp.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

//#pragma comment(lib, "ws2_32.lib")
//#pragma comment (lib, "Mpr.Lib")
//#pragma comment (lib, "IPHlpApi.Lib")

//
//  Note!
//
//      If this DLL is dynamically linked against the MFC
//      DLLs, any functions exported from this DLL which
//      call into MFC must have the AFX_MANAGE_STATE macro
//      added at the very beginning of the function.
//
//      For example:
//
//      extern "C" BOOL PASCAL EXPORT ExportedFunction()
//      {
//          AFX_MANAGE_STATE(AfxGetStaticModuleState());
//          // normal function body here
//      }
//
//      It is very important that this macro appear in each
//      function, prior to any calls into MFC.  This means that
//      it must appear as the first statement within the
//      function, even before any object variable declarations
//      as their constructors may generate calls into the MFC
//      DLL.
//
//      Please see MFC Technical Notes 33 and 58 for additional
//      details.
//

/////////////////////////////////////////////////////////////////////////////
// CIcmpApp

BEGIN_MESSAGE_MAP(CIcmpApp, CWinApp)
    //{{AFX_MSG_MAP(CIcmpApp)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIcmpApp construction

CIcmpApp::CIcmpApp()
{
    hInst = NULL;
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CIcmpApp object

CIcmpApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CIcmpApp initialization

//����������ԭICMP.dll����Ҫ������������
typedef HANDLE(WINAPI *ICMPCREATEFILE)(VOID);
typedef BOOL(WINAPI *ICMPCLOSEHANDLE)(HANDLE IcmpHandle);
typedef DWORD(WINAPI *ICMPSENDECHO)(
    HANDLE IcmpHandle,             //���
    IPAddr DestinationAddress,     //Ŀ��IP��ַ
    LPVOID RequestData,            //�����͵�����
    WORD RequestSize,              //���������ݵĳ���
    PIP_OPTION_INFORMATION RequestOptions,//��ѯʱ���õ�ѡ�
    LPVOID ReplyBuff,              //���ջ�������
    DWORD ReplySize,               //���ջ������ĳ��ȡ�
    DWORD Timeout);                //��ʱֵ��

//���ڱ�����Ҳ��һ��DLL�ļ�������Ϊ�˼���ԭ��ICMP.dll��
//�������潫�亯������Ϊ��������ơ�
ICMPCREATEFILE IcmpCreateFileFromDll;
ICMPCLOSEHANDLE IcmpCloseHandleFromDll;
ICMPSENDECHO IcmpSendEchoFromDll;

BOOL CIcmpApp::InitInstance()
{
    //��ԭDLL�ļ���ע�����ִ������Ͳ�������ϸ���֡�
    hInst = LoadLibrary("ICMP2.dll");

    if (hInst == NULL)
        return FALSE; //���ļ������ڣ�������Ч��DLL�ļ���

    IcmpCreateFileFromDll = (ICMPCREATEFILE)GetProcAddress(hInst, "IcmpCreateFile");
    IcmpCloseHandleFromDll = (ICMPCLOSEHANDLE)GetProcAddress(hInst, "IcmpCloseHandle");
    IcmpSendEchoFromDll = (ICMPSENDECHO)GetProcAddress(hInst, "IcmpSendEcho");

    if (IcmpCreateFileFromDll == NULL || IcmpCloseHandleFromDll == NULL ||
        IcmpSendEchoFromDll == NULL)
        return FALSE; //DLL�ļ��в���������Ҫ����������������ICMP2.dll�ļ���ʽ��汾�д�

    return TRUE;
}

int CIcmpApp::ExitInstance()
{
    // TODO: Add your specialized code here and/or call the base class
    if (hInst != NULL)
        FreeLibrary(hInst);

    return CWinApp::ExitInstance();
}

HANDLE CIcmpApp::IcmpCreateFile()
{
    //��ICMP������
    //AfxMessageBox("����IcmpCreateFile������");
    return IcmpCreateFileFromDll();
}

BOOL CIcmpApp::IcmpCloseHandle(HANDLE IcmpHandle)
{
    //�ر�ICMP������
    //AfxMessageBox("����IcmpCloseHandle������");
    return IcmpCloseHandleFromDll(IcmpHandle);
}

DWORD CIcmpApp::IcmpSendEcho(HANDLE IcmpHandle,
                             IPAddr DestinationAddress, LPVOID RequestData,
                             WORD RequestSize, PIP_OPTION_INFORMATION RequestOptions,
                             LPVOID ReplyBuff, DWORD ReplySize,
                             DWORD Timeout)
{
    //������ICMP���̡�
    //AfxMessageBox("����IcmpSendEcho������");
    //ע������ĺ�����ģ̬�ģ�һ��Ҫ�ȵ��������ز��н����
    DWORD dwRet = IcmpSendEchoFromDll(IcmpHandle, DestinationAddress, RequestData,
                                      RequestSize, RequestOptions, ReplyBuff, ReplySize, Timeout);
    CString strTemp, strTitle, strAll;
    strTitle.Format("ʱ��,Ŀ��IP��ַ,�ظ�IP��ַ,TTL,TOS,��ʱ\r\n");
    PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuff; //�ظ�������
    CString strFilename = ".\\IcmpLog.csv";
    CFile file;
    CFileStatus fs;

    if (file.GetStatus(strFilename, fs)) {
        //����ļ����ڣ�����ļ���
        file.Open(strFilename, CFile::modeReadWrite, NULL);
    } else {
        //����ļ������ڣ����´���һ���ļ���
        file.Open(strFilename, CFile::modeCreate | CFile::modeReadWrite, NULL);
        file.Write(strTitle, strTitle.GetLength());
    }

    file.SeekToEnd();
    CTime time = CTime::GetCurrentTime();
    strAll += time.Format("%H:%M:%S,");
    struct in_addr addr;
    addr.S_un.S_addr = DestinationAddress;
    strTemp.Format("%s,", inet_ntoa(addr));
    strAll += strTemp;
    addr.S_un.S_addr = pEchoReply->Address;
    strTemp.Format("%s,", inet_ntoa(addr));
    strAll += strTemp;
    strTemp.Format("%d,", RequestOptions->Ttl);
    strAll += strTemp;
    strTemp.Format("%d,", RequestOptions->Tos);
    strAll += strTemp;

    if (dwRet != 0) {
        if (pEchoReply->RoundTripTime == 0) {
            strTemp += "С��1 ms";
        } else {
            strTemp.Format("%d ms", pEchoReply->RoundTripTime);
        }
    } else {
        strTemp.Format("��ʱ");
    }

    strAll += strTemp + "\r\n";
    //AfxMessageBox(strTitle+"\r\n"+strAll);
    //д����־�����ر�
    file.Write(strAll, strAll.GetLength());
    file.Close();
    return dwRet;
}

