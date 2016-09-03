//======================================================================
// �ļ��� BinaryMorphDib.h
// ���ݣ� ��ֵ��̬ѧ����������㺯��-ͷ�ļ�
// ���ܣ� ��1����ʴ���㣻
//        ��2���������㣻
//        ��3����������㣻
//        ��4����������������㣻
//        ��5���Ǽ���ȡ�㷨��
// 
//======================================================================

#pragma once

#include "Dib.h"

#include "afx.h"

class CBinaryMorphDib
{
public:
	// ���캯������ʼ�����ݳ�Ա
	CBinaryMorphDib(CDib *pDib);

	// ��������	
	~CBinaryMorphDib(void);

	// ��ʴ���㺯��
	BOOL Erosion(int Structure[3][3]);

	// �������㺯��
	BOOL Dilation(int Structure[3][3]);

	// �����㺯��
	BOOL Opening(int Structure[3][3]);

	// �����㺯��
	BOOL Closing(int Structure[3][3]);

	// ��������������㺯��
	void Hit_Miss_Transform(int hStructure[3][3], int mStructure[3][3]);

	// �Ǽ���ȡ�㷨
	void SkeletonExtraction(int Structure[3][3]);


public:
    // ���ݳ�Ա,CDib�����ָ�� 
	CDib *m_pDib; 

};
