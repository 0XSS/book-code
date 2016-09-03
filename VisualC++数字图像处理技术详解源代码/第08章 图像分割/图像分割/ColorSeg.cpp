#include "StdAfx.h"
#include "ColorSeg.h"
#include "math.h"

CColorSeg::CColorSeg(CDib *pDib)
{
    m_pDib = pDib;
}

CColorSeg::~CColorSeg(void)
{
}

//=======================================================
// �������ܣ� ��ɫģ��
// ��������� LPBYTE lpData: ָ��ԭͼ��������ָ�� 
//            int nLineByte: ԭͼ��ÿ��������ռ���ֽ���                                   
//            int nHeight: ԭͼ��߶�                                  
//            int nTemH: ģ��߶�                                     
//            int nTemW: ģ����                                     
//            int nTemCX: ģ������Ԫ�ص�X����(<nTemW-1)               
//            int nTemCY: ģ�������Ԫ�ص�Y����(<nTemH-1)               
//            float *fpArray: ָ��ģ�������ָ��                      
//            float fCoef��ģ��ϵ��         
// ����ֵ��   void 
//=======================================================
void CColorSeg::Template24bit(LPBYTE lpData, int nLineByte, int nWidth, int nHeight, int nTemH, int nTemW, int nTemCX, int nTemCY, float *fpArray, float fCoef)
{   
	// ѭ������
    int i, j, k, l; 

    // ��ͼ�񻺳�����ָ��
	LPBYTE lpTemp; 
    lpTemp = new BYTE[nLineByte * nHeight];

    // ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	memcpy(lpTemp, lpData, nLineByte * nHeight);

    // ����ֵ������
    float fResult; 

	for(j = nTemCY; j < nHeight - nTemH + nTemCY + 1; j ++)
		for(i = 3 * nTemCX; i < nLineByte - 3*nTemW + 3*nTemCX + 1; i ++)
		{
			// ���㵱ǰ����ֵ
			fResult = 0;

			for(k = 0; k < nTemH; k ++)
				for(l = 0; l < nTemW; l ++)
                    // �������
					fResult += lpData[(j - nTemCY + k) * nLineByte + (i - 3 * nTemCX + 3 * l)]
                             * fpArray[k * nTemW + l];
			
            //��ϵ�����Ե�ǰ����ֵ������
			fResult *= fCoef;

			//ȡ����ֵ
			fResult = (float) fabs(fResult);

			// �ж��Ƿ񳬹�255
			if(fResult > 255)
				
                // ������255������ͼ��ǰ����λ��ֱ�Ӹ�ֵΪ255
                lpTemp[j * nLineByte + i] = 255;
			else
				
                // δ����255������ͼ��ǰ����λ�ø�ֵΪ������
				lpTemp[j * nLineByte + i] = (int) (fResult + 0.5);
		}

    // ����������ͼ���Ƶ�ԭͼ����
	memcpy(lpData,lpTemp, nLineByte * nHeight);

	// �ͷ��ڴ�
	delete [] lpTemp;
}

//=======================================================
// �������ܣ� Prewitt��Ե�������
// ��������� void
// ����ֵ��   void 
//=======================================================
void CColorSeg::Prewitt(void)
{
    // ԭͼ��������ָ��
	LPBYTE lpData;
    lpData = m_pDib->GetData();

    // ͼ��ÿ��������ռ���ֽ���
    int nLineByte = m_pDib->GetLineByte();

    // ͼ��Ŀ��
	int nWidth = m_pDib->GetWidth();

	// ͼ��ĸ߶�
	int nHeight = m_pDib->GetHeight();
       
    // ģ���Ⱥ͸߶�
    int nTemW, nTemH; 

    // ģ��ϵ��
	float fCoef; 

    //ģ������Ԫ�ص�X�����Y����
	int nTemCX, nTemCY;  

	//ģ������
    float arTemplate[9]; 

    // ������ͼ�񻺳�����ָ��
	LPBYTE lpTemp1, lpTemp2; 
    lpTemp1 = new BYTE[nLineByte * nHeight];
    lpTemp2 = new BYTE[nLineByte * nHeight];

    // ��ʼ���·�����ڴ棬����ԭͼ��
	memcpy(lpTemp1, lpData, nLineByte * nHeight);
    memcpy(lpTemp2, lpData, nLineByte * nHeight);
  
	// ����Prewittģ��һ�Ĳ���
	nTemW = 3;
	nTemH = 3;
	fCoef = 1.0;
	nTemCX = 1;
	nTemCY = 1;
	arTemplate[0] = -1.0;
    arTemplate[1] = -1.0;
	arTemplate[2] = -1.0;
	arTemplate[3] = 0.0;
	arTemplate[4] = 0.0;
	arTemplate[5] = 0.0;
	arTemplate[6] = 1.0;
	arTemplate[7] = 1.0;
	arTemplate[8] = 1.0;

	// ����Template()����
	Template24bit(lpTemp1, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef); 

    // ����Prewittģ����Ĳ���
    arTemplate[0] = 1.0;
    arTemplate[1] = 0.0;
	arTemplate[2] = -1.0;
	arTemplate[3] = 1.0;
	arTemplate[4] = 0.0;
	arTemplate[5] = -1.0;
	arTemplate[6] = 1.0;
	arTemplate[7] = 0.0;
	arTemplate[8] = -1.0;

	// ����Template()����
	Template24bit(lpTemp2, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef);

	// ȡ��������ͼ���Ӧ���ص����ֵ��������ڵ�һ������ͼ����
	for(int j = 0; j < nHeight; j ++)
		for(int i = 0; i < nLineByte; i ++)
			if( lpTemp2[j * nLineByte + i] > lpTemp1[j * nLineByte + i] )
				 lpTemp1[j * nLineByte + i] = lpTemp2[j * nLineByte + i];

    // ����һ������ͼ���Ƶ�ԭͼ����
	memcpy(lpData, lpTemp1, nLineByte * nHeight);

	// ɾ��������
	delete  [] lpTemp1;
	delete  [] lpTemp2;	
}
