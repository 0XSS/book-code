// MemoryAccessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MemoryAccess.h"
#include "MemoryAccessDlg.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

//����Ҫʹ�õĽṹ
struct tagPhysStruct {
    DWORD64 dwPhysMemSizeInBytes;
    DWORD64 pvPhysAddress;
    DWORD64 PhysicalMemoryHandle;
    DWORD64 pvPhysMemLin;
    DWORD64 pvPhysSection;
};
struct tagPortStruct {
    USHORT wPortAddr;
    ULONG dwPortVal;
    UCHAR bSize;
};

//��������������Ҫ�ĺ�����
typedef int(WINAPI *INITIALIZEWINIO)();
typedef int(WINAPI *SHUTDOWNWINIO)();
typedef int(WINAPI *MAPPHYSTOLIN)(tagPhysStruct &PhysStruct);
typedef int(WINAPI *UNMAPPHYSICALMEMORY)(tagPhysStruct &PhysStruct);
typedef int(WINAPI *GETPHYSLONG)(PBYTE pbPhysAddr, PDWORD pdwPhysVal);
typedef int(WINAPI *SETPHYSLONG)(PBYTE pbPhysAddr, DWORD dwPhysVal);
typedef int(WINAPI *GETPORTVAL)(WORD wPortAddr, PDWORD pdwPortVal, BYTE bSize);
typedef int(WINAPI *SETPORTVAL)(WORD wPortAddr, DWORD dwPortVal, BYTE bSize);
typedef int(WINAPI *INSTALLWINIODRIVER)(PWSTR pszWinIoDriverPath, bool IsDemandLoaded = false);
typedef int(WINAPI *REMOVEWINIODRIVER)();

//���庯�����ĺ궨�塣
INITIALIZEWINIO     InitializeWinIo;
SHUTDOWNWINIO       ShutdownWinIo;
MAPPHYSTOLIN        MapPhysToLin;
UNMAPPHYSICALMEMORY UnmapPhysicalMemory;
GETPHYSLONG         GetPhysLong;
SETPHYSLONG         SetPhysLong;
GETPORTVAL          GetPortVal;
SETPORTVAL          SetPortVal;
INSTALLWINIODRIVER  InstallWinIoDriver;
REMOVEWINIODRIVER   RemoveWinIoDriver;

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
// CMemoryAccessDlg dialog

CMemoryAccessDlg::CMemoryAccessDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CMemoryAccessDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CMemoryAccessDlg)
    m_strDateTime = _T("");
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMemoryAccessDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMemoryAccessDlg)
    DDX_Text(pDX, IDC_EDIT_DateTime, m_strDateTime);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMemoryAccessDlg, CDialog)
    //{{AFX_MSG_MAP(CMemoryAccessDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_Read, OnBUTTONRead)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMemoryAccessDlg message handlers

BOOL CMemoryAccessDlg::OnInitDialog()
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
    //��ȡWinIO��������
    //������DLL�ļ���
    HINSTANCE hInst;
    hInst = LoadLibrary(".\\WinIo32.dll");

    if (!hInst) {
        MessageBox("�޷���λ��̬���ӿ�WinIo32.dll���뽫���ļ����Ƶ���ǰĿ¼��\
			��ϵͳSystem32Ŀ¼�¡�", "ϵͳ������ʾ", MB_ICONEXCLAMATION);
        return FALSE;
    }

    //�ҵ����и������ĵ�ַ��
    InitializeWinIo = (INITIALIZEWINIO)GetProcAddress(hInst, "InitializeWinIo");
    ShutdownWinIo = (SHUTDOWNWINIO)GetProcAddress(hInst, "ShutdownWinIo");
    MapPhysToLin = (MAPPHYSTOLIN)GetProcAddress(hInst, "MapPhysToLin");
    UnmapPhysicalMemory = (UNMAPPHYSICALMEMORY)GetProcAddress(hInst, "UnmapPhysicalMemory");
    GetPhysLong = (GETPHYSLONG)GetProcAddress(hInst, "GetPhysLong");
    SetPhysLong = (SETPHYSLONG)GetProcAddress(hInst, "SetPhysLong");
    GetPortVal = (GETPORTVAL)GetProcAddress(hInst, "GetPortVal");
    SetPortVal = (SETPORTVAL)GetProcAddress(hInst, "SetPortVal");
    InstallWinIoDriver = (INSTALLWINIODRIVER)GetProcAddress(hInst, "InstallWinIoDriver");
    RemoveWinIoDriver = (REMOVEWINIODRIVER)GetProcAddress(hInst, "RemoveWinIoDriver");

    //ȷ�ϸ���ַ���Ǵ��ڵģ����Ǵ���DLL�汾�����һ�����÷�ʽ
    if (InitializeWinIo == NULL || ShutdownWinIo == NULL ||
        MapPhysToLin == NULL || UnmapPhysicalMemory == NULL ||
        SetPhysLong == NULL || GetPhysLong == NULL ||
        GetPortVal == NULL || SetPortVal == NULL ||
        InstallWinIoDriver == NULL || RemoveWinIoDriver == NULL) {
        MessageBox("�ڶ�ȡWinIo32.DLL������ַʱ����\
			������DLL�汾���Ի��ļ��ѱ��𻵡�", "ϵͳ������ʾ", MB_ICONEXCLAMATION);
        return FALSE;
    }

    //��ʼ����ϵͳ�����ʧ�ܣ�������������г���
    bool bInit = (bool)InitializeWinIo();

    if (bInit == false) {
        MessageBox("��ʼ��ʧ�ܣ����������г���", "ϵͳ������ʾ", MB_ICONEXCLAMATION);
        return FALSE;
    }

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMemoryAccessDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMemoryAccessDlg::OnPaint()
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
HCURSOR CMemoryAccessDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CMemoryAccessDlg::OnBUTTONRead()
{
    //���ζ�ȡCMOS�У�������ʱ���йصļ����
    UpdateData(TRUE);
    m_strDateTime.Format("������%X%X��%X��%X��[��%s] %X��%X��%X��",
                         (UCHAR)ReadFromCMOS(0x32),//����
                         (UCHAR)ReadFromCMOS(0x09),//��
                         (UCHAR)ReadFromCMOS(0x08),//��
                         (UCHAR)ReadFromCMOS(0x07),//��
                         GetWeekName((UCHAR)ReadFromCMOS(0x06)),//����
                         (UCHAR)ReadFromCMOS(0x04),//ʱ
                         (UCHAR)ReadFromCMOS(0x02),//��
                         (UCHAR)ReadFromCMOS(0x00));//��
    UpdateData(FALSE);
}

UCHAR CMemoryAccessDlg::ReadFromCMOS(int index)
{
    //���ݶ˿ڶ�ȡCMOS�е����ݡ�
    if (index < 0 || index > 0x80)
        return 0xFF;

    DWORD dwPortVal = (DWORD)index;

    if (!SetPortVal(0x70, dwPortVal, 1)) {
        return 0xFF;
    }

    dwPortVal = 0xFF;
    DWORD *pdwPortVal = (DWORD *)&dwPortVal;

    if (!GetPortVal(0x71, pdwPortVal, 1)) {
        return 0xFF;
    }

    return (UCHAR)dwPortVal;
}

CString CMemoryAccessDlg::GetWeekName(UCHAR week)
{
    //����������ת��Ϊ���֡�
    CString strWeek;

    switch (week) {
    case 0:
        strWeek = "��";
        break;

    case 1:
        strWeek = "һ";
        break;

    case 2:
        strWeek = "��";
        break;

    case 3:
        strWeek = "��";
        break;

    case 4:
        strWeek = "��";
        break;

    case 5:
        strWeek = "��";
        break;

    case 6:
        strWeek = "��";
        break;

    default:
        break;
    }

    return strWeek;
}

void CMemoryAccessDlg::OnOK()
{
    // TODO: Add extra validation here
    //CDialog::OnOK();
}

void CMemoryAccessDlg::OnCancel()
{
    //�����˳�����Ҫ��ɵ���WinIO�Ĺرգ���Դ���ͷš�
    if (MessageBox("ȷʵҪ�˳����Ƿ������", "������ʾ", MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2) == IDOK) {
        ShutdownWinIo();
        CDialog::OnCancel();
    }
}
