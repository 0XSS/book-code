#include<stdio.h>

void print_it(int n, int arr[])
{
    int        i;
    static int cnt = 0;
    printf("%04d:\t", ++cnt);
    for (i = 0; i < n; i++)
        printf("%2d ", arr[i]);
    printf("\n");
}


void arrange_all(int size, int arr[], int pos)
{
    int i, tmp;
    if (pos + 1== size) /*  ����Ѿ��ŵ����  */
    {
        print_it(size, arr); /*  ��ӡ����  */
        return;
    }
    for (i = pos; i < size; i++) /*  �Ե�ǰλ�ú������λ������*/
    {
        tmp = arr[pos]; /*  ����λ��  */
        arr[pos] = arr[i];
        arr[i] = tmp;
        /*  �ݹ飬��������ĵ���  */
        arrange_all(size, arr, pos + 1);
        /*  �ڽ�������������ԭ�е����д���  */
        tmp = arr[pos];
        arr[pos] = arr[i];
        arr[i] = tmp;
    }
}



int main()
{
    int a[5] = { 6, 2, 3, 4, 5};
    arrange_all(5, a, 0);
    return 0;
}
