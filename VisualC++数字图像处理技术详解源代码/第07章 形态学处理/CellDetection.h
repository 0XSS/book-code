// CellDetection.h : CellDetection Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CCellDetectionApp:
// �йش����ʵ�֣������ CellDetection.cpp
//

class CCellDetectionApp : public CWinApp
{
public:
	CCellDetectionApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCellDetectionApp theApp;