#include <iostream.h>
#include "matlib.h"
#include "mymagic.h"
#include "myeig.h"

void main()
{
// ѭ������
	int i;
	
	// ����ά��
	int m,n;
	
	// ����÷������ά��
	int nDim;
	cout<<"�����������ɻ÷������ά����"<<endl;
	cin>>nDim;
	// ���ɻ÷�����
    Mm X = mymagic(nDim);	
	// ��ȡ����ά��
	m = X.size(1);
	n = X.size(2);
	// Ϊ���ݷ���ռ�
	double *pXData = (double*)malloc(sizeof(double)*m*n);
	memset(pXData,0,sizeof(double)*m*n);
	// ���ƾ�������
	memcpy(pXData,X.addr(),sizeof(double)*m*n);	
	// ����÷�����
	cout<<"�÷�����Ϊ��"<<endl;
	for(i = 0; i < m*n; i++)
	{
		cout.width(4);
		cout<<*(pXData+i)<<" ";

		if(i!=0 && (i+1)%nDim==0)
		{
			cout<<endl;
		}
	}

	// ���������ֵ
	Mm E = myeig(X);
	// ��ȡ����ֵ�ĸ���	
	nDim = E.size(1);
	// ������������ֵ
	cout<<"���������ֵ���£�"<<endl;
	for(i = 1; i <= nDim; i++)
	{
		cout<<E.r(i,1)<<endl;
	}
	
	// ��ͣ���򣬹۲���
	cout<<"���������ַ������س��˳�����"<<endl;
	getchar();
}
