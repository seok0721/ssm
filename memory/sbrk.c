#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void usage (char *program_name)
{
  printf ("usage: %s [Break Count] [Break Size]\n", program_name);
}

int main (int argc, char **argv)
{
  int i, cnt, len;
  void *ptr;

  if (argc > 1)
    {
      cnt = atoi (argv[1]);
      len = atoi (argv[2]);

      if (cnt < 0 || len < 0)
        {
          usage (argv[0]);
          exit (1);
        }

      puts ("[Call \"sbrk\"]========================\n");

      for (i = 0; i < cnt; i++)
        {
          printf ("%d: %p\n", i, sbrk (len));
        }

      puts ("\n[Call \"malloc\"]========================\n");

      for (i = 0; i < cnt; i++)
        {
          printf ("%d: %p\n", i, malloc (len));
        }

      puts ("");
    }
  else
    {
      usage (argv[0]);
    }

  return 0;
}
