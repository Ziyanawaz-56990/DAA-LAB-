/* DAA Lab-01 | Q2 - Fair vs Biased Coin Simulation */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static int toss(double p) {
    return ((double)rand() / ((double)RAND_MAX + 1.0)) < p;
}

int main(void) {
    const int convergence[] = {10,100,1000,10000,100000,1000000};
    const int nc = sizeof(convergence)/sizeof(convergence[0]);
    const int biases[] = {30,50,60,70,90};
    const int nb = sizeof(biases)/sizeof(biases[0]);
    const int comparison_tosses = 100000;

    srand(20260809);

    FILE *fc = fopen("q2_fair_convergence.csv", "w");
    FILE *fb = fopen("q2_bias_comparison.csv", "w");
    if (!fc || !fb) {
        perror("Unable to create Q2 CSV files");
        if (fc) fclose(fc);
        if (fb) fclose(fb);
        return 1;
    }

    fprintf(fc, "tosses,heads,tails,observed_probability,absolute_error\n");
    int heads=0;
    for (int t=1, k=0; t<=convergence[nc-1]; ++t) {
        heads += toss(0.50);
        if (k<nc && t==convergence[k]) {
            double p=(double)heads/t;
            fprintf(fc,"%d,%d,%d,%.8f,%.8f\n",t,heads,t-heads,p,fabs(p-0.50));
            ++k;
        }
    }

    fprintf(fb,"bias_percent,heads,tails,observed_probability,expected_probability,absolute_error\n");
    for (int b=0;b<nb;++b) {
        int h=0;
        for (int t=0;t<comparison_tosses;++t) h += toss(biases[b]/100.0);
        double p=(double)h/comparison_tosses;
        fprintf(fb,"%d,%d,%d,%.8f,%.8f,%.8f\n",biases[b],h,comparison_tosses-h,p,biases[b]/100.0,fabs(p-biases[b]/100.0));
    }

    fclose(fc); fclose(fb);
    printf("Fair coin convergence and fair-vs-biased comparison completed.\n");
    printf("Generated q2_fair_convergence.csv and q2_bias_comparison.csv\n");
    return 0;
}
