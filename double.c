// CSCI-340 - Homework #7

// Starter file for the double linked list


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


// basic node structure

typedef struct __node_t {
    
    int key;
    struct __node_t *next;
    struct __node_t *prev;

} node_t;

// basic list structure (one used per list)

typedef struct __list_t {
    
    node_t *head;
    node_t *tail;
    pthread_mutex_t lock;
    
} list_t;

// Initialize the list

void List_Init(list_t *L) {

    // Add code here to initialize the list
    L->head = NULL;
    L->tail = NULL;
    pthread_mutex_init(&L->lock, NULL);

    
}

// Insert into the list (At the front)

void List_Insert(list_t *L,
                 int key) {

    // Add code here to safely insert a new node at the beginning of the list
    
    node_t *new = malloc(sizeof(node_t)); 
      
    if (new == NULL) {
        perror("malloc");
        return;
    }
    
    new->key = key;
    

    pthread_mutex_lock(&L->lock);
    // make new node next to old head and prev to null 
    new->next = L->head;
    new->prev = NULL;

    //make old head no longer head an new head the head
    L->head = new;

    //make old head prev point to new head
    if(L->head->next != NULL){
        L->head->next->prev = new;
    }
    else{
        L->tail = new;
    }
    pthread_mutex_unlock(&L->lock);
    
}

// Insert into the list (At the end)

void List_Append(list_t *L,
                 int key) {

    // Add code here to safely insert a new node at the end of the list
    node_t *new = malloc(sizeof(node_t));   
    if (new == NULL) {
        perror("malloc");
        return;
    }
    new->key = key;

    pthread_mutex_lock(&L->lock);
    // make new node next to null and new node prev to old tail 
    new->next = NULL;
    new->prev = L->tail;

    //make old tail no longer tail an new tail the tail
    L->tail = new;
    
    //make old tail next point to new tail
    if(L->tail->prev != NULL){
        L->tail->prev->next = new;
    }
    else{
        L->head = new;
    }
    pthread_mutex_unlock(&L->lock);
    
}

int List_Lookup(list_t *L,
                int key) {

    // Add code here to lookup an item in the list
    int rv = -1;
    pthread_mutex_lock(&L->lock);
    node_t *curr = L->head;
    while (curr) {
        if (curr->key == key) {
            rv = 0;
            break;
        }
        curr = curr->next;
    }
    pthread_mutex_unlock(&L->lock);
    return rv;
    
}

void list_display(list_t *L){
// the print is not thread safe its not even a thread operation 

    node_t * temp = L->head;  
    printf("\nTraversal in forward direction \n");
    if(L->head != NULL) 
        printf("head<%d> \n", L->head->key); 
    while (temp != NULL) {  
        printf(" %d ", temp->key);  
        temp = temp->next;
    }  
  
    temp = L->tail;
    printf("\nTraversal in reverse direction \n");
    if(L->head != NULL)  
        printf("tail<%d> \n", L->tail->key);
    while (temp != NULL) {  
        printf(" %d ", temp->key);  
        temp = temp->prev;  
    }
}

int main()
{

    // Add code here to test your list
    list_t *dl_list;
    dl_list = malloc(sizeof(list_t));
    List_Init(dl_list);

    printf("empty queue\n");
    list_display(dl_list);




    List_Insert(dl_list, 1);
    List_Insert(dl_list, 2);
    List_Insert(dl_list, 3);


    List_Append(dl_list, 4);
    List_Append(dl_list, 5);
    List_Append(dl_list, 6);

    printf("full queue\n");
    list_display(dl_list);

    return 0;

    
}
