#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

int main (void)
{
  char *ptr;
  int fd;

  if ((fd = open ("./mmap.c", O_RDONLY)) < 0)
    {
      perror ("open");
      exit (1);
    }

  ptr = mmap (NULL, 1024, PROT_READ, MAP_PRIVATE, fd, 100);

  if (ptr == MAP_FAILED)
    {
      perror ("mmap");
      exit (1);
    }

  write (1, ptr + 100, 100);
  close (fd);

  exit (EXIT_SUCCESS);
}
