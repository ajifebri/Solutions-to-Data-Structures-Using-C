#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1000

int main() {
    char line[BUFFER_SIZE], line2[BUFFER_SIZE];
    char *p;
    double mag;

    while (fgets(line, sizeof(line), stdin) != NULL) {
        strcpy(line2, line);

        p = strtok(line, " ");
        p = strtok(NULL, " ");
        p = strtok(NULL, " ");

        //p = strtok(line, " \t");
        //p = strtok(NULL, " \t");
        //p = strtok(NULL, " \t");
        sscanf(p, "%lf", &mag);
        if (mag > 6)
            printf("%s", line2);
    }

    return 0;
}
