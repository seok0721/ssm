#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MAX 100
#define SYS_ERR ({printf ("%s:%d: %s\n", argv[0], __LINE__, strerror (errno)); exit (1);})

int main (int argc, char **argv)
{
  struct shmid_ds ds;
  char *ptr;
  char buf[BUFSIZ];
  key_t key;
  int id;

  if (argc < 2)
    {
      printf ("usage: %s\n", argv[0]);
      printf ("       (w)rite <Message>\n", argv[0]);
      printf ("       (r)ead\n");
      printf ("       (c)lear\n");
      exit (1);
    }

  key = ftok (".", '3');

  if ((id = shmget (key, MAX, IPC_CREAT | IPC_EXCL | 0666)) < 0)
    if ((id = shmget (key, MAX, 0)) < 0)
      SYS_ERR;

  if ((ptr = shmat (id, 0, 0)) == NULL)
    SYS_ERR;

  if (shmctl (id, IPC_STAT, &ds) < 0)
    SYS_ERR;

  switch (tolower (argv[1][0]))
    {
    case 'w':
      strcpy (ptr, argv[2]);
      printf ("Shared memory segment: %s\n", argv[2]);
      break;
    case 'r':
      printf ("Shared memory segment: %s\n", ptr);
      break;
    case 'c':
      if (shmctl (id, IPC_RMID, 0) < 0)
        SYS_ERR;
      printf ("Shared memory clear.\n");
      break;
    default:
      SYS_ERR;
  }

  exit (0);
}
