#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>


//sem_t mutex;
int thread = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;

void *child(void* arg) {
    int num = *((int *) arg);

    assert(pthread_mutex_lock(&m) == 0);
    
    thread = num;
    pthread_cond_signal(&c);
    
    pthread_mutex_unlock(&m);

    free(arg);
    return NULL;
}

int main(int argc, char *argv[]) {

    //Sem_init(&mutex, 1);

    pthread_t p1;
    int *arg_1 = malloc(sizeof(*arg_1));
    *arg_1 = 1;
    if (pthread_create(&p1, NULL, child, arg_1) != 0) {
        printf("Unable to create thread\n");
        exit(1);
    }

    pthread_t p2;   
    int *arg_2 = malloc(sizeof(*arg_2));
    *arg_2 = 2;
    if (pthread_create(&p2, NULL, child, arg_2) != 0) {
        printf("Unable to create thread\n");
        exit(1);
    }

     pthread_mutex_lock(&m);
    
    while (thread == 0)
        pthread_cond_wait(&c, &m);
    
    pthread_mutex_unlock(&m);


    assert(pthread_join(p1, NULL) == 0);
    assert(pthread_join(p2, NULL) == 0);

    printf("thread that signaled<%d>\n", thread);    

    return 0;

}
