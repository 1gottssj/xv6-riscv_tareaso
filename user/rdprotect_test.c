#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    char *addr = sbrk(0);
    sbrk(4096);
    addr[0] = 'Z';
    printf("Valor inicial: %c\n", addr[0]);

    // Aplicar protección
    if (mrdprotect(addr, 1) < 0) {
        printf("mrdprotect falló\n");
        exit(1);
    }
    printf("Protección aplicada - solo escritura permitida\n");

    // Escritura debería funcionar
    addr[0] = 'A';
    printf("Escritura exitosa bajo protección\n");

    // Lectura debería FALLAR - esto causaría page fault
    // char c = addr[0];  // ← Esto no debería ejecutarse
    // printf("Esto no debería imprimirse si la protección funciona\n");

    // Revertir protección
    if (munrdprotect(addr, 1) < 0) {
        printf("munrdprotect falló\n");
        exit(1);
    }
    printf("Protección revertida\n");

    // Ahora la lectura debería funcionar
    char c = addr[0];
    printf("Lectura después de desproteger: %c\n", c);

    exit(0);
}
