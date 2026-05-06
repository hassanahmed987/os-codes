#include <pthread.h>
#include <stdio.h>

pthread_mutex_t lock;

void* process(void* arg) {
    pthread_mutex_lock(&lock);
    
    // Critical Section
    printf("This is the CS (Thread %ld)\n", (long)arg);
    
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_mutex_init(&lock, NULL);

    pthread_create(&t1, NULL, process, (void*)1);
    pthread_create(&t2, NULL, process, (void*)2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock);
    return 0;
}
