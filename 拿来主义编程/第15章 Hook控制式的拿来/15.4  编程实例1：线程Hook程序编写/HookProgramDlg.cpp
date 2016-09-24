// HookProgramDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HookProgram.h"
#include "HookProgramDlg.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

HHOOK g_hMouseHook = NULL;
//HHOOK g_hKeyBoardHook=NULL;
LRESULT CALLBACK HookMouseProc(int nCode, WPARAM wParam, LPARAM lParam);
CHookProgramDlg *g_pDlg;

/*#define LIST_SAVE_NAME        0
#define LIST_SAVE_HANDLE    1
#define LIST_SAVE_X         2
#define LIST_SAVE_Y         3
#define LIST_SAVE_MESSAGE   4
#define LIST_SAVE_TYPE      5*/

//#include "HookDll.h"
//#pragma comment(lib,"HookDll.lib")   //��ʽ����DLL
typedef void(WINAPI *STARTHOOK)(CListBox *pList);
typedef void(WINAPI *STOPHOOK)();
//typedef LRESULT (WINAPI CALLBACK *HOOKKEYBOARDPROC)(int nCode,WPARAM wParam,LPARAM lParam);
STARTHOOK StartHook;
STOPHOOK StopHook;
//HOOKKEYBOARDPROC HookKeyBoardProc;


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
// CHookProgramDlg dialog

CHookProgramDlg::CHookProgramDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CHookProgramDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CHookProgramDlg)
    m_strComment = _T("");
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHookProgramDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CHookProgramDlg)
    DDX_Control(pDX, IDC_LIST_Save, m_ctlListSave);
    DDX_Text(pDX, IDC_EDIT_Comment, m_strComment);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CHookProgramDlg, CDialog)
    //{{AFX_MSG_MAP(CHookProgramDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_InstallThreadHook, OnBUTTONInstallThreadHook)
    ON_BN_CLICKED(IDC_BUTTON_UninstallThreadHook, OnBUTTONUninstallThreadHook)
    ON_BN_CLICKED(IDC_BUTTON_InstallSystemHook, OnBUTTONInstallSystemHook)
    ON_BN_CLICKED(IDC_BUTTON_UninstallSystemHook, OnBUTTONUninstallSystemHook)
    ON_BN_CLICKED(IDC_BUTTON_Clear, OnBUTTONClear)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHookProgramDlg message handlers

BOOL CHookProgramDlg::OnInitDialog()
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
    g_hMouseHook = NULL;
    //g_hKeyBoardHook=NULL;
    g_pDlg = this;
    /*m_ctlListSave.InsertColumn(LIST_SAVE_NAME,"��������",LVCFMT_LEFT,80);
    m_ctlListSave.InsertColumn(LIST_SAVE_HANDLE,"���",LVCFMT_LEFT,80);
    m_ctlListSave.InsertColumn(LIST_SAVE_X,"����X",LVCFMT_LEFT,80);
    m_ctlListSave.InsertColumn(LIST_SAVE_Y,"����Y",LVCFMT_LEFT,80);
    m_ctlListSave.InsertColumn(LIST_SAVE_MESSAGE,"��Ϣ����",LVCFMT_LEFT,80);
    m_ctlListSave.InsertColumn(LIST_SAVE_TYPE,"����",LVCFMT_LEFT,80);
    m_ctlListSave.SetExtendedStyle(LVS_EX_FULLROWSELECT| LVS_EX_GRIDLINES);*/
    HMODULE glhInstance = LoadLibrary(".\\HookDll.dll");

    if (glhInstance == NULL) {
        MessageBox("���ļ������ڣ�������Ч��DLL�ļ���");
        return FALSE;
    }

    //��λ����Ҫ���õĺ���λ�á�
    StartHook = (STARTHOOK)GetProcAddress(glhInstance, "StartHook");
    StopHook = (STOPHOOK)GetProcAddress(glhInstance, "StopHook");

    //HookKeyBoardProc=(HOOKKEYBOARDPROC)GetProcAddress(glhInstance,"HookKeyBoardProc");
    if (StartHook == NULL || StopHook == NULL) { // || HookKeyBoardProc==NULL)
        MessageBox("DLL�ļ��в���������Ҫ����������������HookDll.dll�ļ���ʽ��汾�д�");
        return FALSE;
    }

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHookProgramDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CHookProgramDlg::OnPaint()
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
HCURSOR CHookProgramDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CHookProgramDlg::OnOK()
{
    // TODO: Add extra validation here
    //CDialog::OnOK();
}

void CHookProgramDlg::OnCancel()
{
    //�˳�����
    if (MessageBox("ȷʵҪ�˳���?", "�˳�ϵͳ", MB_OKCANCEL | MB_DEFBUTTON2 | MB_ICONQUESTION) == IDOK) {
        CDialog::OnCancel();
    }
}

LRESULT CALLBACK HookMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    LPMOUSEHOOKSTRUCT pMouseEvent = (MOUSEHOOKSTRUCT FAR *)lParam;

    if (nCode >= 0) {
        CString strTemp, strAll;
        char buff[MAX_PATH] = {0};
        GetWindowText(pMouseEvent->hwnd, buff, MAX_PATH);
        strTemp.Format("�������ƣ�%s ", buff);
        strAll += strTemp;
        //strTemp.Format("���ھ����%d",pMouseEvent->hwnd);
        //strAll+=strTemp;
        strTemp.Format("����[%d,%d] ", pMouseEvent->pt.x, pMouseEvent->pt.y);
        strAll += strTemp;

        switch (wParam) {
        case WM_MOUSEMOVE:
            strTemp = "WM_MOUSEMOVE";
            break;

        case WM_LBUTTONDOWN:
            strTemp = "WM_LBUTTONDOWN";
            break;

        case WM_LBUTTONUP:
            strTemp = "WM_LBUTTONUP";
            break;

        case WM_LBUTTONDBLCLK:
            strTemp = "WM_LBUTTONDBLCLK";
            break;

        case WM_RBUTTONDOWN:
            strTemp = "WM_RBUTTONDOWN";
            break;

        case WM_RBUTTONUP:
            strTemp = "WM_RBUTTONUP";
            break;

        case WM_RBUTTONDBLCLK:
            strTemp = "WM_RBUTTONDBLCLK";
            break;

        case WM_MBUTTONDOWN:
            strTemp = "WM_MBUTTONDOWN";
            break;

        case WM_MBUTTONUP:
            strTemp = "WM_MBUTTONUP";
            break;

        case WM_MBUTTONDBLCLK:
            strTemp = "WM_MBUTTONDBLCLK";
            break;

        case WM_MOUSEWHEEL:
            strTemp = "WM_MOUSEWHEEL";
            break;

        default:
            strTemp = "δ���塣";
            break;
        }

        strAll += strTemp;
        strAll += nCode == HC_NOREMOVE ? "����" : "�Ƴ�";
        g_pDlg->m_ctlListSave.InsertString(0, strAll);
        /*CListCtrl *pList=(CListCtrl *)&g_pDlg->m_ctlListSave;
        char buff[MAX_PATH]={0};
        GetWindowText(pMouseEvent->hwnd,buff,MAX_PATH);
        pList->InsertItem(0,buff);//LIST_SAVE_NAME
        strTemp.Format("%d",pMouseEvent->hwnd);
        pList->SetItemText(0,LIST_SAVE_HANDLE,strTemp);
        strTemp.Format("%d",pMouseEvent->pt.x);
        pList->SetItemText(0,LIST_SAVE_X,strTemp);
        strTemp.Format("%d",pMouseEvent->pt.y);
        pList->SetItemText(0,LIST_SAVE_Y,strTemp);
        switch(wParam)
        {
        case WM_MOUSEMOVE:strTemp="WM_MOUSEMOVE";break;
        case WM_LBUTTONDOWN:strTemp="WM_LBUTTONDOWN";break;
        case WM_LBUTTONUP:strTemp="WM_LBUTTONUP";break;
        case WM_LBUTTONDBLCLK:strTemp="WM_LBUTTONDBLCLK";break;
        case WM_RBUTTONDOWN:strTemp="WM_RBUTTONDOWN";break;
        case WM_RBUTTONUP:strTemp="WM_RBUTTONUP";break;
        case WM_RBUTTONDBLCLK:strTemp="WM_RBUTTONDBLCLK";break;
        case WM_MBUTTONDOWN:strTemp="WM_MBUTTONDOWN";break;
        case WM_MBUTTONUP:strTemp="WM_MBUTTONUP";break;
        case WM_MBUTTONDBLCLK:strTemp="WM_MBUTTONDBLCLK";break;
        case WM_MOUSEWHEEL:strTemp="WM_MOUSEWHEEL";break;
        default:strTemp="δ���塣";break;
        }
        pList->SetItemText(0,LIST_SAVE_MESSAGE,strTemp);
        pList->SetItemText(0,LIST_SAVE_TYPE,nCode==HC_NOREMOVE?"����":"�Ƴ�");*/
    }

    return CallNextHookEx(g_hMouseHook, nCode, wParam, lParam);
}

void CHookProgramDlg::OnBUTTONInstallThreadHook()
{
    //��װ�߳����Hook
    UpdateData(TRUE);

    if (g_hMouseHook != NULL) {
        m_strComment = "֮ǰ�Ѱ�װ���߳����Hook��";
    } else {
        g_hMouseHook = SetWindowsHookEx(WH_MOUSE, HookMouseProc, NULL, GetCurrentThreadId());

        if (g_hMouseHook == NULL) {
            m_strComment.Format("��װʧ�ܡ�������Ϊ��%d��", GetLastError());
        } else {
            m_strComment.Format("��װ�ɹ���");
        }
    }

    UpdateData(FALSE);
}

void CHookProgramDlg::OnBUTTONUninstallThreadHook()
{
    //ж��֮ǰ��װ���߳����Hook��
    UpdateData(TRUE);

    if (g_hMouseHook == NULL) {
        m_strComment = "֮ǰδ��װ���߳����Hook��";
    } else {
        BOOL bRet = UnhookWindowsHookEx(g_hMouseHook);

        if (bRet) {
            g_hMouseHook = NULL;
            m_strComment = "ж�سɹ���";
        } else {
            m_strComment = "ж��ʧ�ܡ�";
        }
    }

    UpdateData(FALSE);
}

void CHookProgramDlg::OnBUTTONInstallSystemHook()
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    StartHook(&m_ctlListSave);
    m_strComment.Format("��װ�ɹ���");
    UpdateData(FALSE);
}

void CHookProgramDlg::OnBUTTONUninstallSystemHook()
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    StopHook();
    m_strComment.Format("ж�سɹ���");
    UpdateData(FALSE);
}

/*
LRESULT CALLBACK HookKeyBoardProc(int nCode,WPARAM wParam,LPARAM lParam)
{
    if ( nCode >= 0 )
    {
        CString strTemp,strAll;
        strTemp.Format("������%c[%d] ",wParam,wParam);
        strAll+=strTemp;
        strTemp.Format("���ͣ�%s ",nCode==HC_NOREMOVE?"����":"�Ƴ�");
        strAll+=strTemp;
        //0-15λ�����������¼��̴�����
        //16-23λ:ָ��ɨ����. ������OEM
        //24λ:��24λΪ1ʱ�򣺱�ʾ��������չ��;��24λΪ0ʱ�򣺱�ʾ���������ּ��̰���
        //25-28λ:����λ
        //29λ:�����ļ���Ϊ1ʱ�� ALT���£��������Ϊ0
        //30λ:����ǰ������º��͵���Ϣ��30λΪ0������ǰ���̧���30λΪ1��
        //31λ:ָ��ת��״̬;31λΪ0ʱ�򣬰������ڱ����£�Ϊ1ʱ�򣬰������ڱ��ͷ�
        strTemp.Format("����������%d",(UINT)lParam&0xFFFF);
        strAll+=strTemp;
        strTemp.Format("ɨ���룺%d",(UINT)(lParam&0x0FF0000)/0x10000);
        strAll+=strTemp;
        strTemp.Format("�����ͣ�%s",(lParam&0x1000000)==0?"���ּ���":"��չ��");
        strAll+=strTemp;
        strTemp.Format("ALT��%s",(lParam&0x20000000)==0?"δ��":"����");
        strAll+=strTemp;
        strTemp.Format("��ǰ״̬��%s",(lParam&0x40000000)==0?"δ̧��":"��̧��");
        strAll+=strTemp;
        strTemp.Format("ת��״̬��%s",(lParam&0x80000000)==0?"��������":"�����ͷ�");
        strAll+=strTemp;
        g_pDlg->m_ctlListSave.InsertString(0,strAll);
    }
    return CallNextHookEx(g_hKeyBoardHook,nCode, wParam, lParam);
}
*/
void CHookProgramDlg::OnBUTTONClear()
{
    // TODO: Add your control notification handler code here
    m_ctlListSave.ResetContent();
}
