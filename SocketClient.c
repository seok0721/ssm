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
  struct sockaddr_un clntAddr;
  int clntFd;
  char buf[BUFSIZ];
  socklen_t clntAddrLen;
  int len;

  clntAddrLen = sizeof (clntAddr);

  if ((clntFd = socket (AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
      perror ("socket");
      exit (1);
    }

  memset (&clntAddr, 0, sizeof (clntAddr));

  clntAddr.sun_family = AF_UNIX;
  strcpy (clntAddr.sun_path, PATH);

  if (connect (clntFd, (struct sockaddr *) &clntAddr, clntAddrLen) < 0)
    {
      perror ("connect");
      exit (1);
    }

  printf ("Client Send: ");

  len = scanf ("%s", (char *) &buf);
  write (clntFd, buf, len);
  read (clntFd, buf, BUFSIZ);

  printf ("Client Receive: %s\n", buf);

  close (clntFd);

  exit (0);
}
