#include "StdAfx.h"
#include "EdgeDetection.h"
#include "math.h"

CEdgeDetection::CEdgeDetection(CDib *pDib)
{
    m_pDib = pDib;
}

CEdgeDetection::~CEdgeDetection(void)
{
}

//=======================================================
// �������ܣ� Roberts��Ե�������
// ��������� void
// ����ֵ��   void 
//=======================================================
void CEdgeDetection::Roberts(void)
{
    // ѭ������
	int i, j;

	// ԭͼ��������ָ��
	LPBYTE lpData;
    lpData = m_pDib->GetData();

    // ͼ��ÿ��������ռ���ֽ���
    int nLineByte = m_pDib->GetLineByte();

	// ͼ��Ŀ��
	int nWidth = m_pDib->GetWidth();

	// ͼ��ĸ߶�
	int nHeight = m_pDib->GetHeight();
    
    // ��ͼ�񻺳�����ָ��
	LPBYTE lpTemp; 
    lpTemp = new BYTE[nLineByte * nHeight];

    // ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	memset(lpTemp, 255, nLineByte * nHeight);

    // Roberts����
    int pixel[4]; 

    // ����ʹ��2*2��ģ�壬Ϊ��ֹԽ�磬���������ϱߺ����ұߵı߽�����
    for(j = 0; j < nHeight - 1; j ++)
	    for(i = 0; i < nWidth - 1; i ++)
		{
	        //����Roberts����
			pixel[0] = lpData[j * nLineByte + i];
            pixel[1] = lpData[j * nLineByte + i + 1];
			pixel[2] = lpData[(j + 1) * nLineByte + i];
			pixel[3] = lpData[(j + 1) * nLineByte + i + 1];

			// ����ǰ����
			lpTemp[j * nLineByte + i] = sqrt(double ( (pixel[0] - pixel[3]) * (pixel[0] - pixel[3])
                                    + (pixel[1] - pixel[2]) * (pixel[1] - pixel[2]) ) );
		}

	// ����������ͼ���Ƶ�ԭͼ����
	memcpy(lpData,lpTemp, nLineByte * nHeight);

	// �ͷ��ڴ�
	delete [] lpTemp;
}

//=======================================================
// �������ܣ� �Ҷ�ģ��
// ��������� LPBYTE lpData: ָ��ԭͼ��������ָ�� 
//            int nLineByte: ԭͼ��ÿ��������ռ���ֽ���
//            int nWidth: ԭͼ����                                    
//            int nHeight: ԭͼ��߶�                                  
//            int nTemH: ģ��߶�                                     
//            int nTemW: ģ����                                     
//            int nTemCX: ģ������Ԫ�ص�X����(<nTemW-1)               
//            int nTemCY: ģ�������Ԫ�ص�Y����(<nTemH-1)               
//            float *fpArray: ָ��ģ�������ָ��                      
//            float fCoef��ģ��ϵ��         
// ����ֵ��   void 
//=======================================================
void CEdgeDetection::Template(LPBYTE lpData, int nLineByte, int nWidth, int nHeight, int nTemH, int nTemW, int nTemCX, int nTemCY, float *fpArray, float fCoef)
{   
	// ѭ������
    int i, j, k, l; 

    // ��ͼ�񻺳�����ָ��
	LPBYTE lpTemp; 
    lpTemp = new BYTE[nLineByte * nHeight];

    // ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	memset(lpTemp, 255, nLineByte * nHeight);

    // ����ֵ������
    float fResult; 

	for(j = nTemCY; j < nHeight - nTemH + nTemCY + 1; j ++)
		for(i = nTemCX; i < nWidth - nTemW + nTemCX + 1; i ++)
		{
			// ���㵱ǰ����ֵ
			fResult = 0;

			for(k = 0; k < nTemH; k ++)
				for(l = 0; l < nTemW; l ++)
                    // �������
					fResult += lpData[(j - nTemCY + k) * nLineByte + (i - nTemCX + l)]
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
// �������ܣ� Sobel��Ե�������
// ��������� void
// ����ֵ��   void 
//=======================================================
void CEdgeDetection::Sobel()
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
  
	// ����Sobelģ��һ�Ĳ���
	nTemW = 3;
	nTemH = 3;
	fCoef = 1.0;
	nTemCX = 1;
	nTemCY = 1;
	arTemplate[0] = -1.0;
    arTemplate[1] = -2.0;
	arTemplate[2] = -1.0;
	arTemplate[3] = 0.0;
	arTemplate[4] = 0.0;
	arTemplate[5] = 0.0;
	arTemplate[6] = 1.0;
	arTemplate[7] = 2.0;
	arTemplate[8] = 1.0;

	// ����Template()����
	Template(lpTemp1, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef); 

    // ����Sobelģ����Ĳ���
    arTemplate[0] = 1.0;
    arTemplate[1] = 0.0;
	arTemplate[2] = -1.0;
	arTemplate[3] = 2.0;
	arTemplate[4] = 0.0;
	arTemplate[5] = -2.0;
	arTemplate[6] = 1.0;
	arTemplate[7] = 0.0;
	arTemplate[8] = -1.0;

	// ����Template()����
	Template(lpTemp2, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef);

	// ȡ��������ͼ���Ӧ���ص����ֵ��������ڵ�һ������ͼ����
	for(int j = 0; j < nHeight; j ++)
		for(int i = 0; i < nWidth; i ++)
			if( lpTemp2[j * nLineByte + i] > lpTemp1[j * nLineByte + i] )
				 lpTemp1[j * nLineByte + i] = lpTemp2[j * nLineByte + i];

    // ����һ������ͼ���Ƶ�ԭͼ����
	memcpy(lpData, lpTemp1, nLineByte * nHeight);

	// ɾ��������
	delete  [] lpTemp1;
	delete  [] lpTemp2;
}

//=======================================================
// �������ܣ� Prewitt��Ե�������
// ��������� void
// ����ֵ��   void 
//=======================================================
void CEdgeDetection::Prewitt(void)
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
	Template(lpTemp1, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef); 

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
	Template(lpTemp2, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef);

	// ȡ��������ͼ���Ӧ���ص����ֵ��������ڵ�һ������ͼ����
	for(int j = 0; j < nHeight; j ++)
		for(int i = 0; i < nWidth; i ++)
			if( lpTemp2[j * nLineByte + i] > lpTemp1[j * nLineByte + i] )
				 lpTemp1[j * nLineByte + i] = lpTemp2[j * nLineByte + i];

    // ����һ������ͼ���Ƶ�ԭͼ����
	memcpy(lpData, lpTemp1, nLineByte * nHeight);

	// ɾ��������
	delete  [] lpTemp1;
	delete  [] lpTemp2;
}

//=======================================================
// �������ܣ� Laplacian��Ե�������
// ��������� void
// ����ֵ��   void 
//=======================================================
void CEdgeDetection::Laplacian(void)
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

    // ��ͼ�񻺳�����ָ��
	LPBYTE lpTemp; 
    lpTemp = new BYTE[nLineByte * nHeight];

    // ��ʼ���·�����ڴ棬����ԭͼ��
	memcpy(lpTemp, lpData, nLineByte * nHeight);
  
	// ����Laplacianģ��Ĳ���
	nTemW = 3;
	nTemH = 3;
	fCoef = 1.0;
	nTemCX = 1;
	nTemCY = 1;
	arTemplate[0] = -1.0;
    arTemplate[1] = -1.0;
	arTemplate[2] = -1.0;
	arTemplate[3] = -1.0;
	arTemplate[4] = 8.0;
	arTemplate[5] = -1.0;
	arTemplate[6] = -1.0;
	arTemplate[7] = -1.0;
	arTemplate[8] = -1.0;

	// ����Template()����
	Template(lpTemp, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef); 

    // ������ͼ���Ƶ�ԭͼ����
	memcpy(lpData, lpTemp, nLineByte * nHeight);

	// ɾ��������
	delete  [] lpTemp;
}

//=======================================================
// �������ܣ� GuassLaplacian��Ե�������
// ��������� void
// ����ֵ��   void 
//=======================================================
void CEdgeDetection::GuassLaplacian(void)
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
    float arTemplate[25]; 

    // ��ͼ�񻺳�����ָ��
	LPBYTE lpTemp; 
    lpTemp = new BYTE[nLineByte * nHeight];

    // ��ʼ���·�����ڴ棬����ԭͼ��
	memcpy(lpTemp, lpData, nLineByte * nHeight);
  
	// ����GuassLaplacianģ��Ĳ���
	nTemW = 5;
	nTemH = 5;
	fCoef = 0.25;
	nTemCX = 4;
	nTemCY = 4;
	arTemplate[0] = -2.0;
    arTemplate[1] = -4.0;
	arTemplate[2] = -4.0;
	arTemplate[3] = -4.0;
	arTemplate[4] = -2.0;
	arTemplate[5] = -4.0;
	arTemplate[6] = 0.0;
	arTemplate[7] = 8.0;
	arTemplate[8] = 0.0;
    arTemplate[9] = -4.0;
    arTemplate[10] = -4.0;
	arTemplate[11] = 8.0;
	arTemplate[12] = 24.0;
	arTemplate[13] = 8.0;
	arTemplate[14] = -4.0;
	arTemplate[15] = -4.0;
	arTemplate[16] = 0.0;
	arTemplate[17] = 8.0;
    arTemplate[18] = 0.0;
    arTemplate[19] = -4.0;
	arTemplate[20] = -2.0;
	arTemplate[21] = -4.0;
	arTemplate[22] = -4.0;
	arTemplate[23] = -4.0;
	arTemplate[24] = -2.0;

	// ����Template()����
	Template(lpTemp, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef); 

    // ������ͼ���Ƶ�ԭͼ����
	memcpy(lpData, lpTemp, nLineByte * nHeight);

	// ɾ��������
	delete  [] lpTemp;
}

//=======================================================
// �������ܣ� Krisch��Ե�������
// ��������� void
// ����ֵ��   void 
//=======================================================
void CEdgeDetection::Krisch(void)
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
  
	// ����Krischģ��һ�Ĳ���
	nTemW = 3;
	nTemH = 3;
	fCoef = 0.5;
	nTemCX = 1;
	nTemCY = 1;
	arTemplate[0] = 5.0;
    arTemplate[1] = 5.0;
	arTemplate[2] = 5.0;
	arTemplate[3] = -3.0;
	arTemplate[4] = 0.0;
	arTemplate[5] = -3.0;
	arTemplate[6] = -3.0;
	arTemplate[7] = -3.0;
	arTemplate[8] = -3.0;

	// ����Template()����
	Template(lpTemp1, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef); 

    // ����Krischģ����Ĳ���
    arTemplate[0] = -3.0;
    arTemplate[1] = 5.0;
	arTemplate[2] = 5.0;
	arTemplate[3] = -3.0;
	arTemplate[4] = 0.0;
	arTemplate[5] = 5.0;
	arTemplate[6] = -3.0;
	arTemplate[7] = -3.0;
	arTemplate[8] = -3.0;

	// ����Template()����
	Template(lpTemp2, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef);

	// ȡ��������ͼ���Ӧ���ص����ֵ��������ڵ�һ������ͼ����
	for(int j = 0; j < nHeight; j ++)
		for(int i = 0; i < nWidth; i ++)
			if( lpTemp2[j * nLineByte + i] > lpTemp1[j * nLineByte + i] )
				 lpTemp1[j * nLineByte + i] = lpTemp2[j * nLineByte + i];

    // ��ԭͼ���Ƶ��ڶ�������ͼ����
	memcpy(lpTemp2, lpData, nLineByte * nHeight);

    // ����Krischģ�����Ĳ���
    arTemplate[0] = -3.0;
    arTemplate[1] = -3.0;
	arTemplate[2] = 5.0;
	arTemplate[3] = -3.0;
	arTemplate[4] = 0.0;
	arTemplate[5] = 5.0;
	arTemplate[6] = -3.0;
	arTemplate[7] = -3.0;
	arTemplate[8] = 5.0;

	// ����Template()����
	Template(lpTemp2, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef);

	// ȡ��������ͼ���Ӧ���ص����ֵ��������ڵ�һ������ͼ����
	for(int j = 0; j < nHeight; j ++)
		for(int i = 0; i < nWidth; i ++)
			if( lpTemp2[j * nLineByte + i] > lpTemp1[j * nLineByte + i] )
				 lpTemp1[j * nLineByte + i] = lpTemp2[j * nLineByte + i];

    // ��ԭͼ���Ƶ��ڶ�������ͼ����
	memcpy(lpTemp2, lpData, nLineByte * nHeight);

    // ����Krischģ���ĵĲ���
    arTemplate[0] = -3.0;
    arTemplate[1] = -3.0;
	arTemplate[2] = -3.0;
	arTemplate[3] = -3.0;
	arTemplate[4] = 0.0;
	arTemplate[5] = 5.0;
	arTemplate[6] = -3.0;
	arTemplate[7] = 5.0;
	arTemplate[8] = 5.0;

	// ����Template()����
	Template(lpTemp2, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef);

	// ȡ��������ͼ���Ӧ���ص����ֵ��������ڵ�һ������ͼ����
	for(int j = 0; j < nHeight; j ++)
		for(int i = 0; i < nWidth; i ++)
			if( lpTemp2[j * nLineByte + i] > lpTemp1[j * nLineByte + i] )
				 lpTemp1[j * nLineByte + i] = lpTemp2[j * nLineByte + i];

    // ��ԭͼ���Ƶ��ڶ�������ͼ����
	memcpy(lpTemp2, lpData, nLineByte * nHeight);

    // ����Krischģ����Ĳ���
    arTemplate[0] = -3.0;
    arTemplate[1] = -3.0;
	arTemplate[2] = -3.0;
	arTemplate[3] = -3.0;
	arTemplate[4] = 0.0;
	arTemplate[5] = -3.0;
	arTemplate[6] = 5.0;
	arTemplate[7] = 5.0;
	arTemplate[8] = 5.0;

	// ����Template()����
	Template(lpTemp2, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef);

	// ȡ��������ͼ���Ӧ���ص����ֵ��������ڵ�һ������ͼ����
	for(int j = 0; j < nHeight; j ++)
		for(int i = 0; i < nWidth; i ++)
		    if( lpTemp2[j * nLineByte + i] > lpTemp1[j * nLineByte + i] )
				 lpTemp1[j * nLineByte + i] = lpTemp2[j * nLineByte + i];

    // ��ԭͼ���Ƶ��ڶ�������ͼ����
	memcpy(lpTemp2, lpData, nLineByte * nHeight);

    // ����Krischģ�����Ĳ���
    arTemplate[0] = -3.0;
    arTemplate[1] = -3.0;
	arTemplate[2] = -3.0;
	arTemplate[3] = 5.0;
	arTemplate[4] = 0.0;
	arTemplate[5] = -3.0;
	arTemplate[6] = 5.0;
	arTemplate[7] = 5.0;
	arTemplate[8] = -3.0;

	// ����Template()����
	Template(lpTemp2, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef);

	// ȡ��������ͼ���Ӧ���ص����ֵ��������ڵ�һ������ͼ����
	for(int j = 0; j < nHeight; j ++)
		for(int i = 0; i < nWidth; i ++)
			if( lpTemp2[j * nLineByte + i] > lpTemp1[j * nLineByte + i] )
				 lpTemp1[j * nLineByte + i] = lpTemp2[j * nLineByte + i];

     // ��ԭͼ���Ƶ��ڶ�������ͼ����
	memcpy(lpTemp2, lpData, nLineByte * nHeight);

    // ����Krischģ���ߵĲ���
    arTemplate[0] = 5.0;
    arTemplate[1] = -3.0;
	arTemplate[2] = -3.0;
	arTemplate[3] = 5.0;
	arTemplate[4] = 0.0;
	arTemplate[5] = -3.0;
	arTemplate[6] = 5.0;
	arTemplate[7] = -3.0;
	arTemplate[8] = -3.0;

	// ����Template()����
	Template(lpTemp2, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef);

	// ȡ��������ͼ���Ӧ���ص����ֵ��������ڵ�һ������ͼ����
	for(int j = 0; j < nHeight; j ++)
		for(int i = 0; i < nWidth; i ++)
			if( lpTemp2[j * nLineByte + i] > lpTemp1[j * nLineByte + i] )
				 lpTemp1[j * nLineByte + i] = lpTemp2[j * nLineByte + i];

     // ��ԭͼ���Ƶ��ڶ�������ͼ����
	memcpy(lpTemp2, lpData, nLineByte * nHeight);

    // ����Krischģ��˵Ĳ���
    arTemplate[0] = 5.0;
    arTemplate[1] = 5.0;
	arTemplate[2] = -3.0;
	arTemplate[3] = 5.0;
	arTemplate[4] = 0.0;
	arTemplate[5] = -3.0;
	arTemplate[6] = -3.0;
	arTemplate[7] = -3.0;
	arTemplate[8] = -3.0;

	// ����Template()����
	Template(lpTemp2, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef);

	// ȡ��������ͼ���Ӧ���ص����ֵ��������ڵ�һ������ͼ����
	for(int j = 0; j < nHeight; j ++)
		for(int i = 0; i < nWidth; i ++)
			if( lpTemp2[j * nLineByte + i] > lpTemp1[j * nLineByte + i] )
				 lpTemp1[j * nLineByte + i] = lpTemp2[j * nLineByte + i];

    // ����һ������ͼ���Ƶ�ԭͼ����
	memcpy(lpData, lpTemp1, nLineByte * nHeight);

	// ɾ��������
	delete  [] lpTemp1;
	delete  [] lpTemp2;
}