#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define PHILOSOPHES 3

pthread_t phil[PHILOSOPHES];
pthread_mutex_t baguette[PHILOSOPHES];

void mange(int id) {
  printf("Philosophe [%d] mange\n",id);
  for(int i=0;i< rand(); i++) {
    // philosophe mange
  }
}

void* philosophe ( void* arg )
{
  int *id=(int *) arg;
  int left = *id;
  int right = (left + 1) % PHILOSOPHES;
  for(int i=0; i<3; i++) {
    printf("Philosophe [%d] pense\n",*id);
    if(left<right) {
      pthread_mutex_lock(&baguette[left]);
      printf("Philosophe [%d] possède baguette gauche [%d]\n",*id,left);
      pthread_mutex_lock(&baguette[right]);
      printf("Philosophe [%d] possède baguette droite [%d]\n",*id,right);
    }
    else {
      pthread_mutex_lock(&baguette[right]);
      printf("Philosophe [%d] possède baguette droite [%d]\n",*id,right);
      pthread_mutex_lock(&baguette[left]);
      printf("Philosophe [%d] possède baguette gauche [%d]\n",*id,left);
    }
    mange(*id);
    pthread_mutex_unlock(&baguette[left]);
    pthread_mutex_unlock(&baguette[right]);
  }
  return (NULL);
}

int main(int argc, char *argv[])
{
    int nbre_philosophes;
    printf("Combien de philosophes ? : \n");
    scanf("%d", &nbre_philosophes);

    pthread_t phil[nbre_philosophes];
    pthread_mutex_t baguette[nbre_philosophes];

    long i;
    int id[nbre_philosophes];

    srand(getpid());

    struct parametres {
        pthread_mutex_t baguette[nbre_philosophes];
    };

    for (i = 0; i < nbre_philosophes; i++)
        id[i] = i;

    for (i = 0; i < nbre_philosophes; i++)
        pthread_mutex_init(&baguette[i], NULL);

    for (i = 0; i < nbre_philosophes; i++)
        pthread_create(&phil[i], NULL, philosophe, (void *)&(id[i]));

    for (i = 0; i < nbre_philosophes; i++)
        pthread_join(phil[i], NULL);

    return (EXIT_SUCCESS);
}

// gcc task1_1.c task1_1 && ./task1_1