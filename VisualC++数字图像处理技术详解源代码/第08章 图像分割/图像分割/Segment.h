// Segment.h : Segment Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CSegmentApp:
// �йش����ʵ�֣������ Segment.cpp
//

class CSegmentApp : public CWinApp
{
public:
	CSegmentApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSegmentApp theApp;