// NewPingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewPing.h"
#include "NewPingDlg.h"

//�����ļ�
#define INIFILENAME ".\\NewPing.ini"

//����Ҫ��ͷ�ļ��;�̬���ļ�
#include "IPHlpApi.h"
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment (lib, "IPHlpApi.Lib")

#define LIST_ECHO_TTL   0
#define LIST_ECHO_IP    1
#define LIST_ECHO_MS    2

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

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
// CNewPingDlg dialog

CNewPingDlg::CNewPingDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CNewPingDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CNewPingDlg)
    m_strDestIP = _T("");
    m_strComment = _T("");
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNewPingDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CNewPingDlg)
    DDX_Control(pDX, IDC_LIST_Echo, m_ctlListEcho);
    DDX_Text(pDX, IDC_EDIT_DestIP, m_strDestIP);
    DDX_Text(pDX, IDC_EDIT_Comment, m_strComment);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNewPingDlg, CDialog)
    //{{AFX_MSG_MAP(CNewPingDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTONIcmpDll, OnBUTTONIcmpDll)
    ON_BN_CLICKED(IDC_BUTTONClear, OnBUTTONClear)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewPingDlg message handlers

BOOL CNewPingDlg::OnInitDialog()
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
    m_ctlListEcho.InsertColumn(LIST_ECHO_TTL, "TTL", LVCFMT_LEFT, 80);
    m_ctlListEcho.InsertColumn(LIST_ECHO_IP, "IP��ַ", LVCFMT_LEFT, 100);
    m_ctlListEcho.InsertColumn(LIST_ECHO_MS, "��ʱ", LVCFMT_LEFT, 100);
    m_ctlListEcho.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    char buff[MAX_PATH];
    GetPrivateProfileString("Init", "IP", "127.0.0.1", buff, MAX_PATH, INIFILENAME);
    m_strDestIP.Format("%s", buff);
    UpdateData(FALSE);
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNewPingDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CNewPingDlg::OnPaint()
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
HCURSOR CNewPingDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CNewPingDlg::OnOK()
{
    // TODO: Add extra validation here
    //CDialog::OnOK();
}

void CNewPingDlg::OnCancel()
{
    //�����˳�
    if (MessageBox("ȷʵҪ�˳����Ƿ������", "������ʾ", MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2) == IDOK) {
        UpdateData(TRUE);
        WritePrivateProfileString("Init", "IP", m_strDestIP, INIFILENAME);
        CDialog::OnCancel();
    }
}

//������ICMP.dll��Ҫ���õ�����������
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
//���õĺ�������
ICMPCREATEFILE IcmpCreateFile;
ICMPCLOSEHANDLE IcmpCloseHandle;
ICMPSENDECHO IcmpSendEcho;

void CNewPingDlg::OnBUTTONIcmpDll()
{
    //ʵʩPING����
    UpdateData(TRUE);

    if (m_strDestIP.GetLength() < 7) {
        MessageBox("��Ŀ������IP��ַ����ʽ����ȷ");
        return;
    }

    //��ICMP.dll�ļ���
    ULONG ulDestIP = (ULONG)inet_addr(m_strDestIP);
    HMODULE hInst = LoadLibrary(".\\ICMP.dll");

    if (hInst == NULL) {
        MessageBox("���ļ������ڣ�������Ч��DLL�ļ���");
        return;
    }

    //��λ����Ҫ���õĺ���λ�á�
    IcmpCreateFile = (ICMPCREATEFILE)GetProcAddress(hInst, "IcmpCreateFile");
    IcmpCloseHandle = (ICMPCLOSEHANDLE)GetProcAddress(hInst, "IcmpCloseHandle");
    IcmpSendEcho = (ICMPSENDECHO)GetProcAddress(hInst, "IcmpSendEcho");

    if (IcmpCreateFile == NULL || IcmpCloseHandle == NULL ||
        IcmpSendEcho == NULL) {
        MessageBox("DLL�ļ��в���������Ҫ����������������ICMP2.dll�ļ���ʽ��汾�д�");
        return;
    }

    //��ICMP������
    HANDLE hIcmp = IcmpCreateFile();

    if (hIcmp == INVALID_HANDLE_VALUE) {
        MessageBox("��ICMP�������");
        FreeLibrary(hInst);
        return;
    }

    //��ʼ��һЩ������
    //���Ͳ������㡣
    IP_OPTION_INFORMATION IpOption;
    ZeroMemory(&IpOption, sizeof(IP_OPTION_INFORMATION));
    //���跢�͵����ݣ�Windows��ping����Ĭ�Ϸ��͵��ǡ�abcde..xyzabcdefghi����
    char *SendData = "The quick brown fox jumps over a lazy dog.";
    //���ջ��������Լ����ջ�������ָ�롣
    char ReplyBuff[500] = {0};
    PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuff;
    //Ŀ�������п��ܿ������п���û������
    //����жϵ�Ŀ�������ͷ���IPΪͬһ��������Ϊ�ǳɹ���
    //�жϵ���ʱ�����������Ϊû�гɹ���
    BOOL bLoop = TRUE; //�������Ȳ�֪���м��·�����ж��٣����Ե�����Ŀ�ĵ�ʱ�˳���
    CString strTemp, strEcho;

    while (bLoop) {
        //����TTLֵ��
        IpOption.Ttl++;
        strTemp.Format("%d", IpOption.Ttl);
        m_ctlListEcho.InsertItem(0, strTemp); //LIST_ECHO_TTL

        if (IcmpSendEcho(hIcmp, ulDestIP, SendData, sizeof(SendData), &IpOption, ReplyBuff, sizeof(ReplyBuff), 3000) != 0) {
            //��ʱ��
            if (pEchoReply->RoundTripTime == 0) {
                strTemp = "<1 ms";
            } else {
                strTemp.Format("%d ms", pEchoReply->RoundTripTime);
            }

            m_ctlListEcho.SetItemText(0, LIST_ECHO_MS, strTemp);

            if (pEchoReply->Address == ulDestIP) {
                m_strComment.Format("Trace�������ﵽ������Ŀ�ص�");
                bLoop = FALSE;
            }
        } else {
            m_strComment.Format("Trace��ʱ���˳���δ�ܴﵽ����Ŀ�ص�");
            m_ctlListEcho.SetItemText(0, LIST_ECHO_MS, "��ʱ");
            bLoop = FALSE;
        }

        struct in_addr addr;

        addr.S_un.S_addr = pEchoReply->Address;

        strTemp.Format("%s", inet_ntoa(addr));

        m_ctlListEcho.SetItemText(0, LIST_ECHO_IP, strTemp);

        Sleep(100);
    }

    //д����־�����ر�
    IcmpCloseHandle(hIcmp);
    FreeLibrary(hInst);
    UpdateData(FALSE);
}

void CNewPingDlg::OnBUTTONClear()
{
    // TODO: Add your control notification handler code here
    m_ctlListEcho.DeleteAllItems();
}
