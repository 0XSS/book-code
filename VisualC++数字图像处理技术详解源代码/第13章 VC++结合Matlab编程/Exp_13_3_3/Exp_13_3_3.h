// Exp_13_3_3.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CExp_13_3_3App:
// �йش����ʵ�֣������ Exp_13_3_3.cpp
//

class CExp_13_3_3App : public CWinApp
{
public:
	CExp_13_3_3App();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CExp_13_3_3App theApp;