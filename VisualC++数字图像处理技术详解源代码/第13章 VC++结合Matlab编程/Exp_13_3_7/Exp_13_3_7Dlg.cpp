// Exp_13_3_7Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Exp_13_3_7.h"
#include "Exp_13_3_7Dlg.h"
#include "Dib.h"
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


// CExp_13_3_7Dlg �Ի���




CExp_13_3_7Dlg::CExp_13_3_7Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExp_13_3_7Dlg::IDD, pParent)
    , m_strAppPath(_T(""))
    , m_strFile(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExp_13_3_7Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STC_PIC, m_stcPic);
}

BEGIN_MESSAGE_MAP(CExp_13_3_7Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTN_OPEN, &CExp_13_3_7Dlg::OnBnClickedBtnOpen)
    ON_BN_CLICKED(IDC_BTN_CVT, &CExp_13_3_7Dlg::OnBnClickedBtnCvt)
    ON_BN_CLICKED(IDC_BTN_SAVE, &CExp_13_3_7Dlg::OnBnClickedBtnSave)
    ON_BN_CLICKED(IDC_BTN_EXIT, &CExp_13_3_7Dlg::OnBnClickedBtnExit)
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CExp_13_3_7Dlg ��Ϣ�������

BOOL CExp_13_3_7Dlg::OnInitDialog()
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
    // ���ݳ�ʼ��
    m_strFile = "";

    // ����Ĭ�������ļ�·��
    TCHAR szCurrentDir[MAX_PATH];   
    GetCurrentDirectory(MAX_PATH, szCurrentDir); 
    m_strAppPath.Format("%s",szCurrentDir);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CExp_13_3_7Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CExp_13_3_7Dlg::OnPaint()
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
        showPic();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CExp_13_3_7Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CExp_13_3_7Dlg::OnBnClickedBtnOpen()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CFileDialog fileDlg(TRUE,
                       "��λͼ",
                        NULL,
                        OFN_HIDEREADONLY | OFN_NOCHANGEDIR,
                        "λͼ�ļ�(*.bmp)|*.bmp||");
    if(fileDlg.DoModal() == IDOK)
    {
        // ��ȡ�ļ�����
        m_strFile = fileDlg.GetPathName();
        // ����λͼ
        m_dib.LoadFromFile(m_strFile);
        // ��ʾλͼ
        showPic();
    }
}

void CExp_13_3_7Dlg::OnBnClickedBtnCvt()
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
        AfxMessageBox("MATLAB�����ʧ�ܡ�");
		return;
	}

    // �Ƿ���ʾMATLAB���ڣ�true-��ʾ��false-���ء�
    nStatus = engSetVisible(ep, false);
    if(nStatus != 0)
    {
        AfxMessageBox("MATLAB��������ʧ�ܡ�");
		return;
    }

    // ��ȡλͼ�ĸ߶�
    LONG lHeight = m_dib.GetHeight();
    // ��ȡλͼ�Ŀ��
    LONG lWidth =  m_dib.GetWidth();

    // ����λͼ���ݾ���
    mwSize nDim[3];
    nDim[0] = lHeight;
    nDim[1] = lWidth;
    nDim[2] = 3;
    mxArray *A = mxCreateNumericArray(3,nDim,mxUINT8_CLASS,mxREAL);

    // ��ȡλͼ����
    LPBYTE lpData = m_dib.GetData();

    // ת��ΪMATLABλͼ��ʽ
    BYTE *pbData = new BYTE[lHeight*lWidth*3];
    BYTE *pbR = pbData;
    BYTE *pbG = pbData + lHeight*lWidth;
    BYTE *pbB = pbData + lHeight*lWidth*2;
    int n = 0;
    for(int j = 0; j < lWidth; j++)
    {
        for(int i = lHeight - 1 ; i >= 0 ; i--)
        {
            *(pbR+n) = lpData[i*m_dib.GetLineByte() + 3*j + 2];
            *(pbG+n) = lpData[i*m_dib.GetLineByte() + 3*j + 1];
            *(pbB+n) = lpData[i*m_dib.GetLineByte() + 3*j + 0];
            n++;           
        }
    }

    // �����ݴ���λͼ����
    memcpy(mxGetPr(A),pbData,lHeight*lWidth*3);
    // �ͷ��ڴ�
    delete [] pbData;

    // ���������Aд�뵽MATLAB����ռ�
    nStatus = engPutVariable(ep,"img",A);
    if(nStatus != 0)
    {
        AfxMessageBox("�������д��MATLAB����ռ�ʧ�ܡ�");
		return;
    }       

    // �ͷž�������ռ�
    mxDestroyArray(A);

    // ת��M�ļ�����Ŀ¼
    CString strCmd;
    strCmd.Format("cd %s\\CvtRgbToGray",m_strAppPath);
    nStatus = engEvalString(ep,strCmd);
    if(nStatus != 0)
    {
        AfxMessageBox("ת��M�ļ�����Ŀ¼ʧ�ܡ�");
		return;
    }

    // ����Simulinkģ��
    nStatus = engEvalString(ep,"sim('CvtRgbToGray',[0 10]);");
    if(nStatus != 0)
    {
        AfxMessageBox("����Simulinkģ��ʧ�ܡ�");
		return;
    }    

    // ��ȡ���ɵĻҶ�λͼ����
    nDim[0] = lWidth;
    nDim[1] = lHeight;
    mxArray *B = mxCreateNumericArray(2,nDim,mxUINT8_CLASS,mxREAL);
    pbData = new BYTE[lHeight*lWidth]; 
    
    // ���������ɵĻҶ�λͼ���ݴ���B��������ת��
    nStatus = engEvalString(ep,"B = grayImg(:,:,1)';");
    if(nStatus != 0)
    {
        AfxMessageBox("MATLABִ�лҶ�λͼ���ݶ�ȡʧ�ܡ�");
		return;
    }    

    // ��MATLAB�����ռ��ȡλͼ����
    B = engGetVariable(ep,"B");
    memcpy(pbData,mxGetPr(B),lHeight*lWidth);  

    // ����Ҷ�λͼ��������ռ�
    UINT uGrayLineByte = (lWidth + 3) / 4 * 4;
    DWORD dwGrayDataSize = uGrayLineByte * lHeight;
    BYTE *pbGrayData = new BYTE[dwGrayDataSize];

    // ��չΪÿ��4���ֽڵ��������������·�ת
    for(int i = 0; i < lHeight; i++)
    {
        for(int j = 0; j < lWidth; j++)
        {
            *(pbGrayData+(lHeight-i-1)*uGrayLineByte+j) = *(pbData+i*lWidth+j);
        }
    }

    // �滻Ϊ�Ҷ�����
    m_dib.ReplaceData(pbGrayData,FALSE);

    // �ͷ��ڴ�
    mxDestroyArray(B);
    delete [] pbData;
    delete [] pbGrayData;

    // �ر�MATLAB����
    engClose(ep);           

}

void CExp_13_3_7Dlg::OnBnClickedBtnSave()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CFileDialog fileDlg(FALSE,
                       "����λͼ",
                        NULL,
                        OFN_HIDEREADONLY | OFN_NOCHANGEDIR,
                        "λͼ�ļ�(*.bmp)|*.bmp||");
    if(fileDlg.DoModal() == IDOK)
    {
        // ��ȡ�ļ�����
        CString strFile = fileDlg.GetPathName();
        // �����ļ�
        m_dib.SaveToFile(strFile);
    }
}

void CExp_13_3_7Dlg::OnBnClickedBtnExit()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CDialog::OnOK();
}

void CExp_13_3_7Dlg::OnDestroy()
{
    CDialog::OnDestroy();

    // TODO: �ڴ˴������Ϣ����������
}

// ��ʾλͼ
void CExp_13_3_7Dlg::showPic(void)
{
    if(m_strFile != "")
    {          
        // ��ȡͼ���豸
        CDC *pPicDC = m_stcPic.GetDC();
        // ��ȡ�豸�ͻ�����С
        CRect rectPic;
        m_stcPic.GetClientRect(rectPic); 
        CSize sizePic(rectPic.Width(),rectPic.Height());
        // ����λͼ
        m_dib.Draw(pPicDC, CPoint(0,0),sizePic);
        // �ͷ�ͼ���豸
        ReleaseDC(pPicDC);
    }
}
