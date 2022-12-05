#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

int nbre_philosophes;

pthread_mutex_t *baguette;

void mange(int id) {}

void* philosophe (void* arg)
{
  int *id=(int *) arg;
  int left = *id;
  int right = (left + 1) % nbre_philosophes;
  for(int i=0; i<100000; i++) {
    if(left<right) {
      pthread_mutex_lock(&baguette[left]);
      pthread_mutex_lock(&baguette[right]);
    }
    else {
      pthread_mutex_lock(&baguette[right]);
      pthread_mutex_lock(&baguette[left]);
    }
    mange(*id);
    pthread_mutex_unlock(&baguette[left]);
    pthread_mutex_unlock(&baguette[right]);
  }
  return (NULL);
}

// argv[1] = nombre de philosophes
int main(int argc, char *argv[])
{
  if (argc != 2) {
    printf("Nombre d'arguments erroné\n");
    return(EXIT_FAILURE);
  }

    nbre_philosophes = atoi(argv[1]);

     if (nbre_philosophes < 2) {
      printf("Nombre de philosophes insuffisant\n");
      return(EXIT_FAILURE);
    }

    baguette = malloc(nbre_philosophes*sizeof(pthread_mutex_t));

    pthread_t phil[nbre_philosophes];

    long i;
    int id[nbre_philosophes];

    for (i = 0; i < nbre_philosophes; i++) {
        id[i] = i;
    }

    for (i = 0; i < nbre_philosophes; i++) {
        pthread_mutex_init(&baguette[i], NULL);
    }

    for (i = 0; i < nbre_philosophes; i++) {
        pthread_create(&phil[i], NULL, philosophe, (void *)&(id[i]));
    }

    for (i = 0; i < nbre_philosophes; i++) {
        pthread_join(phil[i], NULL);
    }

    free(baguette);

    return (EXIT_SUCCESS);
}
