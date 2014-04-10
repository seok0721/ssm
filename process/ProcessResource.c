#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <syscall.h>
#include <fcntl.h>

#define SYS_ERR ({printf ("%d: %s\n", __LINE__, strerror (errno)); exit (1);})

int main (void)
{
  int fd;
  char *buf = "Hello, World!\n";
  pid_t pid;

  if ((fd = open ("TEMP_FILE", O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR)) < 1)
    SYS_ERR;

  if ((pid = fork ()) < 0)
    SYS_ERR;

  if (pid == 0)
    {
      sleep (3);

      printf ("Child - FD: %d, FD Status: %s\n", fd, (fcntl (fd, F_GETFD) < 0) ? "close" : "open");

      if (write (fd, buf, strlen (buf)) < 0)
        {
          printf ("Child Process Write Failed.\n");
          exit (1);
        }
    }
  else
    {
      if (close (fd) < 0)
        SYS_ERR;

      printf ("Parent - FD: %d, FD Status: %s\n", fd, (fcntl (fd, F_GETFD) < 0) ? "close" : "open");

      wait ();
    }

  return 0;
}
