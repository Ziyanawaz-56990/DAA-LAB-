#include <stdio.h>
#include <math.h>

typedef struct
{
    char name[40];
    long double value;
} Function;

int main()
{
    long double n = 1000000.0L;

    Function f[] =
    {
        {"1/n", 1.0L / n},
        {"log2(n)", log2l(n)},
        {"12*sqrt(n)", 12.0L * sqrtl(n)},
        {"50*n^0.5", 50.0L * powl(n, 0.5L)},
        {"n^0.51", powl(n, 0.51L)},
        {"n*log2(n)", n * log2l(n)},
        {"2^32*n", powl(2.0L, 32) * n},
        {"n^2-324", n * n - 324},
        {"100*n^2+6*n", 100.0L * n * n + 6.0L * n},
        {"2*n^3", 2.0L * n * n * n},
        {"3^n", powl(3.0L, n)},
        {"n^(log2(n))", powl(n, log2l(n))}
    };

    int size = sizeof(f) / sizeof(f[0]);

    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < size - i - 1; j++)
        {
            if (f[j].value > f[j + 1].value)
            {
                Function temp = f[j];
                f[j] = f[j + 1];
                f[j + 1] = temp;
            }
        }
    }

    printf("Functions in increasing order of growth:\n");

    for (int i = 0; i < size; i++)
    {
        printf("%s\n", f[i].name);
    }

    return 0;
}