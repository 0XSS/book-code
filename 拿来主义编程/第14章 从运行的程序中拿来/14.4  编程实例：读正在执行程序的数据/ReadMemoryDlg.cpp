// ReadMemoryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ReadMemory.h"
#include "ReadMemoryDlg.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

//�߳̽ṹ
typedef struct _threadParamStruct {
    BOOL bRunning;
    CReadMemoryDlg *pDlg;
} THREADPARAM;

THREADPARAM g_threadParam;
UINT DebugThread(LPVOID pParam);

#define LIST_DEBUG_TYPE     0
#define LIST_DEBUG_PROCESS  1
#define LIST_DEBUG_THREAD   2
#define LIST_DEBUG_CONTENT  3

typedef HANDLE(WINAPI *OPENTHREAD)(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwThreadId);
OPENTHREAD OpenThread;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

// Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
    // No message handlers
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReadMemoryDlg dialog

CReadMemoryDlg::CReadMemoryDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CReadMemoryDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CReadMemoryDlg)
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CReadMemoryDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CReadMemoryDlg)
    DDX_Control(pDX, IDC_LIST_Content, m_ctlListContent);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CReadMemoryDlg, CDialog)
    //{{AFX_MSG_MAP(CReadMemoryDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTONOpen, OnBUTTONOpen)
    ON_BN_CLICKED(IDC_BUTTON_DebugStart, OnBUTTONDebugStart)
    ON_BN_CLICKED(IDC_BUTTON_DebugStop, OnBUTTONDebugStop)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReadMemoryDlg message handlers

BOOL CReadMemoryDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    // Add "About..." menu item to system menu.
    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);
    CMenu* pSysMenu = GetSystemMenu(FALSE);

    if (pSysMenu != NULL) {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);

        if (!strAboutMenu.IsEmpty()) {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon
    // TODO: Add extra initialization here
    m_ctlListContent.InsertColumn(LIST_DEBUG_TYPE, "����", LVCFMT_LEFT, 160);
    m_ctlListContent.InsertColumn(LIST_DEBUG_PROCESS, "����ID", LVCFMT_LEFT, 60);
    m_ctlListContent.InsertColumn(LIST_DEBUG_THREAD, "�߳�ID", LVCFMT_LEFT, 60);
    m_ctlListContent.InsertColumn(LIST_DEBUG_CONTENT, "����", LVCFMT_LEFT, 180);
    m_ctlListContent.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    HMODULE hInst = LoadLibrary("kernel32.dll");

    if (hInst == NULL) {
        MessageBox("ϵͳ����kernel32.dll���ļ������ڡ�");
        return;
    }

    //��λ����Ҫ���õĺ���λ�á�
    OpenThread = (OPENTHREAD)GetProcAddress(hInst, "OpenThread");

    if (OpenThread == NULL) {
        MessageBox("��ǰ����ϵͳ��ó��򲻼��ݡ�");
        return;
    }

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CReadMemoryDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    } else {
        CDialog::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CReadMemoryDlg::OnPaint()
{
    if (IsIconic()) {
        CPaintDC dc(this); // device context for painting
        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    } else {
        CDialog::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CReadMemoryDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CReadMemoryDlg::OnOK()
{
    // TODO: Add extra validation here
    //CDialog::OnOK();
}

void CReadMemoryDlg::OnCancel()
{
    // TODO: Add extra cleanup here
    if (MessageBox("ȷʵҪ�˳���?", "�˳�",
                   MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2) == IDOK) {
        CDialog::OnCancel();
    }
}


void CReadMemoryDlg::OnBUTTONOpen()
{
    //�򿪳���
    ShellExecute(NULL, "open", ".\\WindowsControl.exe", NULL, NULL, SW_SHOW);
}

void CReadMemoryDlg::OnBUTTONDebugStart()
{
    //���������̡߳�
    g_threadParam.bRunning = TRUE;
    g_threadParam.pDlg = this;
    AfxBeginThread(DebugThread, &g_threadParam, THREAD_PRIORITY_NORMAL);
    Sleep(100);
}

UINT DebugThread(LPVOID pParam)
{
    //�����̡߳�
    THREADPARAM* threadparam = (THREADPARAM*)pParam;
    CReadMemoryDlg *ptr = threadparam->pDlg;
    CListCtrl *pList = &ptr->m_ctlListContent;
    CString strTemp;
    //�ҵ�Ҫ���Գ���
    CString szCaption = "Windows��׼�ؼ�ʾ�����"; //�������
    CWnd *pWnd = ptr->FindWindow(NULL, szCaption);

    if (pWnd == NULL || pWnd->GetSafeHwnd() == NULL) {
        AfxMessageBox("���Ӵ���ʧ��!");
        return 1;
    }

    //��ý���ID��������DebugActiveProcess�򿪸��̡߳�
    DWORD dwID;//����ID
    HWND hHWND = pWnd->GetSafeHwnd();
    GetWindowThreadProcessId(hHWND, &dwID);
    DebugActiveProcess(dwID);
    //���ڵ��Գ����У����ھ����������IDתΪ�����
    HANDLE hProcess =::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwID);

    if (hProcess == NULL) {
        strTemp.Format("OpenProcessʧ�ܣ�GetLastError=%d\n", GetLastError());
        AfxMessageBox(strTemp);
        return 2;
    }

    //�������״̬��
    pList->DeleteAllItems();
    int iListIndex = 0;
    BOOL bRet = TRUE;
    DEBUG_EVENT DebugEv;

    while (g_threadParam.bRunning) {
        //�����¼������¼����ת��
        bRet = WaitForDebugEvent(&DebugEv, 200);

        if (!bRet) {
            Sleep(200);
            continue;
        }

        //���¼�����ʼ�¼�ʶ��
        pList->InsertItem(iListIndex, "");
        strTemp.Format("%d", DebugEv.dwProcessId);
        pList->SetItemText(iListIndex, LIST_DEBUG_PROCESS, strTemp);
        strTemp.Format("%d", DebugEv.dwThreadId);
        pList->SetItemText(iListIndex, LIST_DEBUG_THREAD, strTemp);

        //���ݲ�ͬ�¼����ֱ�������쳣��
        switch (DebugEv.dwDebugEventCode) {
        case EXCEPTION_DEBUG_EVENT: { //������쳣�¼���
            PEXCEPTION_RECORD ptr = &DebugEv.u.Exception.ExceptionRecord;
            strTemp.Format("EXCEPTION_DEBUG_EVENT");
            pList->SetItemText(iListIndex, LIST_DEBUG_TYPE, strTemp);
            strTemp.Format("�쳣�룺%d", (UINT)ptr->ExceptionCode);
            pList->SetItemText(iListIndex, LIST_DEBUG_CONTENT, strTemp);
        }
        break;

        case CREATE_THREAD_DEBUG_EVENT: { //�̴߳������¼���
            //����GetThreadContext������SetThreadContext�������������Ĳ�����
            //�����SuspendThread������ResumeThread��������ͻָ��߳����С�
            LPCREATE_THREAD_DEBUG_INFO ptr = &DebugEv.u.CreateThread;
            strTemp.Format("CREATE_THREAD_DEBUG_EVENT");
            pList->SetItemText(iListIndex, LIST_DEBUG_TYPE, strTemp);
            strTemp.Format("��ʼ��ַ��%d������ַ��%d���߳�ID:%d", ptr->lpStartAddress, ptr->lpThreadLocalBase, ptr->hThread);
            pList->SetItemText(iListIndex, LIST_DEBUG_CONTENT, strTemp);
        }
        break;

        case CREATE_PROCESS_DEBUG_EVENT: //���̴������¼���
            //������CREATE_THREAD_DEBUG_EVENT�¼��е�����4������֮�⣬
            //���ɵ���ReadProcessMemory������WriteProcessMemory��������
            //�����ڴ�Ķ�ȡ��д�롣
        {
            LPCREATE_PROCESS_DEBUG_INFO ptr = &DebugEv.u.CreateProcessInfo;
            strTemp.Format("CREATE_PROCESS_DEBUG_EVENT");
            pList->SetItemText(iListIndex, LIST_DEBUG_TYPE, strTemp);
            strTemp.Format("��ַ��%d", ptr->lpStartAddress);
            pList->SetItemText(iListIndex, LIST_DEBUG_CONTENT, strTemp);
            //CREATE_PROCESS_DEBUG_INFO pi;
            /*char buff[1024]={0,1,2,3,4,5,6,7,8,9,10};
            ULONG len=0;
            //CREATE_PROCESS_DEBUG_INFO pi;
            bRet=ReadProcessMemory(hProcess,(char *)DebugEv.u.CreateProcessInfo.lpImageName,(char *)buff,4,&len);
            if (!bRet || len<=0)
            {
                strTemp.Format("%d %d %d",bRet,len,GetLastError());
                //AfxMessageBox(strTemp);
            }
            else
            {
                CString strAll;
                //strTemp.Format("%d",len);
                //MessageBox(strTemp);

                CFile file;
                file.Open("a.txt",CFile::modeCreate|CFile::modeReadWrite,NULL);
                for (ULONG i=0;i<len;i++)
                {
                    strTemp.Format("%02X ",(UCHAR)buff[i]);
                    strAll+=strTemp;
                }
                //file.Write((char *)buff,len);
                file.Write(strAll,strAll.GetLength());
                file.Close();
                AfxMessageBox("ok");
            }*/
        }
        break;

        case EXIT_THREAD_DEBUG_EVENT: { //�߳��˳����¼���
            //��ʾ�̵߳��˳��롣
            LPEXIT_THREAD_DEBUG_INFO ptr = &DebugEv.u.ExitThread;
            strTemp.Format("EXIT_THREAD_DEBUG_EVENT");
            pList->SetItemText(iListIndex, LIST_DEBUG_TYPE, strTemp);
            strTemp.Format("�˳��룺%d", ptr->dwExitCode);
            pList->SetItemText(iListIndex, LIST_DEBUG_CONTENT, strTemp);
        }
        break;

        case EXIT_PROCESS_DEBUG_EVENT: { //�����˳����¼���
            //��ʾ���̵��˳��롣
            LPEXIT_PROCESS_DEBUG_INFO ptr = &DebugEv.u.ExitProcess;
            strTemp.Format("EXIT_PROCESS_DEBUG_EVENT");
            pList->SetItemText(iListIndex, LIST_DEBUG_TYPE, strTemp);
            strTemp.Format("�˳��룺%d", ptr->dwExitCode);
            pList->SetItemText(iListIndex, LIST_DEBUG_CONTENT, strTemp);
        }
        break;

        case LOAD_DLL_DEBUG_EVENT: { //��̬���ӿ������¼���
            //��ȡDLL�ļ���������Ϣ��
            LPLOAD_DLL_DEBUG_INFO ptr = &DebugEv.u.LoadDll;
            strTemp.Format("LOAD_DLL_DEBUG_EVENT");
            pList->SetItemText(iListIndex, LIST_DEBUG_TYPE, strTemp);
            strTemp.Format("��ַ:%d,���ƣ�%s", ptr->lpBaseOfDll, ptr->lpImageName);
            /*HANDLE hThread=::OpenThread(THREAD_ALL_ACCESS, FALSE, DebugEv.dwThreadId);
            SuspendThread(hThread);

            char pImageNameBase[MAX_PATH];
            char LoadDllName[MAX_PATH];
            ReadProcessMemory(hProcess,ptr->lpImageName,pImageNameBase,4,NULL);
            ReadProcessMemory(hProcess,(PVOID)pImageNameBase,LoadDllName,MAX_PATH,NULL);
            strTemp.Format("%s %s",pImageNameBase,LoadDllName);*/
            pList->SetItemText(iListIndex, LIST_DEBUG_CONTENT, strTemp);
        }
        break;

        case UNLOAD_DLL_DEBUG_EVENT: { //��̬���ӿ�ж�ص��¼���
            //��ʾж��DLL�ļ�����Ϣ��
            LPUNLOAD_DLL_DEBUG_INFO ptr = &DebugEv.u.UnloadDll;
            strTemp.Format("UNLOAD_DLL_DEBUG_EVENT");
            pList->SetItemText(iListIndex, LIST_DEBUG_TYPE, strTemp);
            strTemp.Format("��ַ:%d", (UINT)ptr->lpBaseOfDll);
            pList->SetItemText(iListIndex, LIST_DEBUG_CONTENT, strTemp);
        }
        break;

        case OUTPUT_DEBUG_STRING_EVENT: { //��ʾ������Ϣ���¼���
            //��ʾ����ĵ�����Ϣ����
            LPOUTPUT_DEBUG_STRING_INFO ptr = &DebugEv.u.DebugString;
            strTemp.Format("OUTPUT_DEBUG_STRING_EVENT");
            pList->SetItemText(iListIndex, LIST_DEBUG_TYPE, strTemp);
            strTemp.Format("%s", ptr->lpDebugStringData);
            pList->SetItemText(iListIndex, LIST_DEBUG_CONTENT, strTemp);
        }
        break;

        default: { //δ������Ϣ�Ĵ���
            strTemp.Format("δ����");
            pList->SetItemText(iListIndex, LIST_DEBUG_TYPE, strTemp);
            strTemp.Format("δ������Ϣ�Ĵ���");
            pList->SetItemText(iListIndex, LIST_DEBUG_CONTENT, strTemp);
        }
        break;
        }

        //�ӵ����лָ���
        ContinueDebugEvent(DebugEv.dwProcessId, DebugEv.dwThreadId, DBG_CONTINUE);
        iListIndex++;
    }//whileѭ������

    CloseHandle(hProcess);
    strTemp.Format("�߳��˳���");
    AfxMessageBox(strTemp);
    return 0;
}

void CReadMemoryDlg::OnBUTTONDebugStop()
{
    //�ر��߳�
    g_threadParam.bRunning = FALSE;
}
