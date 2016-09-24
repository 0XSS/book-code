// PortSnifferDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PortSniffer.h"
#include "PortSnifferDlg.h"

#include "VSPE_API.h"
#pragma comment(lib,"VSPE_API.lib")

#include "PComm.h"
#pragma comment(lib,"PComm.lib")

//һЩ����������INI�ļ��С�
#define INIFILENAME ".\\PortSniffer.ini"
#define MAXBUFFLEN  2048

#define LIST_COMMAND_DIRECTION  0
#define LIST_COMMAND_TIME       1
#define LIST_COMMAND_COMMAND    2

//�߳̽ṹ
typedef struct _threadParamStruct {
    int iIndex;
    int iCom;
    CString strIP;
    BOOL g_bRunning;
    CPortSnifferDlg *pDlg;
} THREADPARAM;
THREADPARAM g_sThreadParam;
BOOL g_bRunning;
UINT RealCOMThread(LPVOID pParam);

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
// CPortSnifferDlg dialog

CPortSnifferDlg::CPortSnifferDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CPortSnifferDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPortSnifferDlg)
    m_strComment = _T("");
    m_iComReal = 0;
    m_iComVirtual = 0;
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPortSnifferDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPortSnifferDlg)
    DDX_Control(pDX, IDC_LIST_Command, m_ctlListCommand);
    DDX_Text(pDX, IDC_EDIT_Comment, m_strComment);
    DDX_Text(pDX, IDC_EDIT_ComReal, m_iComReal);
    DDX_Text(pDX, IDC_EDIT_ComVirtual, m_iComVirtual);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPortSnifferDlg, CDialog)
    //{{AFX_MSG_MAP(CPortSnifferDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTONStart, OnBUTTONStart)
    ON_BN_CLICKED(IDC_BUTTONDStop, OnBUTTONDStop)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPortSnifferDlg message handlers

BOOL CPortSnifferDlg::OnInitDialog()
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
    //��������ĳ�ʼ����
    CString strTemp;
    m_ctlListCommand.InsertColumn(LIST_COMMAND_DIRECTION, "����", LVCFMT_LEFT, 40);
    m_ctlListCommand.InsertColumn(LIST_COMMAND_TIME, "ʱ��", LVCFMT_LEFT, 60);
    m_ctlListCommand.InsertColumn(LIST_COMMAND_COMMAND, "����", LVCFMT_LEFT, 300);
    m_ctlListCommand.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    Init();//����VSPE�ĳ�ʼ��������
    m_bReading = FALSE; //���ڲ����ã�����Ҫ���м�ء�
    //����ʵ���ڡ�
    m_iComVirtual = GetPrivateProfileInt("COM", "ComVirtual", 5, INIFILENAME);
    m_iComReal = GetPrivateProfileInt("COM", "ComReal", 3, INIFILENAME);
    m_strComment.Format("������Ҫ���������⴮�ڣ��Լ�Ҫ���ӵ�ʵ���ڡ�");
    UpdateData(FALSE);
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPortSnifferDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPortSnifferDlg::OnPaint()
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
HCURSOR CPortSnifferDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CPortSnifferDlg::OnOK()
{
    // TODO: Add extra validation here
    //CDialog::OnOK();
}

void CPortSnifferDlg::OnCancel()
{
    //�˳�����
    if (MessageBox("ȷʵҪ�˳���?", "�˳�", MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2) == IDOK) {
        CString strTemp, strTemp2;
        UpdateData(TRUE);
        OnBUTTONDStop();
        //���浱ǰCOM���������յĲ�����
        strTemp.Format("%d", m_iComVirtual);
        WritePrivateProfileString("COM", "ComVirtual", strTemp, INIFILENAME);
        strTemp.Format("%d", m_iComReal);
        WritePrivateProfileString("COM", "ComReal", strTemp, INIFILENAME);
        Uninit();
        CDialog::OnCancel();
    }
}

void CPortSnifferDlg::Init()
{
    //VSPE����ͨ�������뼤�
    const char* activationKey = "SRBGMZSYPuIHWILsmLjF5CDyBL3GQYD0IPIEvcZBgPQg8gS9xkGA9CDyBL3GQYD0IPIEvcZBgPQg8gS9xkGA9CDyBL3GQYD0IPIEvcZBgPQg8gS9xkGA9CDyBL3GQYD0IPIEvcZBgPQg8gS9xkGA9CDyBL3GQYD0IPIEvcZBgPRJEEYxlJg+4gdYguyYuMXkIPIEvcZBgPQg8gS9xkGA9CDyBL3GQYD0IPIEvcZBgPQg8gS9xkGA9CDyBL3GQYD0IPIEvcZBgPQg8gS9xkGA9CDyBL3GQYD0IPIEvcZBgPQg8gS9xkGA9CDyBL3GQYD0IPIEvcZBgPQg8gS9xkGA9JyUaC2ZWE1DZV2+wYWlRm7FFYrW3MDbZg8MkQsOQ8r1IPIEvcZBgPQg8gS9xkGA9LzrhjimHDiMlKqr6pSiw9CDl9n+0bAgFr2ho7nXjCoTMHYzt4tsbEkJGNktLGVG42SZ63UbmIUNcKmfhSzXldVCLhfvZv3StR9c/vkYG471Nh62eC1qIYuBUvm+a3BK8iR0POD8w5ovtuYr0T8aQP3eh4b8lUwnPHG9NRJxerttq/+/zX7c++9LDSQym3ThbWesK+A+X/vNw9qDgYt1dsJxDEEytsCRiT7bTiV5Djh1RlpIwETXWA089hiE9OYd7GpjKLq5dQOqSVcA3Fg1Wfdbqn/yn8q0/AIDOd0iZlbVeLY68zKh1Di4gGEoa1kR8EOBp2mxeaFrfwUm3DsJ5Pc04f7aEw9XljfBUwl/bAs3LVH5HRii8lXZvUVvnnfpcQ==1F250CF0960AE1C09E9450C816DE1232"; // <-----  PUT ACTIVATION KEY HERE
    CString strTemp;
    //����VSPE API
    bool result = vspe_activate(activationKey);

    if (result == false) {
        strTemp.Format("����ʧ�ܣ������һԭ��������δ��װ��汾���ԣ���ǰ�汾�ǣ�%s",
                       vspe_getVersionInformation());//��ʾVSPE API�İ汾
        MessageBox(strTemp);
        return;
    }

    //��ʼ����
    result = vspe_initialize();

    if (result == false) {
        strTemp.Format("��ʼ��ʧ�ܡ�");
        MessageBox(strTemp);
        return;
    }

    //ɾ�������Ѵ������豸��
    vspe_destroyAllDevices();
    //ֹͣ�����������е��豸��
    result = vspe_stopEmulation();

    if (result == false) {
        strTemp.Format("����:�е��豸�޷�ֹͣ�������ԡ�");
        MessageBox(strTemp, "������ʾ", MB_OK);
        return;
    }
}


void CPortSnifferDlg::Uninit()
{
    //�ͷ�VSPE��
    vspe_release();
}


UINT RealCOMThread(LPVOID pParam)
{
    //ת���̡߳�
    THREADPARAM* threadparam = (THREADPARAM*)pParam;
    CPortSnifferDlg *ptr = threadparam->pDlg;
    CListCtrl *pListCommand = (CListCtrl *)&ptr->m_ctlListCommand;
    CString strTemp;

    while (g_bRunning) {
        //�����ǰ��û������û���ӣ�������ת״̬��
        if (!ptr->m_bReading) {
            Sleep(1000);
            continue;
        }

        //�������ڼ����ݽ���ת����
        CString strTemp, strAll = "";
        UCHAR buff[MAXBUFFLEN] = {0};
        //���鴮�ڣ�תʵ����
        int len = sio_read(ptr->m_iComVirtual, (char *)buff, MAXBUFFLEN);
        sio_flush(ptr->m_iComVirtual, 0);

        if (len > 0) {
            //д�뵽ʵ���ڡ�
            sio_write(ptr->m_iComReal, (char *)buff, len);
            //����֮��ʾ����־�С�
            strAll = "";

            for (int c = 0; c < len; c++) {
                strTemp.Format("%02X ", buff[c]);
                strAll += strTemp;
            }

            ptr->WriteToList(TRUE, strAll);
        }

        //��ʵ���ڣ�ת������
        len = sio_read(ptr->m_iComReal, (char *)buff, MAXBUFFLEN);
        sio_flush(ptr->m_iComReal, 0);

        if (len > 0) {
            //д�뵽�鴮�ڡ�
            sio_write(ptr->m_iComVirtual, (char *)buff, len);
            //����֮��ʾ����־�С�
            strAll = "";

            for (int c = 0; c < len; c++) {
                strTemp.Format("%02X ", buff[c]);
                strAll += strTemp;
            }

            ptr->WriteToList(FALSE, strAll);
        }

        Sleep(100);
    }

    return 0;
}

void CPortSnifferDlg::OnBUTTONStart()
{
    //�������������5��������1��ʵ���ڣ�2����VSPE��
    //3���з���4�����鴮�ڣ�5����ת���̡߳�
    UpdateData(TRUE);
    CString strTemp;
    //1��ʵ����
    strTemp = ComConnect(m_iComReal);

    if (strTemp != "") {
        MessageBox(strTemp, "������ʾ", MB_OK);
        return;
    }

    //2����������������
    strTemp.Format("%d:0", m_iComVirtual);
    int deviceId = vspe_createDevice("Connector", strTemp);

    if (deviceId == -1) {
        strTemp.Format("�����޷������豸��");
        MessageBox(strTemp);
        vspe_release();
        return;
    }

    //3���з���
    bool result = vspe_startEmulation();

    if (result == false) {
        strTemp.Format("����ʧ�ܣ����������С�");
        MessageBox(strTemp);
        return;
    }

    //4�����鴮�ڡ�
    strTemp = ComConnect(m_iComVirtual);

    if (strTemp != "") {
        MessageBox(strTemp, "������ʾ", MB_OK);
        return;
    }

    //5��ʼ��ȡ
    g_bRunning = TRUE;
    g_sThreadParam.pDlg = this;
    AfxBeginThread(RealCOMThread, &g_sThreadParam, THREAD_PRIORITY_NORMAL);
    Sleep(100);
    m_bReading = TRUE; //���Կ�ʼ��ȡ�����ˡ�
    m_strComment.Format("�����ɹ����������״̬��");
    UpdateData(FALSE);
}

void CPortSnifferDlg::OnBUTTONDStop()
{
    //����1ͣת���߳�2��ʵ����3���鴮��
    //4ͣ����5ɾ���豸
    UpdateData(TRUE);
    CString strTemp;
    //1ֹͣת���̡߳�
    m_bReading = FALSE;
    g_bRunning = FALSE;
    Sleep(1000);
    //2��3��ʵ���鴮�ڡ�
    ComDisconnect(m_iComReal);
    ComDisconnect(m_iComVirtual);
    //4ֹͣ��ǰ��VSPE����
    bool result = vspe_stopEmulation();

    if (result == false) {
        strTemp.Format("����:�����޷�ֹͣ�������²�����");
        MessageBox(strTemp);
        return;
    }

    //5ɾ�������Ѵ������豸��
    vspe_destroyAllDevices();
    m_strComment.Format("ֹͣ�ɹ����˳�����״̬��");
    UpdateData(FALSE);
}

CString CPortSnifferDlg::ComConnect(int iCom)
{
    //���Ӵ��ڣ�������ʵ���ڣ������鴮�ڡ�
    //����ֵ���Ϊ�մ������ʾ���������������ʾ������Ϣ��
    UpdateData(TRUE);
    CString strTemp = "";

    if (sio_open(iCom) != SIO_OK) {
        strTemp.Format("�˿�COM%d��ʧ�ܡ�", iCom);
    } else {
        if (sio_ioctl(iCom, B38400, BIT_8 | STOP_1 | P_NONE) != SIO_OK) {
            ComDisconnect(iCom);
            strTemp.Format("�˿�COM%d�򿪳ɹ������������ó���", iCom);
        } else {
            if (sio_flowctrl(iCom, 0) != SIO_OK) {
                ComDisconnect(iCom);
                strTemp.Format("�˿�COM%d�򿪳ɹ������������ó���", iCom);
            }
        }
    }

    return strTemp;
}

CString CPortSnifferDlg::ComDisconnect(int iCom)
{
    //�رմ��ڣ�������ʵ���ڣ������鴮�ڡ�
    //����ֵ���Ϊ�մ������ʾ���������������ʾ������Ϣ��
    UpdateData(FALSE);
    sio_close(m_iComReal);
    return "";
}

void CPortSnifferDlg::WriteToList(BOOL bDir, CString strMsg)
{
    //��ת������Ϣд�뵽��־�С�
    int iCount = m_ctlListCommand.GetItemCount();

    if (bDir)
        m_ctlListCommand.InsertItem(iCount, "����"); //LIST_COMMAND_DIRECTION
    else
        m_ctlListCommand.InsertItem(iCount, "����"); //LIST_COMMAND_DIRECTION

    m_ctlListCommand.SetItemText(iCount, LIST_COMMAND_COMMAND, strMsg);
    CTime time = CTime::GetCurrentTime();
    m_ctlListCommand.SetItemText(iCount, LIST_COMMAND_TIME, time.Format("%H:%M:%S"));
    //m_ctlListCommand.SetItemData(iCount,(DWORD)len);
}
