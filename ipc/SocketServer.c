#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#define PATH "UNIX_SOCKET"

int main (int argc, char **argv)
{
  struct sockaddr_un servAddr, clntAddr;
  int servFd, clntFd;
  char buf[BUFSIZ];
  socklen_t clntAddrLen;

  clntAddrLen = sizeof (clntAddr);

  if ((servFd = socket (AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
      perror ("socket");
      exit (1);
    }

  memset (&servAddr, 0, sizeof (servAddr));

  servAddr.sun_family = AF_UNIX;
  strcpy (servAddr.sun_path, PATH);

  if (bind (servFd, (struct sockaddr *) &servAddr, sizeof (servAddr)) < 0)
    {
      perror ("bind");
      exit (1);
    }

  if (listen (servFd, 5) < 0)
    {
      perror ("listen");
      exit (1);
    }

  for (;;)
    {
      if ((clntFd = accept (servFd, (struct sockaddr *) &clntAddr, &clntAddrLen)) < 0)
        {
          perror ("accept");
          exit (1);
        }

      if (fork () == 0)
        {
          for (;;)
            {
              memset (buf, 0, BUFSIZ);
              if (read (clntFd, buf, BUFSIZ) < 0)
                {
                  close (clntFd);
                  exit (0);
                }
//              printf ("Server Receive: %s\n", buf);
              write (clntFd, buf, strlen (buf));
            }
        }
    }

  exit (0);
}
