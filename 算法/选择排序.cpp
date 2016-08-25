#include <stdio.h>

//��ӡ������
void PrintResult(int a[], int length)
{
    for (int i = 0; i < length; i++)
        printf("%d ", a[i]);

    printf("\n");
}

//ѡ������
//ѡ����С�����ַŵ�����ĵ�һ��λ�ã�ѡ�ڶ�С�ķŵ��ڶ�������������
void SelectionSort(int a[], int length)
{
    for (int i = 0; i < length - 1; i++) {
        int min = a[i];

        //�ҵ�i+1��ʼ��С�����֣���ֵ��min
        for (int j = i + 1; j < length; j++) {
            if (min > a[j]) {
                int temp = min;
                min = a[j];
                a[j] = temp;
            }
        }

        //���a[i]С��min���������ֽ���
        if (min != a[i]) {
            int temp = min;
            min = a[i];
            a[i] = temp;
        }

        //��ӡÿһ�������Ľ��
        printf("Step %d:", i + 1);
        PrintResult(a, length);
    }
}

int main(int argc, char* argv[])
{
    int a[9] = {9, 4, 2, 7, 6, 5, 8, 3, 1};
    SelectionSort(a, 9);
    return 0;
}

