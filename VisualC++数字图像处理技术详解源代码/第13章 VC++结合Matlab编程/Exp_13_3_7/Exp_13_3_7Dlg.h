// Exp_13_3_7Dlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "dib.h"


// CExp_13_3_7Dlg �Ի���
class CExp_13_3_7Dlg : public CDialog
{
// ����
public:
	CExp_13_3_7Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_EXP_13_3_7_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnOpen();
public:
    afx_msg void OnBnClickedBtnCvt();
public:
    afx_msg void OnBnClickedBtnSave();
public:
    afx_msg void OnBnClickedBtnExit();
public:
    // Ӧ�ó�������·��
    CString m_strAppPath;
public:
    // ͼ���ļ�����
    CString m_strFile;
public:
    // ͼ��ؼ�
    CStatic m_stcPic;
public:
    // λͼ��������
    CDib m_dib;
public:
    afx_msg void OnDestroy();
public:
    // ��ʾλͼ
    void showPic(void);
};
