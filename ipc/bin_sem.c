#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <pthread.h>

int thread_count = 16;
int shared_number = 0;
int id;

void *start_routine (void *);

struct sembuf sem_lock   = { 0, -1, IPC_NOWAIT };
struct sembuf sem_unlock = { 0,  1, IPC_NOWAIT };

int main (int argc, char **argv)
{
  pthread_t tid[16];
  key_t key = 1; // 커맨드 상에서 ipcs 명령어 사용하여 Key 값 확인
  int i;
  int index[16];

  // 0: 세마포어가 이미 있으면 삭제
  if ((id = semget (key, 0, 0600)) == 0)
    semctl (id, 0, IPC_RMID, 0);

  // 1: 세마포어 1개 새로 생성
  id = semget (key, 1, IPC_CREAT | 0600);

  // 2. 세마포어의 크기를 1로 설정(Binary Semaphore, Like Mutex)
  semctl (id, 0, SETVAL, 1);
  
  for (i = 0; i < thread_count; i++)
    {
      index[i] = i;
      pthread_create (&tid[i], NULL, start_routine, &index[i]);
    }

  pause ();

  return 0;
}

void *start_routine (void *index)
{
  // 0: 세마포어 크기가 0 보다 크면 Lock 성공(Return  0)
  //             크기가 0 과 같으면 Lock 실패(Return -1)
  while (semop (id, &sem_lock, 1) == -1);

  // 1: Critical Section 처리
  printf ("[Thread %2d] Shared Number: %d\n", *((int *) index), shared_number);
  shared_number++;

  // 2: 세마포어 언락, 크기 1증가
  semop (id, &sem_unlock, 1);
}
