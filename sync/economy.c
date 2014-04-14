#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define BUFFER_SIZE 10

void *action_producer (void *);
void *action_consumer (void *);
int produce (void);

int buffer[BUFFER_SIZE];
int counter = 0;

int main (void)
{
  pthread_t thread1;
  pthread_t thread2;

  pthread_create (&thread1, NULL, action_producer, NULL);
  pthread_create (&thread2, NULL, action_consumer, NULL);

  pause ();

  return 0;
}

void *action_consumer (void *arg)
{
  int next_consumed;
  int out = 0;

  while (1)
    {
      while (counter == 0);

      next_consumed = buffer[out];
      out = (out + 1) % BUFFER_SIZE;
      counter--;

      printf ("Consuming Integer: %d\n", next_consumed);

      sleep (1);
    }
}

void *action_producer (void *arg)
{
  int next_produced;
  int in = 0;

  while (1)
    {
      next_produced = random ();
      printf ("Producing Integer: %d\n", next_produced);

      while (counter == BUFFER_SIZE);

      buffer[in] = next_produced;
      in = (in + 1) % BUFFER_SIZE;
      counter++;

      sleep (1);
    }
}

int produce (void)
{
  srand (time (NULL));

  return rand ();
}
