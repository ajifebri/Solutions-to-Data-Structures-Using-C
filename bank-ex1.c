#include <stdio.h>
#include <stdlib.h>

#define num_tellers  2

struct node {
    int duration, time, type;
    struct node *next;
};
typedef struct node *NODEPTR;

struct queue {
    NODEPTR front, rear;
    int num;
};
struct queue q[num_tellers];

struct node auxinfo;
NODEPTR evlist;
int atime, dtime, dur, qindx;
float count, tottime;

void arrive(int atime, int dur); 
void depart(int qindx, int dtime);
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
    evlist = NULL;
    count = 0;
    tottime = 0;
    for (qindx=0; qindx<num_tellers; qindx++) {
        q[qindx].num = 0;
        q[qindx].front = NULL;
        q[qindx].rear = NULL;
    }
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
            qindx = auxinfo.type;
            dtime = auxinfo.time;
            tottime = tottime + (dtime-auxinfo.duration);
            count++;
            
            depart(qindx, dtime);
        }
    }

    printf("total time is %4.2f\n", tottime);
    printf("average time is %4.2f\n", tottime/count);

    return 0;
}

void arrive(int atime, int dur) {
    NODEPTR p;
    int j, small;
    /* find the shortest queue */
    j = 0;
    small = q[0].num;
    for (int i=1; i<num_tellers; i++) {
        if (q[i].num < small) {
            small=q[i].num;
            j = i;
        }
    } /* queue j is the shortest */

    /* insert a new customer node */
    auxinfo.time = atime;
    auxinfo.duration = dur;
    auxinfo.type = j;
    insert(&q[j], &auxinfo);
    /* Check if this is the only node on the queue. If it is, */
    /* the customer's departure node must be placed on the event list */
    if (q[j].num == 1) {
        p = q[j].front;
        p->time = atime + dur;
        p->duration = atime;
        removequeue(&q[j]);
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

void depart(int qindx, int dtime) {
    NODEPTR p;
    /* If there are any more customers on the queue, */
    /* place the departure of the next customer onto */
    /* the event list after computing its departure time */

    // Remove customer from queue
    (q[qindx].num)--;

    int temp = 0;
    if (q[qindx].num > 0) {
        p = q[qindx].front;
        temp = p->time;
        p->time = dtime + p->duration;
        p->duration = temp;
        removequeue(&q[qindx]);
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

    //(pq->num)--;
}

