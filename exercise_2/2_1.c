#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#define LOGD //printf

/* Print the ranges of char, short, int and long both by printing limits and computation */
int main()
{
    printf("Size of signed variables\n");
    printf("CHAR_MAX %d CHAR_MIN %d\n",SCHAR_MAX,SCHAR_MIN);
    printf("INT_MAX %d INT_MIN %d\n",INT_MAX,INT_MIN);
    printf("SHORT_MAX %d SHORT_MIN %d\n",SHRT_MAX,SHRT_MIN);
    printf("LONG_MAX %ld LONG_MIN %ld\n",LONG_MAX,LONG_MIN);

    printf("Size of signed variables by computation\n");
    int iSignedCharMax = ((int)pow(2,((sizeof(char)*8)-1))-1);
    int iSignedIntMax = (int)((long)pow(2,((sizeof(int)*8)-1))-1);
    int iSignedShortMax = ((int)pow(2,((sizeof(short)*8)-1))-1);
    long iSignedLongMax = (long)(pow(2,((sizeof(long)*8)-1))-1.0);
    LOGD("(sizeof(int)*8) %d\n",(sizeof(long)*8));
    LOGD("((sizeof(int)*8)-1) %d\n",((sizeof(long)*8)-1));
    LOGD("(long long)pow(2,((sizeof(long)*8)-1)) %lld\n",(long long)pow(2,((sizeof(long)*8)-1)));
    printf("CHAR_MAX %d CHAR_MIN %d\n",iSignedCharMax,-(iSignedCharMax+1));
    printf("INT_MAX %d INT_MIN %d\n",iSignedIntMax,-(iSignedIntMax+1));
    printf("SHORT_MAX %d SHORT_MIN %d\n",iSignedShortMax,-(iSignedShortMax+1));
    printf("LONG_MAX %ld LONG_MIN %ld\n",iSignedLongMax,-(iSignedLongMax+1));
    return 0;
}
