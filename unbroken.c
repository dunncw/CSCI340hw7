#include <assert.h>
#include <pthread.h>
#include <stdlib.h>

/*

2 waysit was broken
1. in the dequeue method you do not lock around the pointer that is passed to get and return the new value of head becuase the queue is mutli threaded you need to lock around this part of the code becuase other threads can be changing what the head pointer points to so this can not be sahred information this need to be locked and kept sepoerate 

2. in the enqueue you locked around thge creation of a new node to be enqueued in the list however this lock is not nessacasry yet as this node does not point to anything and is just floating off in memory somwhere the important part is the pointers when you actaully go to put the node on the list then you need to lock as you dont want other threads messing with where this new thread will point

*/

typedef struct __node_t {
    
    int               value;
    struct __node_t * next;
    
} node_t;

typedef struct { // forgot to type queue_t here to name the struct
    
    node_t *        head;
    node_t *        tail;
    pthread_mutex_t head_lock;
    pthread_mutex_t tail_lock;

} queue_t;

void Queue_Init(queue_t *q) {
    
    node_t *tmp = malloc(sizeof(node_t));
    tmp->next = NULL;
    q->head = q->tail = tmp;
    
    pthread_mutex_init(&q->head_lock, NULL);
    pthread_mutex_init(&q->tail_lock, NULL);
    
}

void Queue_Enqueue(queue_t * q,
                   int       value) {
    
    //pthread_mutex_lock(&q->tail_lock); // you dont need to lock yet
    node_t *tmp = malloc(sizeof(node_t));         /* Get a new node */
    if (tmp == NULL) {                            /* Did the allocation fail? */
        perror("malloc");
        return -1;
    }

    assert(tmp != NULL);
    tmp->value = value;                           /* Set the node's contents */
    tmp->next  = NULL;                            /* Show it's the tail */

    pthread_mutex_lock(&q->tail_lock);          // this is where the lock should be placed
    q->tail->next = tmp;                          /* Point old tail to new tail */
    q->tail = tmp;                                /* Point tail to new node */
    pthread_mutex_unlock(&q->tail_lock);
    
}

int Queue_Dequeue(queue_t * q,
                  int *     value) {

    // Failure is always an option
    int rc = -1;
    
    
    if (q_head->next != NULL) {
        pthread_mutex_lock(&q->head_lock);// this is wehre the lock should be
        *value = q_head->next->value;             /* Return the value */
        //pthread_mutex_lock(&q->head_lock); //lock used to be here       /* Lock the list */
        node_t *tmp = q->head;                    /* Save the old head node pointer */
        q->head = q_head->next;                   /* Reset the head */
        pthread_mutex_unlock(&q->head_lock);      /* Unlock the list */
        free(tmp);                                /* Free the old head node */
        rc = 0;
    }
    
    
    return rc;
    
}

