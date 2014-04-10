#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <syscall.h>

#define SYS_ERR ({printf ("%s\n", strerror (errno)); exit (1);})

void *show_process_and_thread_info (void *);
void *create_new_process (void *);

int main (void)
{
  pid_t pid ;
  pthread_t thread;

  if (pthread_create (&thread, NULL, create_new_process, "New Thread") < 0)
    SYS_ERR;
  if (pthread_join (thread, NULL) < 0)
    SYS_ERR;

  create_new_process ("Main Thread");

  return 0;
}

void *create_new_process (void *prefix)
{
  char str[100];
  pid_t pid;

  memcpy (&str[0], prefix, strlen ((char *) prefix));

  switch ((pid = fork ()))
    {
    case -1:
      SYS_ERR;
    case 0:
      show_process_and_thread_info (strcat (str, " - Child Process"));
      break;
    default:
      show_process_and_thread_info (strcat (str, " - Parent Process"));
      break;
    }
}

void *show_process_and_thread_info (void *prefix)
{
  printf ("[%s] TID: %lu, PID: %u, PPID: %u, PGID: %u\n", (char *) prefix,
          syscall (SYS_gettid), getpid (), getppid (), getpgrp ());
}
