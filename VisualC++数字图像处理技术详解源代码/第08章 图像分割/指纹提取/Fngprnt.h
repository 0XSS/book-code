// Fngprnt.h : Fngprnt Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CFngprntApp:
// �йش����ʵ�֣������ Fngprnt.cpp
//

class CFngprntApp : public CWinApp
{
public:
	CFngprntApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CFngprntApp theApp;