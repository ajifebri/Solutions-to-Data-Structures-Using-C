#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define loop_times 20

int generate_duration_uniform(int min, int max);

int main() {
    /* Intializes random number generator */
    srand((unsigned) time(0));

    for (int i=0; i<loop_times; i++) {
        printf("%d\n", generate_duration_uniform(1,15));
    }

    return 0;
}

int generate_duration_uniform(int min, int max) {
    return min + (int) (rand()/(double) ((unsigned)RAND_MAX + 1) * (max - min));
}

