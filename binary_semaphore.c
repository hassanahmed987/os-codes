#include <semaphore.h>
#include <stdio.h>
#include <pthread.h>

sem_t binary_sem;

void* process(void* arg) {
    sem_wait(&binary_sem); // Decrement (Wait)
    
    // Critical Section
    printf("This is the CS (Thread %ld)\n", (long)arg);
    
    sem_post(&binary_sem); // Increment (Signal)
    return NULL;
}

int main() {
    pthread_t t1, t2;
    // Initialize semaphore: (pointer, shared_between_processes, initial_value)
    sem_init(&binary_sem, 0, 1);

    pthread_create(&t1, NULL, process, (void*)1);
    pthread_create(&t2, NULL, process, (void*)2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&binary_sem);
    return 0;
}
