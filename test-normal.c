#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define loop_times 20

int generate_duration_uniform(int min, int max);
int generate_duration_normal(int mean, int std); 

int main() {
    /* Intializes random number generator */
    srand((unsigned) time(0));

    for (int i=0; i<loop_times; i++) {
        printf("%d\n", generate_duration_normal(20,1));
    }

    return 0;
}

int generate_duration_uniform(int min, int max) {
    return min + (int) (rand()/(double) ((unsigned)RAND_MAX + 1) * (max - min));
}

int generate_duration_normal(int mean, int std) {
    /* Use 15 samples */
    float x[15];
    float sum;
    int y;
    /* Initialize array */
    sum = 0;
    for (int i=0; i<15; i++) {
        x[i] = rand()/(double) ((unsigned)RAND_MAX + 1);
        sum += x[i];
    }
    y = std * (sum - 7.5) / sqrt(1.25) + mean;

    return y;
}


