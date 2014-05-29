#include <stdio.h>

int main()
{
    float cel,fahr;
    int lower,upper,step;
    lower = 0;
    upper = 200;
    step = 20;

    cel = lower;
    printf("Celcius\tFahrenheit\n");
    while(cel <= upper)
    {
        fahr = ((9.0/5.0)*cel)+32.0;
        printf("%3.0f\t %6.2f\n",cel,fahr);
        cel += 20;
    }

    return 0;
}
