#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

pthread_mutex_t mutex_writecount;
pthread_mutex_t mutex_readcount;

int writecount;
int readcount;

sem_t rsem;
sem_t wsem;

#define totalW 640
#define totalR 2560

void writer(void) 
{
    int count = 0;
    while(count < totalW) {
        pthread_mutex_lock(&mutex_writecount);

        writecount++;

        if (writecount==1) {
            sem_wait(&rsem);
        }

        pthread_mutex_unlock(&mutex_writecount);
        sem_wait(&wsem);

        for (int i=0; i<10000; i++);

        sem_post(&wsem);
        pthread_mutex_lock(&mutex_writecount);

        writecount--;

        if (writecount==0) {
            sem_post(&rsem);
        }

        pthread_mutex_unlock(&mutex_writecount);
        
        count++;
    }
}

void reader(void) 
{
    int count = 0;
    while(count < totalR) {
        sem_wait(&rsem);
        pthread_mutex_lock(&mutex_readcount);

        readcount++;

        if (readcount==1) {
            sem_wait(&wsem);
        }
        pthread_mutex_unlock(&mutex_readcount);
        sem_post(&rsem);

        for (int i=0; i<10000; i++);

        pthread_mutex_lock(&mutex_readcount);

        readcount--;

        if (readcount==0) {
            sem_post(&wsem);
        }
        
        pthread_mutex_unlock(&mutex_readcount);

        count++;
    }
}

int main(int argc, char *argv[]) 
{
    if(argc != 3){
        printf("Nombre d'arguments erroné\n");
        return(EXIT_FAILURE);
    }

    int nbre_writers = atoi(argv[1]);
    int nbre_readers = atoi(argv[2]);

    pthread_t writers[nbre_writers];
    pthread_t readers[nbre_readers];

    if (nbre_writers < 1) {
        printf("Nombre d'écrivains insuffisant\n");
        return(EXIT_FAILURE);
    }

    if (nbre_readers < 1) {
        printf("Nombre de lecteurs insuffisant\n");
        return(EXIT_FAILURE);
    }
    sem_init(&wsem,0,1);
    sem_init(&rsem,0,1);

    pthread_mutex_init(&mutex_writecount, NULL);
    pthread_mutex_init(&mutex_readcount,NULL);

    for(int i = 0; i < nbre_writers; i++) {
        pthread_create(&writers[i], NULL, (void *)reader, NULL);
    }
    for(int i = 0; i < nbre_readers; i++) {
        pthread_create(&readers[i], NULL, (void *)writer, NULL);
    }
    
    for(int i = 0; i < nbre_writers; i++) {
        pthread_join(writers[i], NULL);
    }
    for(int i = 0; i < nbre_readers; i++) {
        pthread_join(readers[i], NULL);
    }
    
    pthread_mutex_destroy(&mutex_writecount);
    pthread_mutex_destroy(&mutex_readcount);

    sem_destroy(&wsem);
    sem_destroy(&rsem);

    return (EXIT_SUCCESS);
}
