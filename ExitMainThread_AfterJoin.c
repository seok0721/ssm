#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <syscall.h>

#define SYS_ERR ({printf ("%s\n", strerror (errno)); exit (1);})

void *run (void *);

int main (void)
{
  pthread_t t1, t2;

  if (pthread_create (&t1, NULL, run, NULL) < 0)
    SYS_ERR;
  if (pthread_join (t1, NULL) < 0)
    SYS_ERR;

  if (pthread_create (&t2, NULL, run, NULL) < 0)
    SYS_ERR;
  if (pthread_join (t2, NULL) < 0)
    SYS_ERR;

  printf ("Main Thread Exit.\n");

  return 0;
}

void *run (void *arg)
{
  long tid = syscall (SYS_gettid);

  printf ("Sleep Thread, TID: %lu\n", tid);
  sleep (3);
  printf ("WakeUp Thread, TID: %lu\n", tid);
}
