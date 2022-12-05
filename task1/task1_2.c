#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>


#define Array_places 8
int buffer[Array_places];
int nb_prod;
int nb_cons;

int index_in = 0;
int index_out = 0;

// Déclaration d'un mutex pour protéger la section critique des buffers    
pthread_mutex_t mutex;

// Déclaration des sémaphores empty (permettant aux producteurs 
//de voir s'ils peuvent produire)
// et full (permettant aux consommateurs de voir s'ils
// peuvent consommer)
sem_t buffer_empty;
sem_t buffer_full;

int total = 0;

void producer(void){
    for(int i=0; i < 8192; i++){
        for (int j=0; j<10000; j++);
        int item = rand();
        sem_wait(&buffer_empty);
        pthread_mutex_lock(&mutex);
        buffer[index_in] = item;
        index_in = (index_in+1)%Array_places;
        pthread_mutex_unlock(&mutex);
        sem_post(&buffer_full);
    }
}

void consumer(void){
    for(int i=0; i < 8192; i++){
        for (int j=0; j<10000; j++);
        sem_wait(&buffer_full);
        pthread_mutex_lock(&mutex);
        int item = buffer[index_out];
        index_out = (index_out+1)%Array_places;
        pthread_mutex_unlock(&mutex);
        sem_post(&buffer_empty);
    }
}

// argv[1] : Nombre de consommateurs, argv[2] : Nombre de producteurs
int main(int argc, char *argv[]){
    pthread_t consumers[atoi(argv[1])];
    pthread_t producers[atoi(argv[2])];

    int i;
    int number[atoi(argv[1])];
    for(i=1; i<atoi(argv[1]); i++){
        number[i] = i;
    }

    sem_init(&buffer_empty, 0, Array_places);
    sem_init(&buffer_full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    for(int i = 0; i < 5; i++) {
        pthread_create(&producers[i], NULL, (void *)producer, (void *)&number[i]);
    }
    for(int i = 0; i < 5; i++) {
        pthread_create(&consumers[i], NULL, (void *)consumer, (void *)&number[i]);
    }

    for(int i = 0; i < 5; i++) {
        pthread_join(producers[i], NULL);
    }
    for(int i = 0; i < 5; i++) {
        pthread_join(consumers[i], NULL);
    }

    return EXIT_SUCCESS;
}
