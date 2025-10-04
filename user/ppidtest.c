#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  int me = getpid();
  int papa = getppid();
  printf("[padre] pid=%d ppid=%d\n", me, papa);

  int pid = fork();
  if(pid == 0){
    // Hijo
    printf("[hijo ] pid=%d ppid=%d\n", getpid(), getppid());
    exit(0);
  } else {
    // Padre
    wait(0);
    exit(0);
  }
}