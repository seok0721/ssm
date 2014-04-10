#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <syscall.h>
#include <fcntl.h>

#define SYS_ERR ({printf ("%d: %s\n", __LINE__, strerror (errno)); exit (1);})

int fd;
char *buf = "Hello, World!\n";

void *run (void *);

int main (void)
{
  pthread_t thread;

  if ((fd = open ("TEMP_FILE", O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR)) < 1)
    SYS_ERR;

  if (pthread_create (&thread, NULL, run, NULL) < 0)
    SYS_ERR;

  printf ("Main Thread - FD: %d, FD Status: %s\n", fd, (fcntl (fd, F_GETFD) < 0) ? "close" : "open");
  if (close (fd) < 0)
    SYS_ERR;

  if (pthread_join (thread, NULL) < 0)
    SYS_ERR;

  return 0;
}

void *run (void *arg)
{
  sleep (3);

  printf ("New Thread - FD: %d, FD Status: %s\n", fd, (fcntl (fd, F_GETFD) < 0) ? "close" : "open");

  if (write (fd, buf, strlen (buf)) < 0)
    {
      printf ("Child Process Write Failed.\n");
      exit (1);
    }

  if (close (fd) < 0)
    SYS_ERR;
}
