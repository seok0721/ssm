#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>

int main (int argc, char **argv, char **env)
{
  struct rusage rusage; // Child process resource usage info saved.
  int status; // Child process state changed value.
  int i;
  pid_t pid, child_pid; // PID == Process Id

  switch ((pid = fork ())) // fork: Create new process, Standard C Library function
    {
    case -1: // Error occur when create new process.
      perror ("fork"); // Print error message
      exit (1); // End of program
    case 0:
      for (i = 0; i < 100000000; i++); // Child process run dummy code...
      puts ("--------------------------------------------------");
      if (argc >= 2)
        {
          if (execve (argv[1], &argv[1], env) < 0) // Execute program on child process.
            {
              perror (argv[1]);
              exit (1);
            }
        }
      else
        {
          printf ("Child process no execute binary\n");
        }
      break;
    default:
      puts ("--------------------------------------------------");
      printf ("Parent Process Id: %d\n", getpid ());
      printf ("Parent Process Group Id: %d\n", getpgrp ());
      printf ("Parent Session Id: %d\n", getsid (getpid ()));
      printf ("Parent Parent Process Id: %d\n", getppid ());
      printf ("New Child Process Id: %d\n", pid);

      child_pid = wait3 (&status, 0, &rusage); // Wait for child state changed.

      puts ("--------------------------------------------------");
      printf ("Child Return Status: 0x%04x\n", status);
      printf ("State Changed Child Process Id: %d\n", child_pid);
      printf ("Total User Time Used: %ld.%ld\n", rusage.ru_utime.tv_sec,
              rusage.ru_utime.tv_usec);
      printf ("Total System Time Used: %ld.%ld\n", rusage.ru_stime.tv_sec,
              rusage.ru_stime.tv_usec);
      puts ("--------------------------------------------------");

      // Other resource refer to "/usr/include/$ARCH/bits/resource.h

      break;
    }

  exit (0); // Child process exit. After parent process exit.
}
