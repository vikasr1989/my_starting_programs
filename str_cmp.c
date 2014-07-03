#include <stdio.h>
#include <stdlib.h>

int my_strcmp(char *s, char*t)
{
    for(;*s==*t;s++,t++)
    {
        if(*s == '\0')
        {
            return 0;
        }
    }
    return *s - *t;
}
int main()
{
    printf("difference is %d\n",my_strcmp("abc","ab"));
    return 0;
}
