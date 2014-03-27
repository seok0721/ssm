#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>

void *run (void *);

int main (void)
{
  pthread_t thread;
  int i;

  for (i = 0; i < 10; i++)
    {
      if (pthread_create (&thread, NULL, run, NULL) < 0) // Create new thread.
        {
          perror ("pthread_create");
          exit (1);
        }

      if (pthread_join (thread, NULL) < 0) // Join to new thread. Therefore wait for thread complete.
        {
          perror ("pthread_join");
          exit (1);
        }
    }

  run (NULL);

  return 0;
}

// Print process id and thread id. Process id and thread id are same in linux.
void *run (void *arg)
{
  // "syscall" function is direct system call.
  printf ("Process Id: %u, Thread Id: %lu\n", getpid (), syscall (SYS_gettid));
}
