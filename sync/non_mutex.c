#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int thread_count = 16;
int shared_number = 0;

void *start_routine (void *);

int main (int argc, char **argv)
{
  pthread_t tid[16];
  int i;
  int index[16];

  for (i = 0; i < thread_count; i++)
    {
      index[i] = i;
      pthread_create (&tid[i], NULL, start_routine, &index[i]);
    }

  pause ();

  return 0;
}

void *start_routine (void *index)
{
  printf ("[Thread %2d] Shared Number: %d\n", *((int *) index), shared_number);
  shared_number++; // Critical Section.
}
