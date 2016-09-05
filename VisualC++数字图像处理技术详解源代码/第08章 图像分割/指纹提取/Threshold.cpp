#include "StdAfx.h"
#include "Threshold.h"
#include "math.h"

CThreshold::CThreshold()
{
}

CThreshold::CThreshold(CDib *pDib)
{
    m_pDib = pDib;
}

CThreshold::~CThreshold(void)
{
}

//=======================================================
// �������ܣ� ��󷽲���ֵ�ָ�
// ��������� ��
// ����ֵ��   ��
//=======================================================
void CThreshold::OtusThreshold(void)
{
    // ѭ������
	int i, j;

    // ԭͼ������ָ��
    LPBYTE p_data;
    p_data = m_pDib->GetData();

    // ͼ��ÿ��������ռ���ֽ���
    int nLineByte = m_pDib->GetLineByte();

        // ͼ��Ŀ��
    int nWidth = m_pDib->GetWidth();

    // ͼ��ĸ߶�
    int nHeight = m_pDib->GetHeight();

	// �Ҷ�ֱ��ͼ���飬����ʼ��
	int nGrayHistogram[256];
	memset(nGrayHistogram, 0, sizeof(nGrayHistogram));
     
	// ͳ�Ƹ����Ҷȼ���Ӧ�����ظ���������ŵ��Ҷ�ֱ��ͼ������
    int nPixel;
	for (j = 0; j < nHeight; j ++)
	    for (i = 0; i < nWidth; i ++)
	    {
		    // ��ȡ��ǰ���ص�ĻҶ�ֵ
            nPixel = p_data[nLineByte * j + i];	

		    // �ԻҶ�ֵͳ�Ƽ���
		    nGrayHistogram[nPixel] ++;		
	    }

    // c0���c1��ľ�ֵ
    float u0, u1;

    // c0���c1��ĸ���
    float w0, w1;

    // c0�����������
    int nCount0;

    // ��ֵ�������ֵ����Ӧ�������ʱ����ֵ��
    int nT, nBestT;

    // �������󷽲�
    float fVaria, fMaxVaria = 0;

    // ͳ��ֱ��ͼ�����ص������������ŵ�nSum��
    int nSum=0;
    for(i = 0; i < 256; i ++)
        nSum += nGrayHistogram[i];


    // ����ֵnT��0������255
    for(nT = 0; nT < 256; nT ++)
    {
        // ����ֵΪnTʱ������c0��ľ�ֵ�͸���
        u0 = 0;
        nCount0 = 0;
        for(i = 0; i <= nT; i++)
        {
            u0 += i * nGrayHistogram[i];
            nCount0 += nGrayHistogram[i];
        }
        u0 /= nCount0;
        w0 = (float) nCount0 / nSum;

        // ����ֵΪnTʱ������c1��ľ�ֵ�͸���
        u1 = 0;
        for(i = nT+1; i < 256; i ++)
             u1 += i * nGrayHistogram[i];
        u1 /= (nSum - nCount0);
        w1 = 1 - w0;

        // ���������ķ���
        fVaria = w0 * w1 * (u0 - u1) * (u0 - u1);

        // ��¼��󷽲�������ֵ
        if(fVaria > fMaxVaria)
        {
            fMaxVaria = fVaria;
            nBestT = nT;
        }
    }

    // ���������ֵ��ԭͼ�����ָ��
    for(j = 0; j < nHeight; j ++)
        for(i = 0; i < nWidth; i ++)
        {
            if(p_data[j * nLineByte + i] < nBestT)
                p_data[j * nLineByte + i] = 0;
            else
                p_data[j * nLineByte + i] = 255;
        }
}


//=======================================================
// �������ܣ� ����Ӧ��ֵ�ָ�
// ��������� ��
// ����ֵ��   ��
//=======================================================
void CThreshold::AdaptiveThreshold(void)
{
    // ѭ������
    int i,j;
    
    // ԭͼ��������ָ��
    LPBYTE p_data;
    p_data = m_pDib->GetData();

    // ͼ��ÿ��������ռ���ֽ���
    int nLineByte = m_pDib->GetLineByte();

    // ͼ��Ŀ��
    int nWidth = m_pDib->GetWidth();

    // ͼ��ĸ߶�
    int nHeight = m_pDib->GetHeight();

    // �ֲ���ֵ
    int nThreshold[2][2];

    // ��ͼ��ĻҶ�ƽ��ֵ
    int nAvgValue;

    // ������ͼ�����ɨ��,�������ͼ��ĻҶ�ƽ��ֵ
    nAvgValue = 0;
    for(j = nHeight / 2; j < nHeight; j ++)
        for(i = 0; i < nWidth / 2; i ++)
            nAvgValue += p_data[j * nLineByte + i];
    nAvgValue /= ((nHeight / 2) * (nLineByte / 2));

    // ������ֵΪ��ͼ���ƽ��ֵ
    nThreshold[0][0] = nAvgValue;

    // ������ͼ�����ɨ�貢������ֵ�ָ�
    for(j = nHeight / 2; j < nHeight; j ++)
        for(i = 0; i < nWidth / 2; i ++)
        {
            if(p_data[j * nLineByte + i] < nThreshold[0][0])
                p_data[j * nLineByte + i] = 0;
            else
                p_data[j * nLineByte + i] = 255;
        }

    // ������ͼ�����ɨ��,�������ͼ��ĻҶ�ƽ��ֵ
    nAvgValue = 0;
    for(j = nHeight / 2; j < nHeight; j ++)
        for(i = nWidth / 2; i < nWidth; i ++)
            nAvgValue += p_data[j * nLineByte + i];
    nAvgValue /= ((nHeight / 2) * (nLineByte / 2));

    // ������ֵΪ��ͼ���ƽ��ֵ
    nThreshold[0][1] = nAvgValue;

    // ������ͼ�����ɨ�貢������ֵ�ָ�
    for(j = nHeight / 2; j < nHeight; j ++)
        for(i = nWidth / 2; i < nWidth; i ++)
        {
            if(p_data[j * nLineByte + i] < nThreshold[0][0])
                p_data[j * nLineByte + i] = 0;
            else
                p_data[j * nLineByte + i] = 255;
        }

    // ������ͼ�����ɨ��,�������ͼ��ĻҶ�ƽ��ֵ
    nAvgValue = 0;
    for(j = 0; j < nHeight / 2; j ++)
        for(i = 0; i < nWidth / 2; i ++)
            nAvgValue += p_data[j * nLineByte + i];
    nAvgValue /= ((nHeight / 2) * (nLineByte / 2));

    // ������ֵΪ��ͼ���ƽ��ֵ
    nThreshold[1][0] = nAvgValue;

    // ������ͼ�����ɨ�貢������ֵ�ָ�
    for(j = 0; j < nHeight / 2; j ++)
        for(i = 0; i < nWidth / 2; i ++)
        {
            if(p_data[j * nLineByte + i] < nThreshold[0][0])
                p_data[j * nLineByte + i] = 0;
            else
                p_data[j * nLineByte + i] = 255;
        }

    // ������ͼ�����ɨ��,�������ͼ��ĻҶ�ƽ��ֵ
    nAvgValue = 0;
    for(j = 0; j < nHeight / 2; j ++)
        for(i = nWidth / 2; i < nWidth; i ++)
            nAvgValue += p_data[j * nLineByte + i];
    nAvgValue /= ((nHeight / 2) * (nLineByte / 2));

    // ������ֵΪ��ͼ���ƽ��ֵ
    nThreshold[1][1] = nAvgValue;

    // ��������ͼ�����ɨ�貢������ֵ�ָ�
    for(j = 0; j < nHeight / 2; j ++)
        for(i = nWidth / 2; i < nWidth;i ++)
        {
            if(p_data[j * nLineByte + i] < nThreshold[0][0])
                p_data[j * nLineByte + i] = 0;
            else
                p_data[j * nLineByte + i] = 255;
        }
}