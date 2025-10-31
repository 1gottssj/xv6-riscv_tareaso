#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int N = 10;         
  int base = 50;       

  for (int i = 0; i < N; i++) {
    int pid = fork();
    if (pid == 0) {
      int t = base * (i + 1);
      settickets(t);
      printf("[child %d] pid=%d tickets=%d\n", i, getpid(), t);

      volatile unsigned long x = 0;
      for(;;) {
        x++;
      }
      exit(0);
    }
  }

  // el padre muestra mensaje y espera un poco
  printf("Demo corriendo...\n");
  sleep(200);  // si sleep funciona
  exit(0);
}
