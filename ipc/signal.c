#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>

#define SIG_TYPE SIGTSTP
#define SYS_ERR ({printf ("%s:%d: %s\n", argv[0], __LINE__, strerror (errno)); exit (1);})

void handler (int signum)
{
  printf ("Signal Number: %d\n", signum);
}

int main (int argc, char **argv)
{
  struct sigaction sa;
  sigset_t set;
  pid_t pid;

  switch ((pid = fork ()))
    {
    case -1:
      SYS_ERR;
    case 0:
      memset (&sa, 0, sizeof (sa));
      sa.sa_handler = &handler;
      sigaction (SIG_TYPE, &sa, NULL);
      pause ();
      break;
    default:
      sleep (1);
      kill (pid, SIG_TYPE);
      break;
    }

  exit (0);
}
