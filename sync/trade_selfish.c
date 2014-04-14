#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define STORE_SIZE 10

void *action_farmer (void *);
void *action_hunter (void *);

void *action_producer (void *);
void *action_consumer (void *);
int produce (void);

int count_rice = 0;
int count_meat = 0;

int main (void)
{
  pthread_t farmer;
  pthread_t hunter;

  pthread_create (&farmer, NULL, action_farmer, NULL);
  pthread_create (&hunter, NULL, action_hunter, NULL);

  pause ();

  return 0;
}

void *action_farmer (void *arg)
{
  while (1)
    {
      while (count_meat == 0);

      count_meat--;
      printf ("농부가 창고 안에 고기를 가져갔습니다.\n");

      count_rice++;
      printf ("농부가 쌀을 수확하여 창고에 저장하였습니다.\n");

      printf ("========================================\n");
      sleep (1);
    }
}

void *action_hunter (void *arg)
{
  while (1)
    {
      while (count_rice == 0);

      count_rice--;
      printf ("사냥꾼이 창고에서 쌀을 가져갔습니다.\n");

      count_meat++;
      printf ("사냥꾼이 사냥하여 고기를 창고에 넣었습니다.\n");

      printf ("========================================\n");
      sleep (1);
    }
}
