// Exp_13_3_3Dlg.h : ͷ�ļ�
//

#pragma once


// CExp_13_3_3Dlg �Ի���
class CExp_13_3_3Dlg : public CDialog
{
// ����
public:
	CExp_13_3_3Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_EXP_13_3_3_DIALOG };

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
    afx_msg void OnBnClickedBtnExecute();
public:
    afx_msg void OnClose();
};
