// DImageProcessDoc.h : interface of the CDImageProcessDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIMAGEPROCESSDOC_H__F29DDACB_D2B5_4B2F_8ECE_B57FFE38B9AA__INCLUDED_)
#define AFX_DIMAGEPROCESSDOC_H__F29DDACB_D2B5_4B2F_8ECE_B57FFE38B9AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Dib.h"
extern CDib m_dib;					//����һ�����õ��ⲿ��Ա����
class CDImageProcessDoc : public CDocument
{
protected: // create from serialization only
	CDImageProcessDoc();
	DECLARE_DYNCREATE(CDImageProcessDoc)

	// Attributes
public:
	CDib m_dib;					//����һ��Dib����

	// Operations
public:
	HGLOBAL GetHObject() const	//��ȡDib����ľ��
	{ return m_hDIB; }
	CPalette* GetDocPal() const	//��ȡ��ɫ��ָ��
	{ return m_palDIB; }
	CSize GetDocDimension() const//��ȡ��ɫ���С
	{ return m_sizeDoc; }
	void UpdateObject(HGLOBAL hDIB);//����dib����

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDImageProcessDoc)
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

	// Implementation
public:
	void SetDib();				//��ʼ��dib����
	virtual ~CDImageProcessDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	HGLOBAL m_hDIB;
	CPalette* m_palDIB;
	CSize m_sizeDoc;
	// Generated message map functions
protected:
	//{{AFX_MSG(CDImageProcessDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIMAGEPROCESSDOC_H__F29DDACB_D2B5_4B2F_8ECE_B57FFE38B9AA__INCLUDED_)
