#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t resource_key;    
pthread_mutex_t count_lock; 
int read_count = 0;

void* reader(void* arg) {
    // Entry
    pthread_mutex_lock(&count_lock);
    read_count++;
    if (read_count == 1) sem_wait(&resource_key);
    pthread_mutex_unlock(&count_lock);

    // Critical Section
    printf("This is the CS (Reader)\n");

    // Exit
    pthread_mutex_lock(&count_lock);
    read_count--;
    if (read_count == 0) sem_post(&resource_key);
    pthread_mutex_unlock(&count_lock);
    
    return NULL;
}

void* writer(void* arg) {
    // Entry
    sem_wait(&resource_key);

    // Critical Section
    printf("This is the CS (Writer)\n");

    // Exit
    sem_post(&resource_key);
    
    return NULL;
}

int main() {
    pthread_t r, w;
    sem_init(&resource_key, 0, 1);
    pthread_mutex_init(&count_lock, NULL);

    pthread_create(&w, NULL, writer, NULL);
    pthread_create(&r, NULL, reader, NULL);

    pthread_join(w, NULL);
    pthread_join(r, NULL);

    sem_destroy(&resource_key);
    pthread_mutex_destroy(&count_lock);
    return 0;
}
