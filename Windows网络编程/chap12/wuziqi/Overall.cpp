// Overall.cpp: implementation of the Overall class.
//
//////////////////////////////////////////////////////////////////////
//�������̵���

#include "stdafx.h"
#include "wuziqi.h"
#include "Overall.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Overall::Overall()
{//��ʼ��Ϊ��
	int nCount1,nCount2;
	for(nCount1=0;nCount1<=17;nCount1++)
		for(nCount2=0;nCount2<=17;nCount2++)
		{
			allqipan[nCount1][nCount2]=qipantemp[nCount1][nCount2]=0;
		}
}

Overall::~Overall()
{

}
void Overall::cleartemp()
{//�����ߵ��������
	int nCount1,nCount2;
	for(nCount1=0;nCount1<=17;nCount1++)
		for(nCount2=0;nCount2<=17;nCount2++)
		{
			qipantemp[nCount1][nCount2]=0;
		}
}


void Overall::clear()
{//�����е��������
	int nCount1,nCount2;
	for(nCount1=0;nCount1<=17;nCount1++)
		for(nCount2=0;nCount2<=17;nCount2++)
		{
			allqipan[nCount1][nCount2]=0;
		}
}

//����ʽ�����жϣ�˫����˫�Ķ�ʹ����
//�������壺����һ����Ϊ����
//���Ķ��壺�ж���һ�ֵķ�����Ϊ���
//���Ķ��壺ֻ��һ���ܹ���Ϊ���
//ͬʱ�γ��������ϵĻ��������ģ����߳������ǽ���
int Overall::winchanglian(int x,int y)
{//�ж��ǲ��ǳ�������
	int nCountx,nCounty,nColor,nCount;
	nCount=0;
	nColor=allqipan[x][y];
	//heng
	for(nCountx=x;allqipan[nCountx][y]==nColor&&nCountx>=0;nCountx--,nCount++);
	for(nCountx=x+1;allqipan[nCountx][y]==nColor&&nCountx<=17;nCountx++,nCount++);
	if(nCount==5)		return 1;
	if(nCount>5)
	{
		return 0;
	}

	//shu
	nCount=0;
	for(nCounty=y;allqipan[x][nCounty]==nColor&&nCounty>=0;nCounty--,nCount++);
	for(nCounty=y+1;allqipan[x][nCounty]==nColor&&nCounty<=17;nCounty++,nCount++);
	if(nCount==5)		return 1;
	if(nCount>5)
	{
		return 0;
	}

	//zuoyou
	nCount=0;
	for(nCountx=x,nCounty=y;allqipan[nCountx][nCounty]==nColor&&nCountx>=0&&nCounty>=0;nCountx--,nCounty--,nCount++);
	for(nCountx=x+1,nCounty=y+1;allqipan[nCountx][nCounty]==nColor&&nCountx<=17&&nCounty<=17;nCountx++,nCounty++,nCount++);
	if(nCount==5)		return 1;
	if(nCount>5)
	{
		return 0;
	}

	//youzuo
	nCount=0;
	for(nCountx=x,nCounty=y;allqipan[nCountx][nCounty]==nColor&&nCountx>=0&&nCounty>=0;nCountx--,nCounty++,nCount++);
	for(nCountx=x+1,nCounty=y-1;allqipan[nCountx][nCounty]==nColor&&nCountx<=17&&nCounty<=17;nCountx++,nCounty--,nCount++);
	
	if(nCount>5)
	{
		return 0;
	}

	if(nCount==5)		return 1;
	return -1;
}
int Overall::huosi(int x,int y)
{//�ж��ǲ��ǻ���
	int nCountx,nCounty,nColor,nCount;
	nCount=0;
	nColor=allqipan[x][y];
	//heng
	for(nCountx=x;allqipan[nCountx][y]==nColor&&nCountx>=0;nCountx--,nCount++);
	if(nCountx<0||allqipan[nCountx][y]!=0)
	{
		return 0;
	}
	

	for(nCountx=x+1;allqipan[nCountx][y]==nColor&&nCountx<=17;nCountx++,nCount++);
	if(nCountx>17||allqipan[nCountx][y]!=0)
	{
		return 0;
	}

	if(nCount==4)
	{
		return 1;
	}
	//shu
	nCount=0;
	for(nCounty=y;allqipan[x][nCounty]==nColor&&nCounty>=0;nCounty--,nCount++);
	if(nCounty<0||allqipan[x][nCounty]!=0)
	{
		return 0;
	}
	
	for(nCounty=y+1;allqipan[x][nCounty]==nColor&&nCounty<=17;nCounty++,nCount++);
	if(nCounty>17||allqipan[x][nCounty]!=0)
	{
		return 0;
	}
	
	if(nCount==4)		return 1;
	//zuoyou
	nCount=0;
	for(nCountx=x,nCounty=y;allqipan[nCountx][nCounty]==nColor&&nCountx>=0&&nCounty>=0;nCountx--,nCounty--,nCount++);
	if(nCounty<0||nCountx<0||allqipan[nCountx][nCounty]!=0)
	{
		return 0;
	}
	
	for(nCountx=x+1,nCounty=y+1;allqipan[nCountx][nCounty]==nColor&&nCountx<=17&&nCounty<=17;nCountx++,nCounty++,nCount++);
	if(nCounty>17||nCountx>17||allqipan[nCountx][nCounty]!=0)
	{
		return 0;
	}
	
	if(nCount==4)		return 1;
	//youzuo
	nCount=0;
	for(nCountx=x,nCounty=y;allqipan[nCountx][nCounty]==nColor&&nCountx>=0&&nCounty<=17;nCountx--,nCounty++,nCount++);
	if(nCounty<0||nCountx>17||allqipan[nCountx][nCounty]!=0)
	{
		return 0;
	}
	
	for(nCountx=x+1,nCounty=y-1;allqipan[nCountx][nCounty]==nColor&&nCountx<=17&&nCounty<=17;nCountx++,nCounty--,nCount++);
	if(nCounty>17||nCountx<0||allqipan[nCountx][nCounty]!=0)
	{
		return 0;
	}
	
	if(nCount==4)		return 1;
	return 0;

}

int Overall::shuangsan(int x,int y)
{//�ж��ǲ���˫��
	int nCountx,nCounty,nColor,nCount,flag;
	nCount=flag=0;
	nColor=allqipan[x][y];
	//heng
	for(nCountx=x;allqipan[nCountx][y]==nColor&&nCountx>=0;nCountx--);
	if(nCountx>=0&&allqipan[nCountx][y]==0)
	{
		allqipan[nCountx][y]=nColor;
		if(huosi(nCountx,y)==1)
		{
			nCount++;
			flag=1;
		}
		allqipan[nCountx][y]=0;
	}
	if(flag==0)
	{
		for(nCountx=x+1;allqipan[nCountx][y]==nColor&&nCountx<=17;nCountx++);
		if(nCountx<=17&&allqipan[nCountx][y]==0)
		{
			allqipan[nCountx][y]=nColor;
			if(huosi(nCountx,y)==1)
			{
				nCount++;
			}
			allqipan[nCountx][y]=0;
		}
	}
	flag=0;
	//shu
	for(nCounty=y;allqipan[x][nCounty]==nColor&&nCounty>=0;nCounty--);
	if(nCounty>=0&&allqipan[x][nCounty]==0)
	{
		allqipan[x][nCounty]=nColor;
		if(huosi(x,nCounty)==1)
		{
			nCount++;
			flag=1;
		}
		allqipan[x][nCounty]=0;	
	}
	if(flag==0)
	{
		for(nCounty=y+1;allqipan[x][nCounty]==nColor&&nCounty<=17;nCounty++);
		if(nCounty<=17&&allqipan[x][nCounty]==0)
		{
			allqipan[x][nCounty]=nColor;
			if(huosi(x,nCounty)==1)
			{
				nCount++;
			}
			allqipan[x][nCounty]=0;	
		}
	}
	flag=0;
	//zuoyou
	for(nCountx=x,nCounty=y;allqipan[nCountx][nCounty]==nColor&&nCountx>=0&&nCounty>=0;nCountx--,nCounty--);
	if(nCounty>=0&&nCountx>=0&&allqipan[nCountx][nCounty]==0)
	{
		allqipan[nCountx][nCounty]=nColor;
		if(huosi(nCountx,nCounty)==1)
		{
			nCount++;
			flag=1;
		}
		allqipan[nCountx][nCounty]=0;	
	}
	if(flag==0)
	{

		for(nCountx=x+1,nCounty=y+1;allqipan[nCountx][nCounty]==nColor&&nCountx<=17&&nCounty<=17;nCountx++,nCounty++);
		if(nCounty<=17&&nCountx<=17&&allqipan[nCountx][nCounty]==0)
		{
			allqipan[nCountx][nCounty]=nColor;
			if(huosi(nCountx,nCounty)==1)
			{
				nCount++;
			}
			allqipan[nCountx][nCounty]=0;	
		}
	}
	flag=0;
	
	//youzuo
	for(nCountx=x,nCounty=y;allqipan[nCountx][nCounty]==nColor&&nCountx>=0&&nCounty<=17;nCountx--,nCounty++);
	if(nCounty>=0&&nCountx<=17&&allqipan[nCountx][nCounty]==0)
	{
		allqipan[nCountx][nCounty]=nColor;
		if(huosi(nCountx,nCounty)==1)
		{
			nCount++;
			flag=1;
		}
		allqipan[nCountx][nCounty]=0;	
	}
	if(flag==0)
	{

		for(nCountx=x+1,nCounty=y-1;allqipan[nCountx][nCounty]==nColor&&nCountx<=17&&nCounty<=17;nCountx++,nCounty--);
		if(nCounty<=17&&nCountx>=0&&allqipan[nCountx][nCounty]==0)
		{
			allqipan[nCountx][nCounty]=nColor;
			if(huosi(nCountx,nCounty)==1)
			{
				nCount++;
			}
			allqipan[nCountx][nCounty]=0;	
		}
	}
	if(nCount>=2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
	
int Overall::shuangsi(int x,int y)
{//�ж��ǲ���˫����
	int nCountx,nCounty,nColor,nCount,recordx,recordy;
	nCount=0;
	nColor=allqipan[x][y];
	//heng
	for(nCountx=x;allqipan[nCountx][y]==nColor&&nCountx>=0;nCountx--);
	if(nCountx>=0&&allqipan[nCountx][y]==0)
	{
		allqipan[nCountx][y]=nColor;
		if(winchanglian(nCountx,y)==1)
		{
			nCount++;
			recordx=nCountx;
			recordy=y;
		}
		allqipan[nCountx][y]=0;
	}
	{
		for(nCountx=x+1;allqipan[nCountx][y]==nColor&&nCountx<=17;nCountx++);
		if(nCountx<=17&&allqipan[nCountx][y]==0)
		{
			allqipan[nCountx][y]=nColor;
			if(winchanglian(nCountx,y)==1)
			{
				if(nCountx-recordx!=5) nCount++;
			}
			allqipan[nCountx][y]=0;
		}
	}
	recordx=recordy=-50;
	//shu
	for(nCounty=y;allqipan[x][nCounty]==nColor&&nCounty>=0;nCounty--);
	if(nCounty>=0&&allqipan[x][nCounty]==0)
	{
		allqipan[x][nCounty]=nColor;
		if(winchanglian(x,nCounty)==1)
		{
			nCount++;
			recordx=x;
			recordy=nCounty;
		}
		allqipan[x][nCounty]=0;	
	}
	{
		for(nCounty=y+1;allqipan[x][nCounty]==nColor&&nCounty<=17;nCounty++);
		if(nCounty<=17&&allqipan[x][nCounty]==0)
		{
			allqipan[x][nCounty]=nColor;
			if(winchanglian(x,nCounty)==1)
			{
				if(nCounty-recordy!=5)
				{
					nCount++;
				}
			}
			allqipan[x][nCounty]=0;	
		}
	}
	recordx=recordy=-50;
	//zuoyou
	for(nCountx=x,nCounty=y;allqipan[nCountx][nCounty]==nColor&&nCountx>=0&&nCounty>=0;nCountx--,nCounty--);
	if(nCounty>=0&&nCountx>=0&&allqipan[nCountx][nCounty]==0)
	{
		allqipan[nCountx][nCounty]=nColor;
		if(winchanglian(nCountx,nCounty)==1)
		{
			nCount++;
			recordx=nCountx;
			recordy=nCounty;
		}
		allqipan[nCountx][nCounty]=0;	
	}
	{

		for(nCountx=x+1,nCounty=y+1;allqipan[nCountx][nCounty]==nColor&&nCountx<=17&&nCounty<=17;nCountx++,nCounty++);
		if(nCounty<=17&&nCountx<=17&&allqipan[nCountx][nCounty]==0)
		{
			allqipan[nCountx][nCounty]=nColor;
			if(winchanglian(nCountx,nCounty)==1)
			{
				if(nCountx-recordx!=4&&nCounty-recordy!=5)
				{
					nCount++;
				}
			}
			allqipan[nCountx][nCounty]=0;	
		}
	}
	recordx=recordy=-50;
	
	//youzuo
	for(nCountx=x,nCounty=y;allqipan[nCountx][nCounty]==nColor&&nCountx>=0&&nCounty<=17;nCountx--,nCounty++);
	if(nCounty>=0&&nCountx<=17&&allqipan[nCountx][nCounty]==0)
	{
		allqipan[nCountx][nCounty]=nColor;
		if(winchanglian(nCountx,nCounty)==1)
		{
			nCount++;
			recordx=nCountx;
			recordy=nCounty;
		}
		allqipan[nCountx][nCounty]=0;	
	}
	{

		for(nCountx=x+1,nCounty=y-1;allqipan[nCountx][nCounty]==nColor&&nCountx<=17&&nCounty<=17;nCountx++,nCounty--);
		if(nCounty<=17&&nCountx>=0&&allqipan[nCountx][nCounty]==0)
		{
			allqipan[nCountx][nCounty]=nColor;
			if(winchanglian(nCountx,nCounty)==1)
			{
				if(nCountx-recordx!=4&&recordy-nCounty!=5)
				{
					nCount++;
				}
			}
			allqipan[nCountx][nCounty]=0;	
		}
	}
	if(nCount>=2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
	




