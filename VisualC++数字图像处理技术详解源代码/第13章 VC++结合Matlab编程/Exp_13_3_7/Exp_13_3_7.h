// Exp_13_3_7.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CExp_13_3_7App:
// �йش����ʵ�֣������ Exp_13_3_7.cpp
//

class CExp_13_3_7App : public CWinApp
{
public:
	CExp_13_3_7App();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CExp_13_3_7App theApp;