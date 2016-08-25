#include <iostream.h> 

void run(int* pData,int left,int right) 
{ 
  int i,j; 
  int middle,iTemp; 
  i = left; 
  j = right; 
  middle = pData[(left+right)/2]; //���м�ֵ 
  do{ 
    while((pData[i]<middle) && (i<right))//����ɨ�������ֵ���� 
      i++;      
    while((pData[j]>middle) && (j>left))//����ɨ�������ֵ���� 
      j--; 
    if(i<=j)//�ҵ���һ��ֵ 
    { 
      //���� 
      iTemp = pData[i]; 
      pData[i] = pData[j]; 
      pData[j] = iTemp; 
      i++; 
      j--; 
    } 
  }while(i<=j);//�������ɨ����±꽻����ֹͣ�����һ�Σ� 

  //����߲�����ֵ(left<j)���ݹ����� 
  if(left<j) 
    run(pData,left,j); 
  //���ұ߲�����ֵ(right>i)���ݹ��Ұ�� 
  if(right>i) 
    run(pData,i,right); 
} 

void QuickSort(int* pData,int Count) 
{ 
  run(pData,0,Count-1); 
} 

void main() 
{ 
  int data[] = {10,9,8,7,6,5,4}; 
  QuickSort(data,7); 
  for (int i=0;i<7;i++) 
    cout<<data[i]<<" "; 
  cout<<"\n"; 
} 

