#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define MSG "Hello, world!"

int main(void)
{
  int fd[2], nbytes;
  pid_t pid;
  char buf[BUFSIZ];

  pipe (fd);
  
  if ((pid = fork ()) < 0)
    {
      perror ("fork");
      exit (1);
    }

  if (pid == 0)
    {
      /* Child process closes up input side of pipe */
      close (fd[0]);

      /* Send "string" through the output side of pipe */
      printf ("PID: %d, Pipe Send Message: %s\n", getpid (), MSG);
      write (fd[1], MSG, (strlen (MSG) + 1));
    }
  else
    {
      /* Parent process closes up output side of pipe */
      close (fd[1]);

      /* Read in a string from the pipe */
      read (fd[0], buf, sizeof (buf));
      printf ("PID: %d, Pipe Receive Message:: %s\n", getpid (), buf);
    }
  
  return 0;
}
