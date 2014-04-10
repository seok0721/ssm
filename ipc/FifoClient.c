#include <stdio.h>
#include <stdlib.h>

#define FIFO_FILE "MYFIFO"

int main (int argc, char *argv[])
{
  FILE *fp;

  if (argc != 2)
    {
      printf ("usage: %s <message>\n", argv[0]);
      exit (1);
    }

  if ((fp = fopen (FIFO_FILE, "w")) == NULL)
    {
      perror ("fopen");
      exit (1);
    }

  printf ("FIFO Send Message: %s\n", argv[1]);
  fputs (argv[1], fp);
  fclose (fp);

  return 0;
}
