#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "schedule.h"

process *create_process (int);

int cpu_time = 0;
int total_wait_time = 0;
int total_turnaround_time = 0;

int main (int argc, char **argv)
{
  process *p;
  int i;
  int count;

  if (argc != 2)
    {
      printf ("usage: %s <Process Count>\n", argv[0]);
      exit (1);
    }

  srand ((unsigned) time (NULL) + (unsigned) getpid ());

  count = atoi (argv[1]);

  puts ("==========================================================================================");

  for (i = 0; i < count; i++)
    {
      p = create_process (0);
      printf (" [Process %4d]", i);
      printf (" Come Time: %4d", p->come_time);
      printf (", Wait Time: %4d", cpu_time - p->come_time);
      printf (", Burst Time: %4d", p->burst_time);

      total_wait_time += cpu_time - p->come_time;
      cpu_time += p->burst_time;

      printf (", Turnaround Time: %4d\n", cpu_time - p->come_time);

      total_turnaround_time += cpu_time - p->come_time;
    }

  puts ("==========================================================================================");
  printf (" Average Wait Time: %d\n", total_wait_time / i);
  printf (" Average Turnaround Time: %d\n", total_turnaround_time / i);
  puts ("==========================================================================================");

  exit (0);
}

process *create_process (int come_time)
{
  process *p;

  p = (process *) malloc (sizeof (p));
  p->burst_time = ((rand () % 100) + 1);
  p->come_time = come_time;

  return p;
}
