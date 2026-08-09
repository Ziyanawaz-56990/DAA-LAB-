/*
 * DAA Lab-01 | Q1 - Put them in Order
 *
 * Ranks the twelve functions for a sufficiently large n and writes
 * numerical data for a logarithmic growth plot.
 *
 * Direct evaluation of 3^n and n^(log2 n) can overflow. Therefore
 * log10(f(n)) is calculated algebraically and written to CSV.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FUNCTION_COUNT 12
#define SAMPLE_COUNT 12

typedef struct {
    const char *name;
    long double key;
} FunctionValue;

static long double log10_n_from_x(long double x) { return x; }

/* x = log10(n), return log10(f(n)). */
static long double log10_function(int id, long double x) {
    const long double ln10 = logl(10.0L);
    const long double ln_n = x * ln10;
    const long double log10_2 = log10l(2.0L);
    const long double log10_3 = log10l(3.0L);

    switch (id) {
        case 0: /* n log2 n */
            return x + log10l(ln_n / logl(2.0L));
        case 1: /* 12 sqrt(n) */
            return log10l(12.0L) + 0.5L * x;
        case 2: /* 1/n */
            return -x;
        case 3: /* n^(log2 n) */
            return (ln_n * ln_n / logl(2.0L)) / ln10;
        case 4: /* 100n^2 + 6n */
            return log10l(100.0L) + 2.0L*x +
                   log10l(1.0L + 0.06L * expl(-ln_n));
        case 5: /* n^0.51 */
            return 0.51L*x;
        case 6: /* n^2 - 324 */
            return 2.0L*x + log10l(1.0L - 324.0L * expl(-2.0L*ln_n));
        case 7: /* 50n^0.5 */
            return log10l(50.0L) + 0.5L*x;
        case 8: /* 2n^3 */
            return log10l(2.0L) + 3.0L*x;
        case 9: /* 3^n */
            /* log10(3^n) = n log10(3) = 10^x log10(3). */
            return expl(ln_n) * log10_3;
        case 10: /* 2^32 n */
            return 32.0L*log10_2 + x;
        case 11: /* log2 n */
            return log10l(ln_n / logl(2.0L));
        default:
            return 0.0L;
    }
}

static int compare(const void *a, const void *b) {
    const FunctionValue *x = (const FunctionValue *)a;
    const FunctionValue *y = (const FunctionValue *)b;
    if (x->key < y->key) return -1;
    if (x->key > y->key) return 1;
    return 0;
}

int main(void) {
    const char *names[FUNCTION_COUNT] = {
        "n log2 n", "12 sqrt(n)", "1/n", "n^(log2 n)",
        "100n^2 + 6n", "n^0.51", "n^2 - 324", "50n^0.5",
        "2n^3", "3^n", "2^32 n", "log2 n"
    };

    /* x values are log10(n). The final point represents n = 10^1000. */
    const long double x_values[SAMPLE_COUNT] =
        {1, 2, 4, 8, 16, 32, 64, 128, 256, 400, 700, 1000};

    FILE *data = fopen("q1_growth_data.csv", "w");
    FILE *order = fopen("q1_growth_order.csv", "w");
    if (!data || !order) {
        perror("Unable to create Q1 CSV files");
        if (data) fclose(data);
        if (order) fclose(order);
        return 1;
    }

    fprintf(data, "log10_n,function,log10_f\n");
    for (int s = 0; s < SAMPLE_COUNT; ++s) {
        long double x = x_values[s];
        for (int i = 0; i < FUNCTION_COUNT; ++i) {
            long double y = log10_function(i, x);
            /* 3^n is beyond ordinary floating-point plotting range at high x.
               Skip only non-finite points; all useful lower-x points remain. */
            if (isfinite(y))
                fprintf(data, "%.0Lf,\"%s\",%.12Lf\n", x, names[i], y);
        }
    }

    /* Ranking at n = 10^1000.  1/n is less than 1, so log10(f) is valid. */
    FunctionValue values[FUNCTION_COUNT];
    for (int i = 0; i < FUNCTION_COUNT; ++i) {
        values[i].name = names[i];
        values[i].key = log10_function(i, 1000.0L);
    }
    qsort(values, FUNCTION_COUNT, sizeof(FunctionValue), compare);

    fprintf(order, "rank,function,log10_f_at_n_10^1000\n");
    printf("Increasing order of growth for sufficiently large n:\n\n");
    for (int i = 0; i < FUNCTION_COUNT; ++i) {
        printf("%2d. %s\n", i+1, values[i].name);
        fprintf(order, "%d,\"%s\",%.12Lf\n", i+1, values[i].name, values[i].key);
    }

    fclose(data);
    fclose(order);
    printf("\nGenerated q1_growth_data.csv and q1_growth_order.csv\n");
    return 0;
}
