#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char **argv)
{
  char *ptr1;
  char *ptr2;

  ptr1 = sbrk (4);
  //ptr1 = "abc";
  ptr1[0] = 'a';
  ptr1[1] = 'b';
  ptr1[2] = 'c';
  ptr1[3] = '\0';

  printf ("%p, %s\n", ptr1, (char *) ptr1);

  brk (ptr1 + 4);

  ptr2 = sbrk (4);
  //ptr2 = "123";
  ptr2[0] = '1';
  ptr2[1] = '2';
  ptr2[2] = '3';
  ptr2[3] = '\0';

  printf ("%p, %s\n", ptr2, (char *) ptr2);

  return 0;
}
