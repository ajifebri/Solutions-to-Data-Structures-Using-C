#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define num_workers 10
#define num_polishing 1
#define min_assembly 1
#define max_assembly 15
#define mean_polishing 20
#define std_polishing 7
#define max_items_polish 1000

struct node {
    int time, finish_assembly, start_polish;
    struct node *next;
};
typedef struct node *NODEPTR;

struct node auxinfo;
NODEPTR queue_list;
NODEPTR polish_list;
int total_wait;

void place(NODEPTR *plist, NODEPTR nodeX); 
void push(NODEPTR *plist, NODEPTR nodeX); 
void insafter(NODEPTR q, NODEPTR nodeX); 
void popsub(NODEPTR *plist, NODEPTR nodeX); 
int generate_duration_uniform(int min, int max);
int generate_duration_normal(int mean, int std); 
void add_assembly(int atime);
void finish_polish(int finish_time, int start_polish, int finish_assembly);
NODEPTR getnode();

int main() {
    /* initializations */
    queue_list = NULL;
    polish_list = NULL;
    total_wait = 0;

    /* Intializes random number generator */
    srand((unsigned) time(0));

    /* Initialize assembly time and place to the queue */
    int dur_assembly;
    for (int i=0; i<num_workers; i++) {
        dur_assembly = generate_duration_uniform(min_assembly, max_assembly);
        auxinfo.time = dur_assembly;
        //printf("assembly time: %d\n", dur_assembly);
        place(&queue_list, &auxinfo);
    }

    /* Front queue */
    for (int i=0; i<num_polishing; i++) {
        int dur_polish = 0;
        while (dur_polish < 5) {
            dur_polish = generate_duration_normal(mean_polishing, std_polishing);
        }
        //printf("polishing time: %d\n", dur_polish);

        /* Give the front of the queue the polishing machine */
        popsub(&queue_list, &auxinfo);
        auxinfo.start_polish = auxinfo.time;
        auxinfo.finish_assembly = auxinfo.time;
        auxinfo.time = auxinfo.time + dur_polish;
        place(&polish_list, &auxinfo);
    }
    
    /* run the simulation until the maximum number of polished items reached */
    int dtime, swtime, sptime;
    int num_items_polish = 0;
    while (num_items_polish < max_items_polish) {
        popsub(&polish_list, &auxinfo);
        dtime = auxinfo.time;
        swtime = auxinfo.finish_assembly; 
        sptime = auxinfo.start_polish;
        //printf("time_departure_event: %d\n", dtime);
        //printf("start_wait: %d\n", swtime);
        //printf("start_polish: %d\n", sptime);

        add_assembly(dtime); /* after polishing, this worker begins assembly again */

        /* compute the waiting time for this item, and */
        /* give the associated polishing machine to the front of the waiting queue */
        finish_polish(dtime, sptime, swtime);

        num_items_polish++;
    }

    printf("total items is %4d\n", max_items_polish);
    printf("total waiting time is %4d\n", total_wait);
    printf("average waiting time is %4.2f\n", (float)total_wait/num_items_polish);

    return 0;
}

void add_assembly(int atime) {
    int dur_assembly;
    dur_assembly = generate_duration_uniform(min_assembly, max_assembly);
    //printf("assembly time: %d\n", dur_assembly);
    auxinfo.time = atime + dur_assembly; /* finish assembly time */
    auxinfo.finish_assembly = atime + dur_assembly;
    place(&queue_list, &auxinfo); /* sorted by time of ready to polish (finish assembly time) */
}

void finish_polish(int finish_time, int start_polish, int finish_assembly) {
    int dur_polish;
    int wait_time = start_polish - finish_assembly;
    total_wait += wait_time;

    dur_polish = 0;
    while (dur_polish < 5) {
        dur_polish = generate_duration_normal(mean_polishing, std_polishing);
    }

    //printf("polishing time: %d\n", dur_polish);

    /* Give the front of the queue the polishing machine */
    popsub(&queue_list, &auxinfo);
    auxinfo.start_polish = finish_time;
    auxinfo.time = auxinfo.start_polish + dur_polish;
    place(&polish_list, &auxinfo); /* sorted by the time of finish polishing */
}

void place(NODEPTR *plist, NODEPTR nodeX) {
    NODEPTR p, q, newNode;
    q = NULL;
    int time = nodeX->time;

    for (p = *plist; p != NULL && time > p->time; p = p->next) {
        q = p;
    }

    if (q == NULL) { /* Insert x at the head of the list */
        push(plist, nodeX);
    } else {
        insafter(q, nodeX);
    }
}

void push(NODEPTR *plist, NODEPTR nodeX) {
    NODEPTR newNode;

    newNode = getnode();
    newNode->time = nodeX->time;
    newNode->finish_assembly= nodeX->finish_assembly;
    newNode->start_polish = nodeX->start_polish;

    newNode->next = *plist;

    *plist = newNode;
}

void insafter(NODEPTR q, NODEPTR nodeX) {
    NODEPTR newNode;

    newNode = getnode();
    newNode->time = nodeX->time;
    newNode->finish_assembly = nodeX->finish_assembly;
    newNode->start_polish = nodeX->start_polish;

    if (q->next == NULL) {
        q->next = newNode;
        newNode->next = NULL;
    } else {
        newNode->next = q->next;
        q->next = newNode;
    }
}

void popsub(NODEPTR *plist, NODEPTR nodeX) {
    NODEPTR p;
    p = *plist;
    nodeX->time = p->time;
    nodeX->finish_assembly = p->finish_assembly;
    nodeX->start_polish = p->start_polish;

    *plist = p->next;

    free(p);
}

NODEPTR getnode() {
    NODEPTR p;
    p = (NODEPTR) malloc(sizeof(struct node));
    return p;
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
