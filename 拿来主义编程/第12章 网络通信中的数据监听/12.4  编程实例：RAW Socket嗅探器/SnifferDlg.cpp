// SnifferDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Sniffer.h"
#include "SnifferDlg.h"
#include "DialogSelectIP.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib,"ws2_32")

//#define LIST_RECEIVE_INDEX        0
#define LIST_RECEIVE_TIME       0
#define LIST_RECEIVE_PROTOCOL   1
#define LIST_RECEIVE_IP1        2
#define LIST_RECEIVE_PORT1      3
#define LIST_RECEIVE_IP2        4
#define LIST_RECEIVE_PORT2      5
#define LIST_RECEIVE_LEN        6
#define LIST_RECEIVE_CONTENT    7

#define STATUS_FAILED 0xFFFF //�����쳣�������
#define MAX_PACK_LEN 65535 //���յ����IP����
#define MAX_ADDR_LEN 16 //���ʮ���Ƶ�ַ����󳤶�
#define MAX_PROTO_TEXT_LEN 16 //��Э������(��"TCP")��󳤶�
#define MAX_PROTO_NUM 12 //��Э������
#define MAX_HOSTNAME_LAN 255 //�������������
#define CMD_PARAM_HELP true
#define SIO_RCVALL _WSAIOW(IOC_VENDOR,1)
#define LOCALSETFILENAME ".\\Sniffer.ini"
#define MAX_PACK_SAVE_LEN 3000
#define MAX_PACK_NUM 10000

typedef union MultiByteStruct {
    //IP��ַ������
    int iInt;
    float fFloat;
    UINT uInt;
    DWORD dwDword;
    WORD wWord[2];
    UCHAR ucByte[4];
} UNIONIP, *PUNIONIP;

typedef struct _iphdr {
    unsigned char h_lenver; //4λ�ײ�����+4λIP�汾��
    unsigned char tos; //8λ��������TOS
    unsigned short total_len; //16λ�ܳ��ȣ��ֽڣ�
    unsigned short ident; //16λ��ʶ
    unsigned short frag_and_flags; //3λ��־λ
    unsigned char ttl; //8λ����ʱ�� TTL
    unsigned char proto; //8λЭ�� (TCP, UDP ������)
    unsigned short checksum; //16λIP�ײ�У���
    unsigned int sourceIP; //32λԴIP��ַ
    unsigned int destIP; //32λĿ��IP��ַ
} IP_HEADER;

typedef struct _tcphdr { //����TCP�ײ�
    USHORT th_sport; //16λԴ�˿�
    USHORT th_dport; //16λĿ�Ķ˿�
    unsigned int th_seq; //32λ���к�
    unsigned int th_ack; //32λȷ�Ϻ�
    unsigned char th_lenres; //4λ�ײ�����/6λ������
    unsigned char th_flag; //6λ��־λ
    USHORT th_win; //16λ���ڴ�С
    USHORT th_sum; //16λУ���
    USHORT th_urp; //16λ��������ƫ����
} TCP_HEADER, *PTCP_HEADER;

typedef struct _udphdr { //����UDP�ײ�
    unsigned short uh_sport;     //16λԴ�˿�
    unsigned short uh_dport;     //16λĿ�Ķ˿�
    unsigned short uh_len;           //16λ����
    unsigned short uh_sum;    //16λУ���
} UDP_HEADER, *PUDP_HEADER;

typedef struct _icmphdr { //����ICMP�ײ�
    BYTE  i_type; //8λ����
    BYTE  i_code; //8λ����
    USHORT i_cksum; //16λУ���
    USHORT i_id; //ʶ��ţ�һ���ý��̺���Ϊʶ��ţ�
    USHORT i_seq; //�������к�
    //ULONG timestamp; //ʱ���
} ICMP_HEADER;

typedef struct _packet {
    IP_HEADER ipheader;
    union Translayer {
        TCP_HEADER tcpheader;
        UDP_HEADER udpheader;
        ICMP_HEADER icmpheader;
    } translayer;
} PACKET;

typedef struct _protomap { //������Э��ӳ���
    int ProtoNum;
    char ProtoText[MAX_PROTO_TEXT_LEN];
} PROTOMAP;

PROTOMAP ProtoMap[MAX_PROTO_NUM] = { //Ϊ��Э��ӳ���ֵ
    { IPPROTO_IP  , "IP " },
    { IPPROTO_ICMP , "ICMP" },
    { IPPROTO_IGMP , "IGMP" },
    { IPPROTO_GGP , "GGP " },
    { IPPROTO_TCP , "TCP " },
    { IPPROTO_PUP , "PUP " },
    { IPPROTO_UDP , "UDP " },
    { IPPROTO_IDP , "IDP " },
    { IPPROTO_ND  , "NP " },
    { IPPROTO_RAW , "RAW " },
    { IPPROTO_MAX , "MAX " },
    { NULL , "" }
};

//�߳̽ṹ
typedef struct _threadParamStruct {
    int iIndex;
    CString strIP;
    BOOL g_bRunning;
    CSnifferDlg *pDlg;
} THREADPARAM;

THREADPARAM ThreadParam[MAXIP];
int g_iToltalIP = 0;
UINT SnifferThread(LPVOID pParam);
CString g_strIPFileter = "";
UINT g_uPortFileter = 0;

CString IPIntToStr(UINT IPInt);
CString IPDwordToStr(DWORD IPDword);
UINT IPStrToInt(CString IPStr);

BOOL g_ThreadStop;
int g_TotalNum;
char SaveBuf[MAX_PACK_NUM][MAX_PACK_SAVE_LEN];
int SaveBufLen[MAX_PACK_NUM];
DWORD RecvTime[MAX_PACK_SAVE_LEN];


//���ֶ�̬�����ڴ治���ʡ�
//char **SaveBuf;
//DWORD *RecvTime;

CStringArray IPArray, FindArray;

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
// CSnifferDlg dialog

CSnifferDlg::CSnifferDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CSnifferDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CSnifferDlg)
    m_strContent = _T("");
    m_iShowMode = -1;
    m_strComment = _T("");
    m_strIPFilter = _T("");
    m_uPortFilter = 0;
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSnifferDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSnifferDlg)
    DDX_Control(pDX, IDC_LIST_Receive, m_ctlListReceive);
    DDX_Text(pDX, IDC_EDIT_Content, m_strContent);
    DDX_Radio(pDX, IDC_RADIO_ShowMode, m_iShowMode);
    DDX_Text(pDX, IDC_EDIT_Comment, m_strComment);
    DDX_Text(pDX, IDC_EDIT_IPFilter, m_strIPFilter);
    DDX_Text(pDX, IDC_EDIT_PortFilter, m_uPortFilter);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSnifferDlg, CDialog)
    //{{AFX_MSG_MAP(CSnifferDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_Start, OnBUTTONStart)
    ON_BN_CLICKED(IDC_BUTTON_Stop, OnBUTTONStop)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_Receive, OnDblclkLISTReceive)
    ON_NOTIFY(NM_CLICK, IDC_LIST_Receive, OnClickLISTReceive)
    ON_BN_CLICKED(IDC_BUTTON_ClearResult, OnBUTTONClearResult)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSnifferDlg message handlers

BOOL CSnifferDlg::OnInitDialog()
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
    m_ctlListReceive.InsertColumn(LIST_RECEIVE_TIME, "ʱ��", LVCFMT_LEFT, 60);
    m_ctlListReceive.InsertColumn(LIST_RECEIVE_PROTOCOL, "Э��", LVCFMT_LEFT, 35);
    m_ctlListReceive.InsertColumn(LIST_RECEIVE_IP1, "ԴIP", LVCFMT_LEFT, 100);
    m_ctlListReceive.InsertColumn(LIST_RECEIVE_PORT1, "ԴPORT", LVCFMT_LEFT, 50);
    m_ctlListReceive.InsertColumn(LIST_RECEIVE_IP2, "ĿIP", LVCFMT_LEFT, 100);
    m_ctlListReceive.InsertColumn(LIST_RECEIVE_PORT2, "ĿPORT", LVCFMT_LEFT, 50);
    m_ctlListReceive.InsertColumn(LIST_RECEIVE_LEN, "����", LVCFMT_LEFT, 40);
    m_ctlListReceive.InsertColumn(LIST_RECEIVE_CONTENT, "����", LVCFMT_LEFT, 270);
    m_ctlListReceive.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    CString strTemp;
    //char buff[MAX_PATH];
    //�������ó�ʼ��.
    WSADATA wsaData;
    WORD version = MAKEWORD(2, 0);
    int ret = WSAStartup(version, &wsaData);

    if (ret != 0) {
        MessageBox("�׽���Socket��ʼ������!\n", "������ʾ", MB_OK);
        OnOK();
    }

    //��ð���.
    struct hostent *pHost = gethostbyname("");

    if (pHost == NULL) {
        MessageBox("�޷��������������", "������ʾ", MB_OK | MB_ICONERROR);
        return FALSE;
    }

    g_iToltalIP = 0;

    for (int i = 0; i < MAXIP; i++) {
        char *ptrIP = pHost->h_addr_list[i];

        if (ptrIP == NULL)
            break;

        LPCSTR psz = inet_ntoa(*(struct in_addr *)ptrIP);

        if (psz != NULL) {
            ThreadParam[i].pDlg = this;
            ThreadParam[i].strIP.Format("%s", psz);
            ThreadParam[i].g_bRunning = FALSE;
            ThreadParam[i].iIndex = g_iToltalIP;
            g_iToltalIP++;
        } else
            break;
    }

    if (g_iToltalIP <= 0) {
        MessageBox("�޷���������ת��ΪIP��ַ", "������ʾ", MB_OK | MB_ICONERROR);
        //return FALSE;
    }

    m_iShowMode = 0;
    UpdateData(FALSE);
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSnifferDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSnifferDlg::OnPaint()
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
HCURSOR CSnifferDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CSnifferDlg::OnOK()
{
    // TODO: Add extra validation here
    //CDialog::OnOK();
}

void CSnifferDlg::OnCancel()
{
    // TODO: Add extra cleanup here
    if (MessageBox("ȷʵҪ�˳���?", "�˳�ϵͳ", MB_OKCANCEL | MB_DEFBUTTON2 | MB_ICONQUESTION) == IDOK) {
        OnBUTTONStop();
        Sleep(1000);
        CDialog::OnCancel();
    }
}

void CSnifferDlg::OnBUTTONStart()
{
    // TODO: Add your control notification handler code here
    CString strTemp;
    UpdateData(TRUE);

    if (g_iToltalIP == 0) {
        MessageBox("����IP��ַ��ȡ��������������");
        return;
    }

    int iSel = 0;

    if (g_iToltalIP > 1) {
        CDialogSelectIP dlg;

        for (int i = 0; i < g_iToltalIP; i++) {
            dlg.m_strIP[i] = ThreadParam[i].strIP;
        }

        dlg.m_iTotalIP = g_iToltalIP;

        if (dlg.DoModal() != IDOK)
            return;

        iSel = dlg.m_iCurSel;
    }

    if (ThreadParam[iSel].g_bRunning) {
        m_strComment.Format("��IP%s�����ڼ�����", ThreadParam[iSel].strIP);
    } else {
        GetDlgItem(IDC_EDIT_IPFilter)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_PortFilter)->EnableWindow(FALSE);
        g_strIPFileter = m_strIPFilter;
        AfxBeginThread(SnifferThread, &ThreadParam[iSel], THREAD_PRIORITY_NORMAL);
        m_strComment.Format("����%s�߳�����������.......", ThreadParam[iSel].strIP);
        Sleep(100);
    }

    UpdateData(FALSE);
}

UINT SnifferThread(LPVOID pParam)
{
    THREADPARAM* threadparam = (THREADPARAM*)pParam;
    CSnifferDlg *ptr = threadparam->pDlg;
    CString strIP = threadparam->strIP;
    CString strTemp;
    SOCKET sock;
    SOCKADDR_IN addr_local, addr_remote; //Local & Remote
    int buflen = sizeof(addr_remote);

    if ((sock = socket(AF_INET, SOCK_RAW, IPPROTO_IP)) == INVALID_SOCKET) {
        AfxMessageBox("ԭʼ�׽���Socket��������!\n", MB_OK | MB_ICONINFORMATION);
        return 1;
    }

    BOOL flag = TRUE;

    if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, (char *)&flag, sizeof(flag)) == SOCKET_ERROR) {
        AfxMessageBox("�׽���ѡ��IP_HDRINCL����!\n", MB_OK | MB_ICONINFORMATION);
        shutdown(sock, 2);
        closesocket(sock);
        return 2;
    }

    int nTimeOver = 5000;

    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&nTimeOver, sizeof(nTimeOver)) == SOCKET_ERROR) {
        AfxMessageBox("����ʱ��������ó���SO_SNDTIMEO!\n", MB_OK | MB_ICONINFORMATION);
        shutdown(sock, 2);
        closesocket(sock);
        return 3;
    }

    int localPort = GetPrivateProfileInt("Init", "LocalPort", 5000, LOCALSETFILENAME);
    addr_local.sin_family = AF_INET;
    addr_local.sin_port = htons(localPort);
    addr_local.sin_addr.s_addr = inet_addr(strIP);

    if (bind(sock, (PSOCKADDR)&addr_local, sizeof(addr_local)) == SOCKET_ERROR) {
        AfxMessageBox("�󶨱���IP�˿ڽ��ճ�����ȷ������������ȷ�����޸�Sniffer.ini�ļ��е�Init/LocalPort���ض˿�ֵ��", MB_OK | MB_ICONINFORMATION);
        shutdown(sock, 2);
        closesocket(sock);
        return 4;
    }

    DWORD dwBufferLen[10];
    DWORD dwBufferInLen = 1;
    DWORD dwBytesReturned = 0;

    if (WSAIoctl(sock, SIO_RCVALL, &dwBufferInLen, sizeof(dwBufferInLen), &dwBufferLen, sizeof(dwBufferLen), &dwBytesReturned, NULL, NULL) == SOCKET_ERROR) {
        AfxMessageBox("WSAIoctl���ó���", MB_OK | MB_ICONINFORMATION);
        shutdown(sock, 2);
        closesocket(sock);
        return 5;
    }

    threadparam->g_bRunning = TRUE;
    g_TotalNum = 0;
    UCHAR RecvBuf[MAX_PACK_LEN];
    IP_HEADER *pIPHead = (IP_HEADER *)RecvBuf;
    int iIPLen = sizeof(IP_HEADER);
    int iTcpLen = sizeof(TCP_HEADER);
    int iUdpLen = sizeof(UDP_HEADER);
    int iIcmpLen = sizeof(ICMP_HEADER);
    TCP_HEADER *pTcpHead = (TCP_HEADER *)(RecvBuf + iIPLen);
    UDP_HEADER *pUdpHead = (UDP_HEADER *)pTcpHead;
    ICMP_HEADER *pIcmpHead = (ICMP_HEADER *)pTcpHead;
    char *pTcpData = (char *)(RecvBuf + iIPLen + iTcpLen);
    char *pUdpData = (char *)(RecvBuf + iIPLen + iUdpLen);
    char *pIcmpData = (char *)(RecvBuf + iIPLen + iIcmpLen);

    while (threadparam->g_bRunning && g_TotalNum < MAX_PACK_NUM) {
        memset(RecvBuf, 0, sizeof(RecvBuf)); //���㣻
        int retlen = recvfrom(sock, (char *)RecvBuf, MAX_PACK_LEN, 0, (LPSOCKADDR)&addr_remote, &buflen);

        if (retlen == SOCKET_ERROR || retlen < 50)
            continue;

        CString strIPS = IPIntToStr(pIPHead->sourceIP);
        CString strIPD = IPIntToStr(pIPHead->destIP);

        if (g_strIPFileter != "" && strIPS != g_strIPFileter && strIPD != g_strIPFileter)
            continue;

        int copylen = retlen;

        if (copylen > MAX_PACK_SAVE_LEN)
            copylen = MAX_PACK_SAVE_LEN;

        SaveBufLen[g_TotalNum] = retlen;
        memcpy((char *)SaveBuf[g_TotalNum], (char *)RecvBuf, copylen); //���и��ƣ�����ֻ�������޸���
        CTime time = CTime::GetCurrentTime();
        ptr->m_ctlListReceive.InsertItem(g_TotalNum, time.Format("%H:%M:%S"));
        ptr->m_ctlListReceive.SetItemText(g_TotalNum, LIST_RECEIVE_IP1, strIPS);
        ptr->m_ctlListReceive.SetItemText(g_TotalNum, LIST_RECEIVE_IP2, strIPD);
        CString strPortSour, strPortDest, strMsg = "";
        int iDataLen = 0, i;

        switch (pIPHead->proto) {
        case 1://ICMP
            strPortSour = "/";
            strPortDest = "/";
            ptr->m_ctlListReceive.SetItemText(g_TotalNum, LIST_RECEIVE_PROTOCOL, "ICMP");
            iDataLen = retlen - (iIPLen + iIcmpLen);

            for (i = 0; i < iDataLen; i++) {
                strTemp.Format("%02X ", (UCHAR)(*(pIcmpData + i)));
                strMsg += strTemp;
            }

            break;

        case 6://TCP
            strPortSour.Format("%d", ntohs(pTcpHead->th_sport));
            strPortDest.Format("%d", ntohs(pTcpHead->th_dport));
            ptr->m_ctlListReceive.SetItemText(g_TotalNum, LIST_RECEIVE_PROTOCOL, "TCP");
            iDataLen = retlen - (iIPLen + iTcpLen);

            for (i = 0; i < iDataLen; i++) {
                strTemp.Format("%02X ", (UCHAR)(*(pTcpData + i)));
                strMsg += strTemp;
            }

            break;

        case 17://UDP
            strPortSour.Format("%d", ntohs(pUdpHead->uh_sport));
            strPortDest.Format("%d", ntohs(pUdpHead->uh_dport));
            ptr->m_ctlListReceive.SetItemText(g_TotalNum, LIST_RECEIVE_PROTOCOL, "UDP");
            iDataLen = retlen - (iIPLen + iUdpLen);

            for (i = 0; i < iDataLen; i++) {
                strTemp.Format("%02X ", (UCHAR)(*(pUdpData + i)));
                strMsg += strTemp;
            }

            break;

        default:
            ptr->m_ctlListReceive.SetItemText(g_TotalNum, LIST_RECEIVE_PROTOCOL, "ETC");
            break;
        }

        strTemp.Format("%d", retlen);
        ptr->m_ctlListReceive.SetItemText(g_TotalNum, LIST_RECEIVE_LEN, strTemp);
        ptr->m_ctlListReceive.SetItemText(g_TotalNum, LIST_RECEIVE_PORT1, strPortSour);
        ptr->m_ctlListReceive.SetItemText(g_TotalNum, LIST_RECEIVE_PORT2, strPortDest);
        ptr->m_ctlListReceive.SetItemText(g_TotalNum, LIST_RECEIVE_CONTENT, strMsg);
        g_TotalNum++;
    }//while (threadparam->g_bRunning)

    shutdown(sock, 2);
    closesocket(sock);
    //AfxMessageBox(strIP+" over");
    threadparam->g_bRunning = FALSE;
    return 0;
}

CString IPIntToStr(UINT IPInt)
{
    UNIONIP IP;
    CString IPStr;
    IP.uInt = IPInt;
    IPStr.Format("%d.%d.%d.%d", IP.ucByte[0], IP.ucByte[1], IP.ucByte[2], IP.ucByte[3]);
    return IPStr;
}

CString IPDwordToStr(DWORD IPDword)
{
    UNIONIP IP;
    CString IPStr;
    IP.dwDword = IPDword;
    IPStr.Format("%d.%d.%d.%d", IP.ucByte[3], IP.ucByte[2], IP.ucByte[1], IP.ucByte[0]);
    return IPStr;
}

UINT IPStrToInt(CString IPStr)
{
    UNIONIP IP;
    int i, j = 0;
    IPStr.TrimLeft(" ");
    IPStr.TrimRight(" ");

    for (i = 0; i < IPStr.GetLength(); i++) {
        if (IPStr.GetAt(i) < '0' || IPStr.GetAt(i) > '9')
            if (IPStr.GetAt(i) == '.')
                j++;
            else
                return 0;
    }

    if (j != 3)
        return 0;

    i = 0;
    IPStr += ".";
    CString temp;

    for (int m = 0; m < 4; m++) {
        temp = "";

        while (IPStr.GetAt(i) != '.') {
            temp += IPStr.GetAt(i);
            i++;
        }

        i++;

        if (temp == "" || atoi(temp) > 0xFF)
            return 0;
        else
            IP.ucByte[m] = atoi(temp);
    }

    return IP.uInt;
}


void CSnifferDlg::OnBUTTONStop()
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);

    for (int i = 0; i < g_iToltalIP; i++) {
        ThreadParam[i].g_bRunning = FALSE;
    }

    GetDlgItem(IDC_EDIT_IPFilter)->EnableWindow(TRUE);
    GetDlgItem(IDC_EDIT_PortFilter)->EnableWindow(TRUE);
    m_strComment = "�߳��ѹرա�";
    UpdateData(FALSE);
}

void CSnifferDlg::OnDblclkLISTReceive(NMHDR* pNMHDR, LRESULT* pResult)
{
    // TODO: Add your control notification handler code here
    POSITION posReceive = m_ctlListReceive.GetFirstSelectedItemPosition();

    if (posReceive == NULL) {
        *pResult = 0;
        return;
    }

    int index = m_ctlListReceive.GetNextSelectedItem(posReceive);
    UpdateData(TRUE);
    CString strTemp;
    int showlen = SaveBufLen[index];

    if (showlen > MAX_PACK_SAVE_LEN)
        showlen = MAX_PACK_SAVE_LEN;

    int iOffset = 0;
    int packlen = sizeof(IP_HEADER);
    m_strContent = "IP��ͷ:";

    for (int i = 0; i < packlen; i++) {
        strTemp.Format("%02X ", (UCHAR)SaveBuf[index][iOffset + i]);
        m_strContent += strTemp;
    }

    m_strContent += "\r\n";
    iOffset += packlen;
    strTemp = m_ctlListReceive.GetItemText(index, LIST_RECEIVE_PROTOCOL);

    if (strTemp == "TCP") {
        packlen = sizeof(TCP_HEADER);
        m_strContent += "TCP��ͷ:";
    }

    if (strTemp == "UDP") {
        packlen = sizeof(UDP_HEADER);
        m_strContent += "UDP��ͷ:";
    }

    if (strTemp == "ICMP") {
        packlen = sizeof(ICMP_HEADER);
        m_strContent += "ICMP��ͷ:";
    }

    if (strTemp == "ETC")
        packlen = 0;

    for (i = 0; i < packlen; i++) {
        strTemp.Format("%02X ", (UCHAR)SaveBuf[index][iOffset + i]);
        m_strContent += strTemp;
    }

    m_strContent += "\r\n";
    iOffset += packlen;
    m_strContent += "����:\r\n";

    switch (m_iShowMode) {
    case 0: { //ʮ�����ơ�
        for (i = 0; i < showlen - iOffset; i++) {
            strTemp.Format("%02X ", (UCHAR)SaveBuf[index][iOffset + i]);
            m_strContent += strTemp;

            if (i % 40 == 39) //ÿ40���ֽڻ�һ�С�
                m_strContent += "\r\n";
        }
    }
    break;

    case 1: { //ASCII
        for (int i = 0; i < showlen - iOffset; i++) {
            strTemp.Format("%c", (UCHAR)SaveBuf[index][iOffset + i]);
            m_strContent += strTemp;
        }
    }
    break;

    default:
        break;
    }

    UpdateData(FALSE);
    *pResult = 0;
}

void CSnifferDlg::OnClickLISTReceive(NMHDR* pNMHDR, LRESULT* pResult)
{
    // TODO: Add your control notification handler code here
    OnDblclkLISTReceive(pNMHDR, pResult);
    *pResult = 0;
}

void CSnifferDlg::OnBUTTONClearResult()
{
    // TODO: Add your control notification handler code here
    m_ctlListReceive.DeleteAllItems();
    g_TotalNum = 0;
}
