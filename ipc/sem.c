#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define SYS_ERR ({printf ("%s:%d: %s\n", __FILE__, __LINE__, strerror (errno)); exit (1);})
#define MAX 1

union semun
{
  int val;
  struct semid_ds *buf;
  unsigned short *array;
};

int main (int argc, char **argv)
{
  struct sembuf sem_lock   = { 0, -1, IPC_NOWAIT };
  struct sembuf sem_unlock = { 0,  1, IPC_NOWAIT };
  union semun sem_opts;
  key_t key;
  int i;
  int id;
  int sem_size;
  int sem_val;
  int sem_num;

  if (argc < 2)
    {
      printf ("usage: %s\n", argv[0]);
      printf ("       (c)reate <Semaphore Size>\n", argv[0]);
      printf ("       (l)ock   <Semaphore Index>\n");
      printf ("       (u)nlock <Semaphore Index>\n");
      printf ("       (d)elete\n");
      exit (1);
    }

  // key = ftok (".", '2');
  key = 1;

  switch (tolower (argv[1][0]))
    {
    case 'c':
      sem_size = atoi (argv[2]);

      if ((id = semget (key, sem_size, IPC_CREAT | IPC_EXCL | 0666)) < 0)
        SYS_ERR;

      sem_opts.val = MAX;

      if (semctl (id, 0, SETVAL, sem_opts) < 0)
        SYS_ERR;

      printf ("Semaphore created. Size: %d\n", sem_size);

      break;
    case 'l':
      if ((id = semget (key, 0, 0666)) < 0)
        SYS_ERR;

      sem_lock.sem_num = 0;
      sem_num = atoi (argv[2]);
      sem_val = semctl (id, sem_num, GETVAL, 0);

      if (sem_val == 0)
        {
          printf ("Semaphore[%d] Lock Fail. Resource Count: %d / %d\n", sem_num, sem_val, MAX);
          exit (1);
        }

      semop (id, &sem_lock, 1);

      printf ("Semaphore[%d] Lock Success, Resource Count: %d / %d\n", sem_num, sem_val - 1, MAX);

      break;
    case 'u':
      if ((id = semget (key, 0, 0666)) < 0)
        SYS_ERR;

      sem_unlock.sem_num = 0;
      sem_num = atoi (argv[2]);
      sem_val = semctl (id, sem_num, GETVAL, 0);

      if (sem_val == MAX)
        {
          printf ("Semaphore[%d] Unlock Fail. Resource Count: %d / %d\n", sem_num, sem_val, MAX);
          exit (1);
        }

      semop (id, &sem_unlock, 1);

      printf ("Semaphore[%d] Unlock Success, Resource Count: %d / %d\n", sem_num, sem_val + 1, MAX);

      break;
    case 'd':
      if ((id = semget (key, 0, 0666)) < 0)
        SYS_ERR;

      if (semctl (id, 0, IPC_RMID, 0) < 0)
        SYS_ERR;

      printf ("Semaphore removed.\n");

      break;
    default:
      SYS_ERR;
  }

  exit (0);
}
