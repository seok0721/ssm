#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "status.h"

int get_self_status (void)
{
  int ret;
  int fd;
  char buf[BUFSIZ];

  if ((fd = open ("/proc/self/status", O_RDONLY)) < 0)
    {
      perror ("open");
      return 1;
    }

  while ((ret = read (fd, buf, BUFSIZ)) > 0)
    {
      write (1, buf, ret);
    }

  close (fd);

  return 0;
}
