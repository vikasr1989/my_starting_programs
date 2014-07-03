#include <stdio.h>
#include <stdlib.h>

#define PRINT_ELEMENTS(a,size) \
    for(i=0;i<((size));i++)           \
    {                          \
        printf("%d ",(a)[i]);    \
    }                          \
    printf("\n");              \
    


void swap(int v[],int i, int j)
{
    int temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}
#if 0
void my_qsort(int v[],int left,int right)
{
    int i,last;
    if(left<=right)
    {
        return;
    }
    last = left;
    swap(v,left,(left+right)/2);
    for(i=0;i<right;i++)
    {
        if(v[i] < v[left])
        {
            swap(v,i,++last);
        }
    }
    swap(v,(left+right)/2,left);
    my_qsort(v,left,last);
    my_qsort(v,last+1,right);
}
#endif
#if 1
void my_qsort(int v[],int left,int right)
{
    int i,last;
    if(left>=right)
    {
        return;
    }
    last = left;
    swap(v,left,(left+right)/2);
    for(i=left+1;i<=right;i++)
    {
        if(v[i] < v[left])
        {
            swap(v,++last,i);
        }
    }
    swap(v,left,last);
    my_qsort(v,left,last-1);
    my_qsort(v,last+1,right);
}
#endif

#define LEFT 0
#define RIGHT ((sizeof(a)/(sizeof(int)))-1)
int main()
{
    int i;
    int a[] = {98,76,1,43,21};
    printf("before sorting\n");
    PRINT_ELEMENTS(a,RIGHT+1);
    
    my_qsort(a,LEFT,RIGHT);
    printf("after sorting\n");
    PRINT_ELEMENTS(a,RIGHT+1);
    return 0;
}
