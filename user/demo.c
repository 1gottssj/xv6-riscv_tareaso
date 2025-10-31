#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int N = 10;         
  int base = 50;       

  for(int i = 0; i < N; i++){
    int pid = fork();
    if(pid == 0){
      int t = base * (i + 1);
      settickets(t);
      printf("[child %d] pid=%d tickets=%d\n", i, getpid(), t);


      volatile unsigned long x = 0;
      for(;;){
        x += 1;
        if((x & 0xFFFFF) == 0)  
          yield();              
      }
      exit(0);
    }
  }

  for(int i = 0; i < N; i++)
    wait(0);

  exit(0);
}
