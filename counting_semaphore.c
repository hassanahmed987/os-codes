#include <semaphore.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

sem_t counting_sem;

void* process(void* arg) {
    sem_wait(&counting_sem);
    
    // Critical Section
    printf("This is the CS: Thread %ld entered.\n", (long)arg);
    sleep(1); // Stay in CS for a moment to demonstrate limit
    printf("Thread %ld is leaving the CS.\n", (long)arg);
    
    sem_post(&counting_sem);
    return NULL;
}

int main() {
    pthread_t threads[5];
    // Initialize with 3: Only 3 threads can enter the CS at once
    sem_init(&counting_sem, 0, 3);

    for (long i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, process, (void*)i);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&counting_sem);
    return 0;
}
