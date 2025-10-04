#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int me  = getpid();
  int dad = getppid();

  printf("Yo soy %d y mi padre es %d\n", me, dad);
  printf("getancestor(0) = %d\n", getancestor(0));
  printf("getancestor(1) = %d\n", getancestor(1));
  printf("getancestor(2) = %d\n", getancestor(2));

  int pid = fork();
  if (pid == 0) {

    int meH = getpid();
    printf("[Hijo ] soy %d, mi padre es %d\n", meH, getppid());
    printf("[Hijo ] getancestor(0) = %d\n", getancestor(0));
    printf("[Hijo ] getancestor(1) = %d\n", getancestor(1));
    printf("[Hijo ] getancestor(2) = %d\n", getancestor(2));

    int pid2 = fork();
    if (pid2 == 0) {
  
      int meN = getpid();
   
      printf("[Nieto] soy %d, mi padre es %d\n", meN, getppid());
      printf("[Nieto] getancestor(0) = %d\n", getancestor(0));
      printf("[Nieto] getancestor(1) = %d\n", getancestor(1)); 
      printf("[Nieto] getancestor(2) = %d\n", getancestor(2)); 
      printf("[Nieto] getancestor(3) = %d\n", getancestor(3)); 
      exit(0);
    } else {

      wait(0);
      exit(0);
    }

  } else {
    
    wait(0);
    exit(0);
  }
}