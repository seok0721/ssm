#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define SYS_ERR ({printf ("%s:%d: %s\n", argv[0], __LINE__, strerror (errno)); exit (1);})

int main (int argc, char **argv)
{
  struct msqid_ds ds;
  char buf[BUFSIZ];
  key_t key;
  int id;

  if (argc < 2)
    {
      printf ("usage: %s\n", argv[0]);
      printf ("       (e)nqueue <Message>\n", argv[0]);
      printf ("       (d)equeue\n");
      printf ("       (s)how\n");
      printf ("       (c)lear\n");
      exit (1);
    }

  key = ftok (".", '1');

  if ((id = msgget (key, IPC_CREAT | 0666)) < 0)
    SYS_ERR;

  if (msgctl (id, IPC_STAT, &ds) < 0)
    SYS_ERR;

  switch (tolower (argv[1][0]))
    {
    case 'e':
      if (msgsnd (id, argv[2], strlen (argv[2]) + 1, 0) < 0)
        SYS_ERR;

      printf ("Message queue element count: %ld\n", ds.msg_qnum + 1);
      break;
    case 'd':
      if (ds.msg_qnum == 0)
        {
          printf ("Message queue is empty.\n");
          exit (0);
        }

      if (msgrcv (id, buf, BUFSIZ, 0, 0) < 0)
        SYS_ERR;

      printf ("Message: %s\n", buf);
      printf ("Remain queue element count: %ld\n", ds.msg_qnum - 1);
      break;
    case 's':
      printf ("Message queue element count: %ld\n", ds.msg_qnum);
      break;
    case 'c':
      if (msgctl (id, IPC_RMID, 0) < 0)
        SYS_ERR;
      printf ("Message queue clear.\n");
      break;
    default:
      SYS_ERR;
  }

  exit (0);
}
