#if !defined(AFX_DLGIMAGEMATCHGYH_H__E98CA2E0_F9C4_4525_9B13_477F299BDC1E__INCLUDED_)
#define AFX_DLGIMAGEMATCHGYH_H__E98CA2E0_F9C4_4525_9B13_477F299BDC1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgImageMatchGYH.h : header file
//
#include "CDib.h"
#include "ImageMatchingDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgImageMatchGYH dialog

class CDlgImageMatchGYH : public CDialog
{
// Construction
public:
	BOOL ImageMatchGYH(CDib* pDibSrc, CDib* pDibTemplate);    // ģ��ƥ��
	void CalImageLocation();    // ����ͼ��ȿؼ���λ�ô�С
	CRect m_rectResltImage;     // ƥ���ͼ����ʾ����
	CRect m_rectModelImage;     // ģ��ͼ����ʾ����
	CRect m_rectInitImage;      // ��ʼͼ����ʾ����
	CImageMatchingDoc* m_pDoc;  // �ĵ���ָ��
	CDib* m_pDibResult;         // ƥ����ͼ��
	CDib* m_pDibModel;          // ģ��ͼ��
	CDib* m_pDibInit;           // ��ʼͼ��
	BOOL m_bCalImgLoc;          // ����ͼ��λ�õı�־λ��FALSE��ʾ��û�м���ͼ��λ��
	CDlgImageMatchGYH(CWnd* pParent = NULL, CImageMatchingDoc* pDoc = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgImageMatchGYH)
	enum { IDD = IDD_DIALOG_GYH };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgImageMatchGYH)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgImageMatchGYH)
	afx_msg void OnOpentemplate();
	afx_msg void OnImagematch();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIMAGEMATCHGYH_H__E98CA2E0_F9C4_4525_9B13_477F299BDC1E__INCLUDED_)
