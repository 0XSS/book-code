// CWebDeviceMonitorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CWebDeviceMonitor.h"
#include "CWebDeviceMonitorDlg.h"

#include <afxinet.h>
#define MAXBUFFLEN  (1024*1024)
UCHAR g_ucBuff[MAXBUFFLEN];

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
// CCWebDeviceMonitorDlg dialog

CCWebDeviceMonitorDlg::CCWebDeviceMonitorDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CCWebDeviceMonitorDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CCWebDeviceMonitorDlg)
    m_strHTMLWhole = _T("");
    m_strHTMLMonitor1 = _T("");
    m_strHTMLMonitor2 = _T("");
    m_strHTMLMonitor3 = _T("");
    m_strComment = _T("");
    m_strURL = _T("");
    m_iReadFreq = 0;
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCWebDeviceMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCWebDeviceMonitorDlg)
    DDX_Text(pDX, IDC_EDIT_HTMLWhole, m_strHTMLWhole);
    DDX_Text(pDX, IDC_EDIT_HTMLMonitor1, m_strHTMLMonitor1);
    DDX_Text(pDX, IDC_EDIT_HTMLMonitor2, m_strHTMLMonitor2);
    DDX_Text(pDX, IDC_EDIT_HTMLMonitor3, m_strHTMLMonitor3);
    DDX_Text(pDX, IDC_EDIT_Comment, m_strComment);
    DDX_Text(pDX, IDC_EDIT_URL, m_strURL);
    DDX_Text(pDX, IDC_EDIT_Freq, m_iReadFreq);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCWebDeviceMonitorDlg, CDialog)
    //{{AFX_MSG_MAP(CCWebDeviceMonitorDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_Start, OnBUTTONStart)
    ON_BN_CLICKED(IDC_BUTTON_Stop, OnBUTTONStop)
    ON_WM_TIMER()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCWebDeviceMonitorDlg message handlers

BOOL CCWebDeviceMonitorDlg::OnInitDialog()
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
    srand((unsigned)time(NULL));
    m_strURL = "http://localhost/Monitor/Device.asp";
    m_iReadFreq = 5;
    UpdateData(FALSE);
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCWebDeviceMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCWebDeviceMonitorDlg::OnPaint()
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
HCURSOR CCWebDeviceMonitorDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CCWebDeviceMonitorDlg::OnOK()
{
    // TODO: Add extra validation here
    //CDialog::OnOK();
}

void CCWebDeviceMonitorDlg::OnCancel()
{
    // TODO: Add extra cleanup here
    if (MessageBox("ȷʵҪ�˳���?", "�˳�ϵͳ", MB_OKCANCEL | MB_DEFBUTTON2 | MB_ICONQUESTION) == IDOK) {
        CDialog::OnCancel();
    }
}


void CCWebDeviceMonitorDlg::OnBUTTONStart()
{
    //��ʼ����ҳ�����ѣ��趨һ����ʱ����
    UpdateData(TRUE);
    int iFreq = m_iReadFreq;

    if (iFreq < 1)
        iFreq = 5;

    SetTimer(1, iFreq * 1000, NULL);
    OnTimer(1);//��������һ�Ρ�
}

void CCWebDeviceMonitorDlg::OnBUTTONStop()
{
    //ֹͣ��ȡ��ֹͣ��ʱ����
    KillTimer(1);
}

void CCWebDeviceMonitorDlg::OnTimer(UINT nIDEvent)
{
    //��ʱ����
    switch (nIDEvent) {
    case 1: {
        ReadHTML();
    }
    break;

    default:
        break;
    }

    CDialog::OnTimer(nIDEvent);
}

void CCWebDeviceMonitorDlg::ReadHTML()
{
    //����һ���Ự���ɴ˻Ự����һ�����ӣ�����ͨ���ļ��ķ�ʽ���ж�д��
    UpdateData(TRUE);
    CString fileURL;
    fileURL.Format("WWW Monitor");
    CInternetSession iSession(fileURL);//�����ĻỰ
    CHttpFile *pFile = NULL; //���ڶ�д���ļ���
    //char buff[1000];//��ȡ��������

    try {
        //ѡ����Է�û���ṩWWW����
        if (m_strURL.Find("?") > 0) //����URL
            fileURL.Format("%s&param%d=%d", m_strURL, rand(), rand());
        else
            fileURL.Format("%s?id=%d", m_strURL, rand());

        pFile = (CHttpFile *)iSession.OpenURL(fileURL); //���ļ�

        if (pFile == NULL) {
            m_strHTMLWhole = "";
            m_strHTMLMonitor1 = "";
            m_strHTMLMonitor2 = "";
            m_strHTMLMonitor3 = "";
            m_strComment = "��ȡ������ȷ���豸��ַ�������Ƿ���ȷ��";
        } else {
            int iPos = 0;
            BOOL bGoon = TRUE;

            while (bGoon) {
                //AfxMessageBox(fileURL);
                int len = pFile->Read((char *)&g_ucBuff[iPos], 1000);

                if (len <= 0) {
                    bGoon = FALSE;
                    break;
                }

                iPos += len;
            }

            pFile->Close();//�ر��ļ������
            g_ucBuff[iPos] = '\0';
            m_strHTMLWhole.Format("%s", g_ucBuff);
            m_strHTMLMonitor1 = GetValue(m_strHTMLWhole, "�豸����", "<b>", "</b>");
            m_strHTMLMonitor2 = GetValue(m_strHTMLWhole, "��ǰʱ��", "#FFFFFF>", "</font>");
            m_strHTMLMonitor3 = GetValue(m_strHTMLWhole, "��ǰֵ", "<b>", "</b>");
            m_strComment = "��ȡ��ȷ��";
        }

        iSession.Close();//�رջỰ��
    }
    //�����д�г��ֵ��쳣��
    catch (CInternetException* ex) {
        CString strTemp;
        strTemp.Format("��д����������ɨ�衣", ex->m_dwError);
    }

    UpdateData(FALSE);
}

CString CCWebDeviceMonitorDlg::GetValue(CString strWhole, CString strLabel, CString strHead, CString strTail)
{
    //���ݹؼ��֣������ݷֽ������ȡ��
    CString strRet = "";
    int iPos0 = strWhole.Find(strLabel);

    if (iPos0 <= 0)
        return strRet;

    int iPos1 = strWhole.Find(strHead, iPos0);

    if (iPos1 <= 0)
        return strRet;

    iPos1 += strHead.GetLength();
    int iPos2 = strWhole.Find(strTail, iPos1);

    if (iPos2 <= 0)
        return strRet;

    strRet = strWhole.Mid(iPos1, iPos2 - iPos1);
    return strRet;
}