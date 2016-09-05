// Exp_13_4_2Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Exp_13_4_2.h"
#include "Exp_13_4_2Dlg.h"
#include "tracking.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CExp_13_4_2Dlg �Ի���




CExp_13_4_2Dlg::CExp_13_4_2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExp_13_4_2Dlg::IDD, pParent)
    , m_strFile(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExp_13_4_2Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDT_FILE, m_strFile);
}

BEGIN_MESSAGE_MAP(CExp_13_4_2Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTN_OPENFILE, &CExp_13_4_2Dlg::OnBnClickedBtnOpenfile)
    ON_BN_CLICKED(IDC_BTN_EXECUTE, &CExp_13_4_2Dlg::OnBnClickedBtnExecute)
    ON_BN_CLICKED(IDC_BTN_EXIT, &CExp_13_4_2Dlg::OnBnClickedBtnExit)
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CExp_13_4_2Dlg ��Ϣ�������

BOOL CExp_13_4_2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
    bool bRes = false;

    // ��ʼ��Ӧ�ó���
    bRes = mclInitializeApplication(NULL,0);
    if(!bRes)
    {
        AfxMessageBox("Ӧ�ó����ʼ��ʧ�ܡ�");
        return FALSE;
    }

    // ����Ĭ�������ļ�·��
    TCHAR szCurrentDir[MAX_PATH];   
    GetCurrentDirectory(MAX_PATH, szCurrentDir);   
    m_strFile.Format(_T("%s\\%s"), szCurrentDir,"data.txt");

    // ���½�������
    UpdateData(FALSE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CExp_13_4_2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CExp_13_4_2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CExp_13_4_2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CExp_13_4_2Dlg::OnBnClickedBtnOpenfile()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CFileDialog fileDlg(TRUE,
                       "����Ƶ�����ļ�",
                        NULL,
                        OFN_HIDEREADONLY | OFN_NOCHANGEDIR,
                        "�ı��ļ�(*.txt)|*.txt||");
    if(fileDlg.DoModal() == IDOK)
    {
        m_strFile = fileDlg.GetPathName();
        UpdateData(FALSE);
    }

}

void CExp_13_4_2Dlg::OnBnClickedBtnExecute()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������  
    // ��������
    UpdateData(TRUE);

    // ���ļ���ȡ����
    CFile file;
    file.Open(m_strFile, CFile::modeRead);
    CArchive ar(&file, CArchive::load); 
    double *pixels = new double[240*360*40];
    memset(pixels,0,sizeof(double)*240*360*40);
    ar.Read(pixels,sizeof(double)*240*360*40); 
    ar.Close();
    file.Close();

    // �����ݴ���mwArray����
    unsigned int nDim[3];
    nDim[0] = 240;
    nDim[1] = 360;
    nDim[2] = 40;
    mwArray A(3,nDim,mxDOUBLE_CLASS); 
    A.SetData(pixels,240*360*40);

    // ��ʼ���˶����ٿ�
    bool bRes = false;     
    bRes = trackingInitialize();
    if(!bRes)
    {
        AfxMessageBox("�����˶����ٿ��ʼ��ʧ�ܡ�");
        return;
    }

    try
    { 
        // ���ø��ٺ���
        tracking(A);
    }
    catch(mwException e)
    {
        AfxMessageBox(e.what());
    }

    // �����������˶����ٿ�ĵ���
    trackingTerminate();

}

void CExp_13_4_2Dlg::OnBnClickedBtnExit()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CDialog::OnOK();
}

void CExp_13_4_2Dlg::OnDestroy()
{
    CDialog::OnDestroy();

    // TODO: �ڴ˴������Ϣ����������
    bool bRes = false;

    // ����Ӧ�ó���
    bRes = mclTerminateApplication();
    if(!bRes)
    {
        AfxMessageBox("����Ӧ�ó������");
    }           

}
