#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    char *addr = sbrk(0);
    sbrk(4096);
    addr[0] = 'Z';
    
    if (mrdprotect(addr, 1) < 0) {
        printf("mrdprotect falló\n");
        exit(1);
    }
    
    addr[0] = 'A';
    char c = addr[0];
    printf("Valor leido: %c\n", c);
    
    if (munrdprotect(addr, 1) < 0) {
        printf("munrdprotect falló\n");
        exit(1);
    }
    printf("Protección revertida\n");
    exit(0);
}
