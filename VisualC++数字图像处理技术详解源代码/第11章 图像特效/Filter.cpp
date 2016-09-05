#include "StdAfx.h"
#include "Filter.h"
#include "improve.h"

//-----------------------------------------
//	����:			ͼ��ɫ����
//	����:		
//		pixel		ԭʼͼ�����������
//		tempPixel	���ͼ�����������
//		width		ԭʼͼ����
//		height		ԭʼͼ��߶�
//-----------------------------------------
void Negative(BYTE* pixel, BYTE* tempPixel, int width, int height)
{
	//������������ĳ���
	int sum = width * height * 4;
	memcpy(pixel, tempPixel, sum);

	for(int i = 0; i < sum; i += 4) 
	{
		// ������ֵȡ��
		tempPixel[i]	= 255 - tempPixel[i];	//blue
		tempPixel[i+1]	= 255 - tempPixel[i+1];	//green
		tempPixel[i+2]	= 255 - tempPixel[i+2];	//red
	}
}

//-----------------------------------------
//	����:			ʵ��ͼ��ĸ���Ч��
//	����:		
//		pixel		ԭʼͼ�����������
//		tempPixel	���ͼ�����������
//		width		ԭʼͼ����
//		height		ԭʼͼ��߶�
//-----------------------------------------
void Emboss(BYTE* pixel, BYTE* tempPixel, int width, int height)
{
	//������������ĳ���
	int sum = width * height *4;
	memcpy(tempPixel, pixel, sum);		

	int r = 0, g = 0, b = 0;
	for(int i = 0; i < height-1; i++) 
	{
		for (int j = 0; j < (width-1)*4; j+=4) 
		{
			//��������ֵ
			b = abs(tempPixel[i*width*4+j] 
					-tempPixel[(i+1)*width*4+j+4]+128);
			g = abs(tempPixel[i*width*4+j+1]
					-tempPixel[(i+1)*width*4+j+5]+128);
			r = abs(tempPixel[i*width*4+j+2]
					-tempPixel[(i+1)*width*4+j+6]+128);

			//����Խ�������ֵ���д���
			if (r>255)
				r=255;

			if (g>255)
				g=255;

			if (b>255)
				b=255;

			tempPixel[i*width*4 + j]		= b;//blue
			tempPixel[i*width*4 + j + 1]	= g;//green
			tempPixel[i*width*4 + j + 2]	= r;//red
		}
	}

	for (int k = width * 4 * (height-1); k < width*4*height; k += 4) 
	{
		tempPixel[k]=128;
		tempPixel[k+1]=128;
		tempPixel[k+2]=128;
	}

	for (int l = (width-1) * 4; l < width*4*height; l += width*4) 
	{
		tempPixel[l]=128;
		tempPixel[l+1]=128;
		tempPixel[l+2]=128;
	}
}

//-----------------------------------------
//	����:			ʵ��ͼ��ĺڰ�Ч��
//	����:		
//		pixel		ԭʼͼ�����������
//		tempPixel	���ͼ�����������
//		width		ԭʼͼ����
//		height		ԭʼͼ��߶�
//-----------------------------------------
void ColorToBW(BYTE* pixel, BYTE* tempPixel, int width, int height)
{
	//������������ĳ���
	int sum = width * height * 4;

	memcpy(tempPixel, pixel, sum);

	for(int i = 0; i < sum; i += 4)
	{
		//ƽ��ֵ��
		tempPixel[i]	 = (tempPixel[i] + tempPixel[i+1] + tempPixel[i+2]) / 3; //blue
		tempPixel[i + 1] = tempPixel[i];										 //green
		tempPixel[i + 2] = tempPixel[i];										 //red

		//���ֵ��
		/*
		tempPixel[i] = tempPixel[i] > tempPixel[i+1]? 
							  tempPixel[i] : tempPixel[i+1];
		tempPixel[i] = tempPixel[i] >tempPixel[i+2]?
							  tempPixel[i] : tempPixel[i+2];

		tempPixel[i+1] = tempPixel[i] ;
		tempPixel[i+2] = tempPixel[i];
		*/

		//��Ȩƽ��ֵ��
		/*
		tempPixel[i] =( ((int) (tempPixel[i]*0.5)) + 
							   ((int) (tempPixel[i+1]*0.3)) + 
							   ((int) (tempPixel[i+2]*0.2)) ;

		tempPixel[i + 1] = tempPixel[i];										 //green
		tempPixel[i + 2] = tempPixel[i];		
		*/
	}
}

//-----------------------------------------
//	����:			ʵ��ˮƽ��ʽ����Ч��
//	����:		
//		pixel		ԭʼͼ�����������
//		tempPixel	���ͼ�����������
//		width		ԭʼͼ����
//		height		ԭʼͼ��߶�
//		f			������
//-----------------------------------------
void HorFog(BYTE* pixel, BYTE* tempPixel, int width, int height, int f)
{
	int k;
	int n;
	for(int i = 0; i < height; i++) 
	{
		for (int j = 0; j < width*4; j += 4) 
		{
			k = abs(rand() % f);
			n = j + k*4;

			if (n>(width-1) * 4)
				n = (width-1)*4;

			tempPixel[i*width*4 + j]     = pixel[i*width*4 + n];
			tempPixel[i*width*4 + j + 1] = pixel[i*width*4 + n + 1];
			tempPixel[i*width*4 + j + 2] = pixel[i*width*4 + n + 2];
			tempPixel[i*width*4 + j + 3] = pixel[i*width*4 + n + 3];
		}
	}
}

//-----------------------------------------
//	����:			ʵ�ִ�ֱ��ʽ����Ч��
//	����:		
//		pixel		ԭʼͼ�����������
//		tempPixel	���ͼ�����������
//		width		ԭʼͼ����
//		height		ԭʼͼ��߶�
//		f			������
//-----------------------------------------
void VerFog(BYTE* pixel, BYTE* tempPixel, int width, int height, int f)
{
	int k;
	int m;
	for(int i = 0; i < height; i++) 
	{
		for (int j = 0; j < width*4; j += 4) 
		{
			k = abs(rand() % f);
			m = i + k;
			if (m>height-1)
				m = height-1;

			tempPixel[i*width*4 + j]     = pixel[m*width*4 + j];
			tempPixel[i*width*4 + j + 1] = pixel[m*width*4 + j + 1];
			tempPixel[i*width*4 + j + 2] = pixel[m*width*4 + j + 2];
			tempPixel[i*width*4 + j + 3] = pixel[m*width*4 + j + 3];
		}
	}
}

//-----------------------------------------
//	����:			ʵ�ָ��Ϸ�ʽ����Ч��
//	����:		
//		pixel		ԭʼͼ�����������
//		tempPixel	���ͼ�����������
//		width		ԭʼͼ����
//		height		ԭʼͼ��߶�
//		f			������
//-----------------------------------------
void ComFog(BYTE* pixel, BYTE* tempPixel, int width, int height, int f)
{
	int k;
	int m, n;
	for(int i = 0; i < height; i++) 
	{
		for (int j = 0; j < width*4; j += 4) 
		{
			k = abs(rand() % f);
			m = i + k;
			n = j + k * 4;

			// �Գ���ͼ������ĵ������Ӧ����
			if (m > height-1) 
				m = height-1;

			if (n > (width-1) * 4) 
				n = (width-1) * 4;

			// ������������
			tempPixel[i*width*4 + j]     = pixel[m*width*4 + n];
			tempPixel[i*width*4 + j + 1] = pixel[m*width*4 + n + 1];
			tempPixel[i*width*4 + j + 2] = pixel[m*width*4 + n + 2];
			tempPixel[i*width*4 + j + 3] = pixel[m*width*4 + n + 3];
		}
	}
}

//-----------------------------------------
//	����:		�����δ洢������ת��Ϊ��ά������ʽ
//	����:		
//		image	���δ洢������
//		width	ͼ��Ŀ��
//		height	ͼ��ĸ߶�
//-----------------------------------------
static BYTE** CreatImage(BYTE* image, unsigned int width, unsigned int height)
{
	BYTE** imageBuf = (BYTE**)malloc(sizeof(BYTE*) * (height));
	for(unsigned int y = 0; y < height; y++)
	{
		//ʹimageBuf��ÿ��ָ��ֱ�ָ�����±��ʾ���е����׵�ַ
		imageBuf[y] = image + y * width * 4;
	}

	return imageBuf;
}

//-----------------------------------------
//	����:			ʵ��ͼ���������Ч��
//	����:		
//		pixel		ԭʼͼ�����������
//		tempPixel	���ͼ�����������
//		w			ԭʼͼ����
//		h			ԭʼͼ��߶�
//		size		�����˷���Ĵ�С
//-----------------------------------------
void Mosaic(BYTE* pixel, BYTE* tempPixel, unsigned int w, 
			unsigned int h, unsigned int size)
{
	BYTE** imageBuf0 = CreatImage(pixel, w, h);
	BYTE** imageBuf1 = CreatImage(tempPixel, w, h);
	int x = 0,y = 0;        //�����˷�������Ͻ�����
	int tx = 0,ty = 0;      //�����˷�������½�����
	int i, j;
	while(true)
	{
		if(tx == w)  //����Ѿ�������һ��
		{
			if(ty == h) break;

			//ȷ�������˷�����Ϸ��߽�
			y = ty;  

			//ȷ�������˷�����·��߽�
			if(y + size <= h)
				ty = y + size;
			else
				ty = h;

			//�ѷ���λ���ƶ�����һ���ײ�
			x = 0;
		}

		//ȷ�������˷������߽߱�
		if(x + size <= w)
			tx = x + size;
		else
			tx = w;

		for(i = y;i < ty; i++)
			for(j = x; j<tx; j++)
			{
				//�������˷������Ϊ�е������
				imageBuf1[i][j*4]	  = imageBuf0[(y+ty) / 2][(x+tx)/2*4];
				imageBuf1[i][j*4 + 1] = imageBuf0[(y+ty) / 2][(x+tx)/2*4 + 1];
				imageBuf1[i][j*4 + 2] = imageBuf0[(y+ty) / 2][(x+tx)/2*4 + 2];
				imageBuf1[i][j*4 + 3] = imageBuf0[(y+ty) / 2][(x+tx)/2*4 + 3];
			}
			//ˮƽ�ƶ�����һ�������˷���
			x = tx;
	}

	free(imageBuf0);
	free(imageBuf1);
}


//-----------------------------------------
//ģ��ľ������
//-----------------------------------------
int TempltExcute(BYTE** imageBuf0, int w, int h, int* templt, int tw, int x, int y)
{
	int i,j;
	int m=0,px,py;
	//�ֱ��ģ����ÿ��λ�ý��м���
	for(i=0; i<tw; i++)
		for(j=0; j<tw; j++)
		{
			//����������ԭͼ���ϵ�λ��
			py=y-tw/2+i;
			px=x-tw/2+j;
			//��ÿ�����صĻҶȳ���Ȩֵ�����
			m+=GetAsh(imageBuf0,px,py) * templt[i*tw+j];
		}
		return m;
}


//-----------------------------------------
// ��a��ʾ�ĻҶ����ָ������
//-----------------------------------------
void SetPixel2(BYTE** imageBuf1, int x, int y, int a)
{
	imageBuf1[y][x*4] = a;
	imageBuf1[y][x*4+1] = a;
	imageBuf1[y][x*4+2] = a;
	imageBuf1[y][x*4+3]= 255;
}

//-----------------------------------------
//	����:			ʵ��ͼ�������Ч��
//	����:		
//		pixel		ԭʼͼ�����������
//		tempPixel	���ͼ�����������
//		w			ԭʼͼ����
//		h			ԭʼͼ��߶�
//		size		�����˷���Ĵ�С
//-----------------------------------------
void LaplacianB(BYTE* pixel, BYTE* tempPixel, int w, int h)
{
	//������ʱͼ��洢�ռ�
	BYTE* tempImage;

	tempImage = (BYTE*)malloc(sizeof(BYTE)*w*h*4);

	//��ͼ��ת��Ϊ������ʽ
	BYTE** imageBuf0 = CreatImage(pixel, w, h);
	BYTE** imageBuf1 = CreatImage(tempPixel, w, h);
	BYTE** tempImageBuf = CreatImage(tempImage, w, h);
	double scale = 2;
	//������˹����ģ��
	int templt[9]={ 1, 1, 1, 1,-8, 1, 1, 1, 1 };
	//�������ģ��
	int templtTest1[9]={ 1, 1,-1, 1, 0,-1, 1,-1,-1 };
	int templtTest2[9]={ 1, 1, 1,-1, 0, 1,-1,-1,-1 };
	//ģ������ģ��
	int templtAve[9]={ 1, 1, 1, 1, 4, 1, 1, 1, 1};
	int x,y;
	int a,b,b1,b2;

	//���ζ�ԭͼ���ÿ�����ؽ��д���
	for(y = 1; y < h - 1; y++)
		for(x = 1; x < w - 1; x++)
		{
			//������˹�������
			a=TempltExcute(imageBuf0, w, h, templt, 3, x, y);
			//�������
			b1=abs(TempltExcute(imageBuf0, w, h, templtTest1, 3, x, y));
			b2=abs(TempltExcute(imageBuf0, w, h, templtTest2, 3, x, y));
			b=b1>b2?b1:b2;
			if(b<25) a=0;
			else
			{
				a = (int)(a * scale);
				//���޴���
				if(a > 255) a = 255;
				else if(a < 32) a=0;
			}
			//��ɫ����
			a=255-a;
			SetPixel2(tempImageBuf, x, y, a);
		}

		//ģ������
		for(y = 1; y < h - 1; y++)
			for(x = 1; x < w - 1; x++)
			{
				a=TempltExcute(tempImageBuf, w, h, templtAve, 3, x, y) / 12;
				SetPixel2(imageBuf1, x, y, a);
			}

	free(tempImage);
	free(imageBuf0);
	free(imageBuf1);
	free(tempImageBuf);
}