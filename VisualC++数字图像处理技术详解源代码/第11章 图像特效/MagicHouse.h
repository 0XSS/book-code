// MagicHouse.h : MagicHouse Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CMagicHouseApp:
// �йش����ʵ�֣������ MagicHouse.cpp
//

class CMagicHouseApp : public CWinApp
{
public:
	CMagicHouseApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

private:
	GdiplusStartupInput m_GdiplusStartupInput;
	ULONG_PTR			m_GdiplusToken;
public:
	virtual int ExitInstance();
};

extern CMagicHouseApp theApp;