#include <stdio.h>

#define NUMELTS 1000
#define NUM_DIGITS 2

int power(int base, int exp);
void radixsort(int x[], int n);

int main() {
    int length = 8;
    int a[8] = {25, 57, 48, 37, 12, 92, 86, 33};

    for (int i=0; i<length; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

    radixsort(a, length);
    for (int i=0; i<length; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

    return 0;

}

void radixsort(int x[], int n) {
    int front[10], rear[10];
    struct {
        int info;
        int next;
    } node[NUMELTS];

    int exp, first, i, j, k, p, q, y;

    /* Initialize linked list */
    for (i=0; i<n-1; i++) {
        node[i].info = x[i];
        node[i].next = i+1;
    }
    node[n-1].info = x[n-1];
    node[n-1].next = -1;
    first = 0; /* first is the head of the linked list */

    for (k=1; k <= NUM_DIGITS; k++) {
        /* Assume we have four-digit numbers */
        for (i=0; i<10; i++) {
            /* Initialize queues */
            rear[i] = -1;
            front[i] = -1;
        }
        /* Process each element on the list */
        while (first != -1) {
            p = first;
            first = node[first].next;
            y = node[p].info;
            /* Extract the kth digit */
            exp = power(10,k-1); /* raise 10 to (k-1)th power */
            j = (y/exp)%10;
            /* Insert y into queue[j] */
            q = rear[j];
            if (q == -1) 
                front[j] = p;
            else
                node[q].next = p;
            rear[j] = p;
        }
        /* At this point each record is in its proper queue */
        /* based on digit k. We now form a single list from */
        /* all the queue elements. Find the first element.  */
        for (j=0; j<10 && front[j]==-1; j++)
            ;
        first = front[j];
        /* Link up remaining queues */
        while (j <= 9) {  /* Check if finished */
            /* Find the next element */
            for (i=j+1; i<10 && front[i]==-1; i++)
                ;
            if (i <= 9) {
                p = i;
                node[rear[j]].next = front[i];
            }
            j = i;
        }
        node[rear[p]].next = -1;
    }
    /* Copy back to original array */
    for (i=0; i<n; i++) {
        x[i] = node[first].info;
        first = node[first].next;
    }
}

int power(int base, int exp) {
    int result = 1;
    for (int i=0; i<exp; i++) {
        result *= base;
    }
    return result;
}
