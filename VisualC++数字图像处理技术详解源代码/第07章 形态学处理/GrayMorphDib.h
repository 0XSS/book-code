//======================================================================
// �ļ��� GrayMorphDib.h
// ���ݣ� �Ҷ���̬ѧ����������㺯��-ͷ�ļ�
// ���ܣ� ��1����ʴ���㣻
//        ��2���������㣻
//        ��3����������㣻
//        ��4���Ҷ���̬ѧ�ݶȣ�
//        ��5��Top-Hat�任��
//
//======================================================================

#pragma once

#include "Dib.h"

#include "afx.h"


class CGrayMorphDib
{
public:
	// ���캯������ʼ�����ݳ�Ա
	CGrayMorphDib(CDib *pDib);

	// ��������
	~CGrayMorphDib(void);

	// ��ʴ���㺯��
	void Erosion(int Structure[3][3]);

	// �������㺯��
	void Dilation(int Structure[3][3]);

	// �����㺯��
	void Opening(int Structure[3][3]);

	// �����㺯��
	void Closing(int Structure[3][3]);

	// �Ҷ���̬ѧ�ݶ��㷨
	void Grads(int Structure[3][3]);

	// Top-Hat�任
	void Top_Hat(int Structure[3][3]);


private:
    // ���ݳ�Ա,CDib�����ָ�� 
	CDib *m_pDib; 
};
