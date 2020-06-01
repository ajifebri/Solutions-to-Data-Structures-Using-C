#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define num_tellers 2
#define max_customers 1000

struct node {
    int duration, time, type;
    struct node *next;
};
typedef struct node *NODEPTR;

struct queue {
    NODEPTR front, rear;
    int num;
};
struct queue wait_queue;

struct node auxinfo;
NODEPTR evlist;
int atime, dtime, dur;
float tottime, average_time, std;
int count;
int in_services;

int time_spent[max_customers];

void arrive(int atime, int dur); 
void depart(int dtime);
void insert(struct queue *pq, NODEPTR nodeX);
void removequeue(struct queue *pq); 
void place(NODEPTR *plist, NODEPTR nodeX); 
void placeDeparture(NODEPTR *plist, NODEPTR nodeX); 
void push(NODEPTR *plist, NODEPTR nodeX); 
void insafter(NODEPTR q, NODEPTR nodeX); 
void popsub(NODEPTR *plist, NODEPTR nodeX); 
NODEPTR getnode();

int main() {
    /* initializations */
    in_services = 0;
    evlist = NULL;
    count = 0;
    tottime = 0;

    /* initialize the event list with the first arrival */
    //printf("enter time and duration\n");
    scanf("%d %d", &auxinfo.time, &auxinfo.duration);
    auxinfo.type = -1; /* an arrival */
    place(&evlist, &auxinfo);

    /* run the simulation as long as */
    /* the event list is not empty */
    while (evlist != NULL) {
        popsub(&evlist, &auxinfo);
        /* check if the next event is an arrival or departure */
        if (auxinfo.type == -1) {
            /* an arrival */
            atime = auxinfo.time;
            dur = auxinfo.duration;
            arrive(atime, dur);
        } else {
            /* a departure */
            dtime = auxinfo.time;
            time_spent[count] = (dtime-auxinfo.duration);
            tottime = tottime + (dtime-auxinfo.duration);
            count++;
            
            depart(dtime);
        }
    }

    average_time = (float) tottime/count;

    /* Computing the standard deviation */
    float mean_squares = 0.0;
    for (int i=0; i<count; i++) {
        mean_squares += ((time_spent[i] - average_time)*(time_spent[i] - average_time));
    }
    std = sqrt(mean_squares/(count-1.0));

    printf("total time is %4.2f\n", tottime);
    printf("average time is %4.2f\n", average_time);
    printf("standard deviation is %4.2f\n", std);
    
    return 0;
}

void arrive(int atime, int dur) {
    NODEPTR p;

    /* insert a new customer node */
    auxinfo.time = atime;
    auxinfo.duration = dur;
    auxinfo.type = 0;
    insert(&wait_queue, &auxinfo);
    /* Check if this is the only node on the queue. If it is, */
    /* the customer's departure node must be placed on the event list */
    if (in_services < num_tellers) {
        in_services++; 
        p = wait_queue.front;
        p->time = atime + dur;
        p->duration = atime;
        removequeue(&wait_queue);
        placeDeparture(&evlist, p);
    }

    /* If any input remains, read the next data pair and */
    /* place an arrival on the event list. */
    // printf("enter time\n");
    if (scanf("%d %d", &auxinfo.time, &auxinfo.duration) != EOF) {
        //printf("enter duration\n");
        //scanf("%d", &auxinfo.duration);
        auxinfo.type = -1;
        place(&evlist, &auxinfo);
    }
}

void depart(int dtime) {
    NODEPTR p;
    /* If there are any more customers on the queue, */
    /* place the departure of the next customer onto */
    /* the event list after computing its departure time */

    // Decrease the number of customers in service
    in_services--;

    int temp = 0;
    /* There is any customer in the queue and the service is available */
    if ((wait_queue.num > 0) && (in_services<num_tellers)) { 
        in_services++;
        p = wait_queue.front;
        temp = p->time;
        p->time = dtime + p->duration;
        p->duration = temp;
        removequeue(&wait_queue);
        placeDeparture(&evlist, p);
    }
}

void insert(struct queue *pq, NODEPTR nodeX) {
    NODEPTR p;
    p = getnode();
    p->time = nodeX->time;
    p->duration = nodeX->duration;
    p->type = nodeX->type;
    p->next = NULL;

    if (pq->rear == NULL) {
        pq->front = p;
    } else {
        (pq->rear)->next = p;
    }

    pq->rear = p;

    /* Update the number of nodes of the queue */
    (pq->num)++;
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
    newNode->duration = nodeX->duration;
    newNode->type = nodeX->type;

    newNode->next = *plist;

    *plist = newNode;
}

void insafter(NODEPTR q, NODEPTR nodeX) {
    NODEPTR newNode;

    newNode = getnode();
    newNode->time = nodeX->time;
    newNode->duration = nodeX->duration;
    newNode->type = nodeX->type;

    if (q->next == NULL) {
        q->next = newNode;
        newNode->next = NULL;
    } else {
        newNode->next = q->next;
        q->next = newNode;
    }
}

void placeDeparture(NODEPTR *plist, NODEPTR nodeX) {
    NODEPTR p, q, newNode;
    q = NULL;
    int time = nodeX->time;

    for (p = *plist; p != NULL && time > p->time; p = p->next) {
        q = p;
    }

    if (q == NULL) { /* Insert x at the head of the list */
        nodeX->next = *plist;
        *plist = nodeX;
    } else {
        if (q->next == NULL) {
            q->next = nodeX;
            nodeX->next = NULL;
        } else {
            nodeX->next = q->next;
            q->next = nodeX;
        }
    }
}

void popsub(NODEPTR *plist, NODEPTR nodeX) {
    NODEPTR p;
    p = *plist;
    nodeX->time = p->time;
    nodeX->duration = p->duration;
    nodeX->type = p->type;

    *plist = p->next;

    free(p);
}

NODEPTR getnode() {
    NODEPTR p;
    p = (NODEPTR) malloc(sizeof(struct node));
    return p;
}

void removequeue(struct queue *pq) {
    NODEPTR p;

    p = pq->front;

    if (pq->num == 1) {
        pq->front = NULL;
        pq->rear = NULL;
    } else {
        pq->front = p->next;
    }

    //free(p);

    (pq->num)--;
}

