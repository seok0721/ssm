#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define SYS_ERR ({printf ("%s:%d: %s\n", argv[0], __LINE__, strerror (errno)); exit (1);})

int main (int argc, char **argv)
{
  char buf[BUFSIZ];
  int fd[2];
  int pid;
  int ret;

  if (argc < 2)
    {
      printf ("usage: %s <Message>\n", argv[0]);
      exit (1);
    }

  if (pipe (fd) < 0)
    SYS_ERR;

  switch (fork ())
    {
    case -1:
      SYS_ERR;
    case 0:
      close (fd[0]);
      write (fd[1], argv[1], strlen (argv[1]));
      close (fd[1]);
      break;
    default:
      close (fd[1]);
      while ((ret = read (fd[0], buf, BUFSIZ)) > 0)
        {
          write (STDOUT_FILENO, &buf, ret);
        }
      write (STDOUT_FILENO, "\n", 1);
      close (fd[0]);
      break;
    }

  exit (0);
}
