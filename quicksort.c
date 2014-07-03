/************************************************
 * FILE NAME - quicksort.c
 *
 * REVISION  DATE      AUTHOR          CHANGES
 *    1      03/07/14  Vikas R         Added quicksort routine and test code
 * *********************************************/
#include <stdio.h>
#include <stdlib.h>

#define LEFT 0
#define RIGHT ((sizeof(a)/(sizeof(int)))-1)

#define PRINT_ELEMENTS(a,size) \
    for(i=0;i<((size));i++)    \
    {                          \
        printf("%d ",(a)[i]);  \
    }                          \
    printf("\n");              \
    
void swap(int v[],int i, int j);
void my_qsort(int v[],int left,int right);

/************************************************
 * FUNCTION NAME - main
 * DESCRIPTION   - This is the test function to test quicksort routine
 * ARGUMENTS     - None
 * RETURN        - SUCCESS(0) / FAILURE (NON-ZERO)
 * *********************************************/

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

/************************************************
 * FUNCTION NAME - my_qsort
 * DESCRIPTION   - This is the quicksort routine. It is a recursive function 
 *                 which sorts the integer elements provided in the input 
 *                 array
 * ARGUMENTS     - int v[] - pointer to the array of elements to be sorted
 *                 int left - the left index of the array
 *                 int right - the right index of the array
 * RETURN        - NONE
 * *********************************************/
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
/************************************************
 * FUNCTION NAME - swap
 * DESCRIPTION   - Swaps 2 elements of the array v of indices i and j
 * ARGUMENTS     - int v[] - pointer to the array of elements
 *                 int i   - Element to be swapped
 *                 int j   - Element to be wapped with
 * RETURN        - NONE
 * *********************************************/
void swap(int v[],int i, int j)
{
    int temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}
