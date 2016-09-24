// EmbedScreenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EmbedScreen.h"
#include "EmbedScreenDlg.h"

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
// CEmbedScreenDlg dialog

CEmbedScreenDlg::CEmbedScreenDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CEmbedScreenDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CEmbedScreenDlg)
    m_strProgramName = _T("");
    m_iOffsetLeft = 0;
    m_iOffsetTop = 0;
    m_iRatio = -1;
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEmbedScreenDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CEmbedScreenDlg)
    DDX_Text(pDX, IDC_EDIT_ProgramName, m_strProgramName);
    DDX_Text(pDX, IDC_EDIT_OffsetLeft, m_iOffsetLeft);
    DDX_Text(pDX, IDC_EDIT_OffsetTop, m_iOffsetTop);
    DDX_CBIndex(pDX, IDC_COMBO_Ratio, m_iRatio);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEmbedScreenDlg, CDialog)
    //{{AFX_MSG_MAP(CEmbedScreenDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_EmbedStart, OnBUTTONEmbedStart)
    ON_BN_CLICKED(IDC_BUTTON_EmbedStop, OnBUTTONEmbedStop)
    ON_WM_TIMER()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEmbedScreenDlg message handlers

BOOL CEmbedScreenDlg::OnInitDialog()
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
    //���Ȼ�á�Ƕ�����򡱵ķ�Χ��
    //���С�Ƕ�����򡱵�IDֵΪIDC_STATIC_Embed��
    GetDlgItem(IDC_STATIC_Embed)->GetWindowRect(m_rectEmbed);
    ClientToScreen(m_rectEmbed);
    //��Ƕ�����򡱿ؼ�������һ���������ʵ�ʻ�������ҪС��
    int iHeight = m_rectEmbed.Height() - 40;
    m_rectEmbed.left += 10;
    m_rectEmbed.top -= GetSystemMetrics(SM_CYCAPTION) + 10;
    m_rectEmbed.right -= 30;
    m_rectEmbed.bottom = m_rectEmbed.top + iHeight;
    //��������ƫ�������Լ��Ƿ����š�
    m_iOffsetLeft = 0;
    m_iOffsetTop = 0;
    m_iRatio = 1;
    //Ҫʵʱ��ȡ����ĳ������ƣ�����������ơ�
    m_strProgramName = "δ���� - ��ͼ";
    UpdateData(FALSE);
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CEmbedScreenDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CEmbedScreenDlg::OnPaint()
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
HCURSOR CEmbedScreenDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CEmbedScreenDlg::OnOK()
{
    // TODO: Add extra validation here
    //CDialog::OnOK();
}

void CEmbedScreenDlg::OnCancel()
{
    //�˳�����
    if (MessageBox("ȷʵҪ�˳���?", "�˳�ϵͳ", MB_OKCANCEL | MB_DEFBUTTON2 | MB_ICONQUESTION) == IDOK) {
        CDialog::OnCancel();
    }
}

void CEmbedScreenDlg::OnBUTTONEmbedStart()
{
    //����һ����ͣ��ȡ�Ķ�ʱ����֮��Ĳ����ɶ�ʱ����ɡ�
    SetTimer(1, 2000, NULL);
}

void CEmbedScreenDlg::OnBUTTONEmbedStop()
{
    //�ص���ʱ����
    KillTimer(1);
}

void CEmbedScreenDlg::OnTimer(UINT nIDEvent)
{
    //��ʱ������
    switch (nIDEvent) {
    case 1://��ʱ��������ʾ��
        CopyScreen();
        break;

    default:
        break;
    }

    CDialog::OnTimer(nIDEvent);
}

void CEmbedScreenDlg::CopyScreen()
{
    //��ʱ��ȡ�Է��Ľ��棬Ȼ����ʾ���Լ�ָ��λ�á�
    //�����ҵ�Ҫ����Ļ�����򣬸�������ܻ��ƶ�������Ҫ��̬�ҡ�
    UpdateData(TRUE);
    CWnd *pWnd = FindWindow(NULL, m_strProgramName);

    if (pWnd == NULL) {
        MessageBox("û�ҵ�Ҫ��ѯ�Ĵ��ڡ�");
        return;
    }

    //����Ҫ��ʾ�Ŀ�͸ߡ�
    //�������Ƕ����򡱵Ľ���ܴ�Ҳֻ����ʾ��Ƕ�����򡱵Ĵ�С��
    //�������Ƕ����򡱵Ľ����С����Ϊ��ˢ����ͼ����Ƕ������ҲҪ������
    CRect rect;
    pWnd->GetWindowRect(rect);//��ô��ڵ�����ͳ���
    int iWidth = rect.Width(); //����Ƕ����򡱵Ŀ�
    int iHeight = rect.Height(); //����Ƕ����򡱵ĸ�

    if (iWidth > m_rectEmbed.Width())
        iWidth = m_rectEmbed.Width();

    if (iHeight > m_rectEmbed.Height())
        iHeight = m_rectEmbed.Height();

    //������Ļ��
    //һ���ǡ���Ƕ����򡱵��豸ָ�룬һ���ǵ�ǰ�豸ָ��
    CWindowDC dcWindow(pWnd);
    CClientDC dcClient(this);

    switch (m_iRatio) {
    case 0: { //���50%��ʾЧ������ͼ����СЧ����
        for (int y = 0; y < iHeight; y++) {
            for (int x = 0; x < iWidth; x++) {
                //�ӡ���Ƕ�����������ȡ2X2���ĸ��㣬
                //Ȼ������RGB��ƽ��ֵ��Ȼ����ʾ��
                COLORREF colorTemp1, colorTemp2, colorTemp3, colorTemp4;
                COLORREF colorTemp;
                colorTemp1 = dcWindow.GetPixel(x * 2 + m_iOffsetLeft, y * 2 + m_iOffsetTop);
                colorTemp2 = dcWindow.GetPixel(x * 2 + 1 + m_iOffsetLeft, y * 2 + m_iOffsetTop);
                colorTemp3 = dcWindow.GetPixel(x * 2 + m_iOffsetLeft, y * 2 + 1 + m_iOffsetTop);
                colorTemp4 = dcWindow.GetPixel(x * 2 + 1 + m_iOffsetLeft, y * 2 + 1 + m_iOffsetTop);
                int r = ((int)GetRValue(colorTemp1) + (int)GetRValue(colorTemp2) +
                         (int)GetRValue(colorTemp3) + (int)GetRValue(colorTemp4)) / 4;
                int g = ((int)GetGValue(colorTemp1) + (int)GetGValue(colorTemp2) +
                         (int)GetGValue(colorTemp3) + (int)GetGValue(colorTemp4)) / 4;
                int b = ((int)GetBValue(colorTemp1) + (int)GetBValue(colorTemp2) +
                         (int)GetBValue(colorTemp3) + (int)GetBValue(colorTemp4)) / 4;
                colorTemp = RGB(r, g, b);
                int xx = m_rectEmbed.left + x;
                int yy = m_rectEmbed.top + y;
                dcClient.SetPixel(xx, yy, colorTemp);
            }
        }
    }
    break;

    case 1: { //�����100%������ζ�Ų�����
        for (int y = 0; y < iHeight; y++) {
            for (int x = 0; x < iWidth; x++) {
                //�����ţ���ֱ����ʾ���ɡ�
                COLORREF colorTemp;
                colorTemp = dcWindow.GetPixel(x + m_iOffsetLeft, y + m_iOffsetTop);
                int xx = m_rectEmbed.left + x;
                int yy = m_rectEmbed.top + y;
                dcClient.SetPixel(xx, yy, colorTemp);
            }
        }
    }
    break;

    case 2: { //���200%�����Ŵ�
        for (int y = 0; y < iHeight / 2; y++) {
            for (int x = 0; x < iWidth / 2; x++) {
                //����2���Ŵ���ֱ�ӽ�һ������ʾΪ2X2��4���㼴�ɡ�
                COLORREF colorTemp;
                colorTemp = dcWindow.GetPixel(x + m_iOffsetLeft, y + m_iOffsetTop);
                dcClient.SetPixel(m_rectEmbed.left + x * 2, m_rectEmbed.top + y * 2, colorTemp);
                dcClient.SetPixel(m_rectEmbed.left + x * 2 + 1, m_rectEmbed.top + y * 2, colorTemp);
                dcClient.SetPixel(m_rectEmbed.left + x * 2, m_rectEmbed.top + y * 2 + 1, colorTemp);
                dcClient.SetPixel(m_rectEmbed.left + x * 2 + 1, m_rectEmbed.top + y * 2 + 1, colorTemp);
            }
        }
    }
    break;

    default:
        break;
    }
}
