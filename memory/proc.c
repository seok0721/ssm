#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "status.h"

int main (int argc, char **argv)
{
  int i, cnt, len;

  if (argc == 1)
    {
      i = 1;
    }
  else if (argc > 2)
    {
      cnt = atoi (argv[1]);
      len = atoi (argv[2]);

      if (cnt < 0 || len < 0)
        {
          printf ("usage: %s [Block Count] [Block Size]\n", argv[0]);
          exit (1);
        }

      for (i = 0; i < cnt; i++)
        {
          malloc (len);
        }
    }
  else
    {
      printf ("usage: %s [Block Count] [Block Size]\n", argv[0]);
      exit (1);
    }

  get_self_status ();
}
