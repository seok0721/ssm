#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <syscall.h>

#define SYS_ERR ({printf ("%s\n", strerror (errno)); exit (1);})

void *show_process_and_thread_info (void *);

int main (void)
{
  pid_t pid ;
  pthread_t t1, t2;

  switch ((pid = fork ()))
    {
    case -1:
      SYS_ERR;
    case 0:
      if (pthread_create (&t1, NULL, show_process_and_thread_info, "CHILD") < 0)
        SYS_ERR;
      if (pthread_join (t1, NULL) < 0)
        SYS_ERR;
      show_process_and_thread_info ("CHILD");
      break;
    default:
      if (pthread_create (&t2, NULL, show_process_and_thread_info, "PARENT") < 0)
        SYS_ERR;
      if (pthread_join (t2, NULL) < 0)
        SYS_ERR;
      show_process_and_thread_info ("PARENT");
      break;
    }

  return 0;
}

void *show_process_and_thread_info (void *prefix)
{
  printf ("[%s] TID: %lu, PID: %u, PPID: %u, PGID: %u\n", (char *) prefix,
          syscall (SYS_gettid), getpid (), getppid (), getpgrp ());
}
