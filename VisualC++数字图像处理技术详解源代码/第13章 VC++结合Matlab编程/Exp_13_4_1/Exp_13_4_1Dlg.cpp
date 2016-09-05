// Exp_13_4_1Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Exp_13_4_1.h"
#include "Exp_13_4_1Dlg.h"
#include "engine.h"

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


// CExp_13_4_1Dlg �Ի���




CExp_13_4_1Dlg::CExp_13_4_1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExp_13_4_1Dlg::IDD, pParent)
    , m_strFile(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExp_13_4_1Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDT_FILE, m_strFile);
}

BEGIN_MESSAGE_MAP(CExp_13_4_1Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTN_FILE, &CExp_13_4_1Dlg::OnBnClickedBtnFile)
    ON_BN_CLICKED(IDC_BTN_EXECUTE, &CExp_13_4_1Dlg::OnBnClickedBtnExecute)
    ON_BN_CLICKED(IDC_BTN_EXIT, &CExp_13_4_1Dlg::OnBnClickedBtnExit)
END_MESSAGE_MAP()


// CExp_13_4_1Dlg ��Ϣ�������

BOOL CExp_13_4_1Dlg::OnInitDialog()
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
    // ����Ĭ�������ļ�·��
    TCHAR szCurrentDir[MAX_PATH];   
    GetCurrentDirectory(MAX_PATH, szCurrentDir);   
    m_strFile.Format(_T("%s\\%s"), szCurrentDir,"data.txt");
    m_strPath.Format(_T("%s"), szCurrentDir);

    // ���½�������
    UpdateData(FALSE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CExp_13_4_1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CExp_13_4_1Dlg::OnPaint()
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
HCURSOR CExp_13_4_1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CExp_13_4_1Dlg::OnBnClickedBtnFile()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CFileDialog fileDlg(FALSE,
                       "������Ƶ�����ļ�",
                       "data.txt",
                       OFN_HIDEREADONLY | OFN_NOCHANGEDIR,
                       "�ı��ļ�(*.txt)|*.txt||");
    if(fileDlg.DoModal() == IDOK)
    {
        m_strFile = fileDlg.GetPathName();
        UpdateData(FALSE);
    }
}

void CExp_13_4_1Dlg::OnBnClickedBtnExecute()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    // ״̬����
    int nStatus = 0;
	
	// ����MATLAB����ָ��
	Engine *ep;
	
	// ��MATLAB����
	ep = engOpen(NULL);
	if(ep == NULL)
	{
        AfxMessageBox("�޷���MATLAB���档");
		return;
	}

    // �Ƿ���ʾMATLAB���ڣ�true-��ʾ��false-���ء�
    nStatus = engSetVisible(ep, false);
    if(nStatus != 0)
    {
        AfxMessageBox("����MATLAB��ʾ����ʧ�ܡ�");
        return;
    }

    // �л���M�ļ�����Ƶ���ڵ�Ŀ¼
    CString strCmd;
    strCmd.Format("cd %s",m_strPath);
    nStatus = engEvalString(ep,strCmd);
     if(nStatus != 0)
    {
        AfxMessageBox("ִ��MATLAB�л�Ŀ¼����ʧ�ܡ�");
        return;
    }

    // ִ�����������ȡ��Ƶ����
    nStatus = engEvalString(ep,"pixels=getVideo;");
    if(nStatus != 0)
    {
        AfxMessageBox("ִ��MATLAB��ȡ��Ƶ��������ʧ�ܡ�");
        return;
    }

    // ��MATLAB����ռ��ж�ȡִ�н��
    unsigned int nDim[3];
    nDim[0] = 240;
    nDim[1] = 360;
    nDim[2] = 40;
    mxArray *A = mxCreateNumericArray(3,nDim,mxLOGICAL_CLASS,mxREAL); 
    A = engGetVariable(ep,"pixels");
    double *pixels = new double[240*360*40];
    memset(pixels,0,sizeof(double)*240*360*40);
    memcpy(pixels,mxGetPr(A),sizeof(double)*240*360*40);

    // �����ݴ����ļ�
    CFile file;
    file.Open(m_strFile,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite); 
    CArchive ar(&file, CArchive::store); 
    ar.Write(pixels,sizeof(double)*240*360*40); 
    ar.Close();
    file.Close();

    // �ͷž�������ռ�
    mxDestroyArray(A);  
    delete [] pixels;

    // �ر�MATLAB����
    engClose(ep);

    // ���ݻ�ȡ�����ʾ
    AfxMessageBox("��Ƶ������ȡ�ɹ���");

}

void CExp_13_4_1Dlg::OnBnClickedBtnExit()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CDialog::OnOK();
}
