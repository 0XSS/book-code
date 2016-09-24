// VcExcelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VcExcel.h"
#include "VcExcelDlg.h"

#include "excel.h"

#define LIST_EXCEL_INDEX    0
#define LIST_EXCEL_NAME     1
#define LIST_EXCEL_SEX      2
#define LIST_EXCEL_AGE      3

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
// CVcExcelDlg dialog

CVcExcelDlg::CVcExcelDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CVcExcelDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CVcExcelDlg)
    m_strFilename = _T("");
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVcExcelDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CVcExcelDlg)
    DDX_Control(pDX, IDC_LIST_Excel, m_ctlListExcel);
    DDX_Text(pDX, IDC_EDIT_Filename, m_strFilename);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVcExcelDlg, CDialog)
    //{{AFX_MSG_MAP(CVcExcelDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTONBrowser, OnBUTTONBrowser)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVcExcelDlg message handlers

BOOL CVcExcelDlg::OnInitDialog()
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
    m_ctlListExcel.InsertColumn(LIST_EXCEL_INDEX, "���", LVCFMT_LEFT, 80);
    m_ctlListExcel.InsertColumn(LIST_EXCEL_NAME, "����", LVCFMT_LEFT, 80);
    m_ctlListExcel.InsertColumn(LIST_EXCEL_SEX, "�Ա�", LVCFMT_LEFT, 80);
    m_ctlListExcel.InsertColumn(LIST_EXCEL_AGE, "����", LVCFMT_LEFT, 80);
    m_ctlListExcel.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    if (!AfxOleInit()) {
        AfxMessageBox("COM�����ʼ��ʧ�ܡ�");
        return FALSE;
    }

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVcExcelDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CVcExcelDlg::OnPaint()
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
HCURSOR CVcExcelDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CVcExcelDlg::OnOK()
{
    // TODO: Add extra validation here
    //CDialog::OnOK();
}

void CVcExcelDlg::OnCancel()
{
    // TODO: Add extra cleanup here
    if (MessageBox("ȷʵҪ�˳���?", "�˳�ϵͳ", MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2) == IDOK) {
        CDialog::OnCancel();
    }
}

void CVcExcelDlg::OnBUTTONBrowser()
{
    // TODO: Add your control notification handler code here
    //��Excel�ļ���
    CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                    "Excel Files (*.xls,*.xlsx)|*.xls;*.xlsx|All Files (*.*)|*.*||");

    if (dlg.DoModal() == IDCANCEL)
        return;

    UpdateData(TRUE);
    //0 ����ЩExcel�ļ��У����Դ򿪾����Excel�ļ��ˡ�
    m_strFilename = dlg.GetPathName();
    //1 ����һ���롰Excel�������������ӡ�
    _Application excelApp;//Excel�����������

    if (!excelApp.CreateDispatch("Excel.Application")) {
        MessageBox("�޷�����Excel�������������Ƿ�����æ����ϵͳû�а�װExcel�����");
        return;
    }

    //ʹExcel���ɼ�������ϵͳ���Excel�ļ����������������û�����Excel��
    excelApp.SetVisible(FALSE);
    excelApp.SetUserControl(FALSE);
    //2 �ҵ���Excel�ļ�����һ���ļ���
    //�����ȿ�������Excel�ļ��ϣ�
    //���򿪶��Excel�ļ�ʱ��ϵͳ���Զ��򿪶��Excel������Щ�ļ���
    Workbooks excelFiles;
    LPDISPATCH lpBooks = excelApp.GetWorkbooks();
    excelFiles.AttachDispatch(lpBooks);//��Ӧ�ý�������
    //3 ����ѡ���ļ���
    COleVariant avar((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
    LPDISPATCH lpFiles = excelFiles.Open(m_strFilename,
                                         avar, avar, avar, avar, avar, avar, avar, avar, avar, avar, avar, avar, avar, avar);
    //4 ��ÿһ��Excel�ļ��У��������ǹ������ļ��ϡ�
    _Workbook excelFile;
    excelFile.AttachDispatch(lpFiles);
    //5 ��ǰ�����������С�
    Worksheets excelSheets;
    excelSheets.AttachDispatch(excelFile.GetWorksheets());
    //6 ��������ǰ����������
    _Worksheet excelSheet;
    LPDISPATCH lpSheet = excelFile.GetActiveSheet();
    excelSheet.AttachDispatch(lpSheet);
    //7 Ĭ�ϵĹ��������������У�Ĭ�ϵı�������в�����ʹ�õı��
    Range usedRange;
    usedRange.AttachDispatch(excelSheet.GetUsedRange());
    //8 ����ı��
    Range range;
    range.AttachDispatch(usedRange.GetRows());
    long iNumRow = range.GetCount(); //�Ѿ�ʹ�õ�����
    range.AttachDispatch(usedRange.GetColumns());
    long iNumCol = range.GetCount(); //�Ѿ�ʹ�õ�����
    //9 ��ʼ��ȡ���е����ݡ�
    CString strItem, strLine, strAll;
    m_ctlListExcel.DeleteAllItems();

    for (long i = 2; i <= iNumRow; i++) {
        strLine = "";
        strItem.Format("%d", i);
        m_ctlListExcel.InsertItem(i - 2, strItem);

        for (int j = 1; j <= iNumCol; j++) {
            range.AttachDispatch(excelSheet.GetCells());
            VARIANT va = range.GetItem(COleVariant(long(i)), COleVariant(long(j)));
            range.AttachDispatch(va.pdispVal);
            COleVariant vValue = range.GetValue2();

            switch (vValue.vt) {
            case VT_R8:
                strItem.Format("%d", (int)vValue.dblVal);
                break;

            case VT_BSTR:
                strItem = vValue.bstrVal;
                break;

            case VT_I4:
                strItem.Format("%ld", (int)vValue.lVal);
                break;

            default:
                strItem.Format("�ǳ����ʽ��%d��", vValue.vt);
                break;
            }

            strLine += strItem;
            m_ctlListExcel.SetItemText(i - 2, j - 1, strItem);
        }

        strAll += strLine + "\r\n";
    }

    //MessageBox(strAll);
    //10 �رձ��
    range.ReleaseDispatch();
    usedRange.ReleaseDispatch();
    //11 �رձ���
    excelSheet.ReleaseDispatch();
    excelSheets.ReleaseDispatch();
    //12 �رչ������ļ�
    excelFile.ReleaseDispatch();
    //excelFile.Save();
    excelFile.Close(avar, COleVariant(m_strFilename), avar);
    excelFiles.ReleaseDispatch();
    excelFiles.Close();
    //13 �ر�����Excel��������
    excelApp.ReleaseDispatch();
    excelApp.Quit();
    HWND hwnd =::FindWindow("XLMAIN", NULL);
    ::SendMessage(hwnd, WM_CLOSE, 0, 0);
}
