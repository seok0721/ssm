#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define SYS_ERR ({printf ("%s:%d: %s\n", argv[0], __LINE__, strerror (errno)); exit (1);})
#define IP "127.0.0.1"
#define PORT 50000

int main (int argc, char **argv)
{
  struct sockaddr_in servAddr;
  struct sockaddr_in acptAddr;
  struct sockaddr_in clntAddr;
  int servFd;
  int acptFd;
  int clntFd;
  char buf[BUFSIZ];
  int ret;

  if (argc < 2)
    {
      printf ("usage: %s <Message>\n", argv[0]);
      exit (1);
    }

  switch (fork ())
    {
    case -1:
      SYS_ERR;
    case 0:
      if ((servFd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        SYS_ERR;

      memset (&servAddr, 0, sizeof (servAddr));

      servAddr.sin_family = AF_INET;
      servAddr.sin_port = htons (PORT);
      servAddr.sin_addr.s_addr = htonl (INADDR_ANY);

      if (bind (servFd, (struct sockaddr *) &servAddr, sizeof (servAddr)) < 0)
        SYS_ERR;

      if (listen (servFd, 1) < 0)
        SYS_ERR;
      
      if ((acptFd = accept (servFd, NULL, NULL)) < 0)
        {
          close (servFd);
          SYS_ERR;
        }

      while ((ret = read (acptFd, &buf, BUFSIZ)) > 0)
        write (acptFd, &buf, ret);

      close (acptFd);
      close (servFd);

      break;
    default:
      sleep (1);

      if ((clntFd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        SYS_ERR;

      memset (&clntAddr, 0, sizeof (clntAddr));

      clntAddr.sin_family = AF_INET;
      clntAddr.sin_port = htons (PORT);

      inet_pton (AF_INET, IP, &clntAddr.sin_addr);

      if ((connect (clntFd,  (struct sockaddr *) &clntAddr, (socklen_t) sizeof (clntAddr)) < 0))
        SYS_ERR;

      write (clntFd, argv[1], strlen (argv[1]));
      ret = read (clntFd, buf, BUFSIZ);

      write (STDOUT_FILENO, buf, ret);
      write (STDOUT_FILENO, "\n", 1);

      close (clntFd);

      break;
    }

  exit (0);
}
