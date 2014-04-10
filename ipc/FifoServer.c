#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/stat.h>

#define FIFO_FILE "MYFIFO"

int main (void)
{
  FILE *fp;
  char buf[BUFSIZ];

  /* Create the FIFO if it does not exist */
  umask (0);
  mknod (FIFO_FILE, S_IFIFO | 0644, 0);

  while (1)
    {
      fp = fopen (FIFO_FILE, "r");
      fgets (buf, BUFSIZ, fp);
      printf ("FIFO Receive Message: %s\n", buf);
      fclose (fp);
    }

  return 0;
}
