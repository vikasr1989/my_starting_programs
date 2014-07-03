/******************************************************************************
 * FILE NAME - quicksort.c
 *
 * REVISION  DATE      AUTHOR          CHANGES
 *    1      03/07/14  Vikas R         Added quicksort routine and test code
 *    2      03/07/14  Vikas R         Added int compare function and modified
 *                                     quicksort to take function pointer
 *                                     for comparision
 * ***************************************************************************/
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
    
void my_swap(int v[],int i, int j);
void my_qsort(int v[],int left,int right, int (*comp)(void*,void*));
int  my_int_compare_function(int *a,int *b);

/******************************************************************************
 * FUNCTION NAME - main
 * DESCRIPTION   - This is the test function to test quicksort routine
 * ARGUMENTS     - None
 * RETURN        - SUCCESS(0) / FAILURE (NON-ZERO)
 * ***************************************************************************/
int main()
{
    int i;
    int a[] = {98,76,1,43,21};
    int (*comp)(void*,void*);
    printf("before sorting\n");
    PRINT_ELEMENTS(a,RIGHT+1);
    
    comp = (int (*)(void *,void *))my_int_compare_function;
    my_qsort(a,LEFT,RIGHT,comp);
    printf("after sorting\n");
    PRINT_ELEMENTS(a,RIGHT+1);
    return 0;
}

/******************************************************************************
 * FUNCTION NAME - my_qsort
 * DESCRIPTION   - This is the quicksort routine. It is a recursive function 
 *                 which sorts the integer elements provided in the input 
 *                 array
 * ARGUMENTS     - int v[] - pointer to the array of elements to be sorted
 *                 int left - the left index of the array
 *                 int right - the right index of the array
 * RETURN        - NONE
 * ***************************************************************************/
void my_qsort(int v[],int left,int right, int (*comp)(void*,void*))
{
    int i,last;
    if(left>=right)
    {
        return;
    }
    last = left;
    my_swap(v,left,(left+right)/2);
    for(i=left+1;i<=right;i++)
    {
        /* comp has to be a funtion which returns -1 if &v[i]<&v[left] */
        if((*comp)(&v[i],&v[left]) == -1)
        {
            my_swap(v,++last,i);
        }
    }
    my_swap(v,left,last);
    my_qsort(v,left,last-1,comp);
    my_qsort(v,last+1,right,comp);
}
/******************************************************************************
 * FUNCTION NAME - my_swap
 * DESCRIPTION   - Swaps 2 elements of the array v of indices i and j
 * ARGUMENTS     - int v[] - pointer to the array of elements
 *                 int i   - Element to be swapped
 *                 int j   - Element to be wapped with
 * RETURN        - NONE
 * ***************************************************************************/
void my_swap(int v[],int i, int j)
{
    int temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}
/******************************************************************************
 * FUNCTION NAME - my_int_compare_function
 * DESCRIPTION   - Compares 2 integers
 * ARGUMENTS     - int *a   - Address of first integer to be compared
 *                 int *b   - Address of second integer to be compared
 * RETURN        - -1 if *a < *b
 *                 1  if *a > *b
 *                 0  if *a = *b
 * ***************************************************************************/
int my_int_compare_function(int *a,int *b)
{
    int iRet = 0;
    if(*a < *b)
        iRet = -1;
    if(*a > *b)
        iRet = 1;
    if(*a == *b)
        iRet = 0;

    return iRet;
}
