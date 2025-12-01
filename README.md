# Informe Tarea 3: Protección de Lectura en XV6

## Integrantes:
- Bastian De La Fuente
- Dubalio Pérez

## Funcionamiento y lógica de la implementación
- Se implementó un mecanismo de protección de memoria "solo escritura" para datos sensibles como claves criptográficas.
- El sistema permite que un proceso proteja regiones de su memoria contra lectura, manteniendo el permiso de escritura.
- Cuando una página está protegida, cualquier intento de lectura desde espacio de usuario generará un page fault.
- La protección se aplica modificando el bit PTE_R en las entradas de la tabla de páginas, sin afectar otros permisos.

## Modificaciones realizadas

- **kernel/vm.c**:
  Se implementaron las funciones `mrdprotect()` y `munrdprotect()`:
  - `mrdprotect(void *addr, int len)`: Recorre las páginas en el rango [addr, addr + len*PGSIZE) y limpia el bit PTE_R en cada PTE válida.
  - `munrdprotect(void *addr, int len)`: Restaura el bit PTE_R en las páginas previamente protegidas.
  - Ambas funciones validan que las direcciones estén alineadas, pertenezcan al espacio de usuario y tengan PTE_V.
  - Se usa la función `walk()` para encontrar las PTEs y operaciones de bits para modificar solo el bit PTE_R.

- **kernel/sysproc.c**:
  Se crearon las syscalls `sys_mrdprotect()` y `sys_munrdprotect()`:
  - Leen los parámetros `addr` y `len` desde espacio de usuario usando `argaddr()` y `argint()`.
  - Llaman a las funciones correspondientes en vm.c y retornan el resultado.

- **kernel/syscall.h, kernel/syscall.c, user/user.h, user/usys.pl**:
  Se registraron las nuevas syscalls `mrdprotect` y `munrdprotect`:
  - Se asignaron los números de syscall en syscall.h.
  - Se agregaron los prototipos en la tabla de syscalls en syscall.c.
  - Se declararon en user.h y se generaron los stubs en usys.pl.

- **user/rdprotect_test.c**:
  Programa de prueba que demuestra la funcionalidad:
  - Reserva una página de memoria con `sbrk()`.
  - Escribe un valor inicial en la memoria.
  - Protege la página con `mrdprotect()`.
  - Verifica que la escritura sigue funcionando.
  - Intenta leer la memoria (debería fallar o causar page fault).
  - Revierte la protección con `munrdprotect()`.
  - Confirma que la lectura funciona nuevamente.

## Dificultades encontradas y soluciones implementadas

- **Modificación segura de PTEs**:
  Al modificar los bits de las PTEs, era crucial no afectar otros flags como PTE_W, PTE_X, o PTE_U.
  La solución fue usar operaciones de bits precisas: `*pte &= ~PTE_R` para desactivar y `*pte |= PTE_R` para activar.
  
- **Problemas con sincronización github

- **Validación de parámetros**:
  Se implementaron verificaciones exhaustivas para direcciones no alineadas, longitudes inválidas, 
  acceso a memoria del kernel, y páginas no mapeadas, devolviendo -1 en caso de error.

- **Sincronización de TLB**:
  Después de modificar las PTEs, se usa `sfence.vma` para asegurar que los cambios se reflejen 
  inmediatamente en la TLB y evitar condiciones de carrera.

## Ventajas y aplicaciones de este mecanismo

- **Seguridad para datos sensibles**: Ideal para proteger claves criptográficas, contraseñas, 
  y otros secretos en memoria, previniendo que sean leídos por malware o mediante ataques de memoria.

- **Granularidad flexible**: La protección se puede aplicar a regiones específicas de memoria 
  con longitud variable, no necesariamente a todo el espacio de direcciones.

- **Reversibilidad**: La protección puede activarse y desactivarse dinámicamente según sea necesario, 
  permitiendo acceso controlado a la memoria protegida.

## Limitaciones consideradas

- La protección aplica solo al espacio de usuario; el kernel siempre puede leer la memoria.
- No previene ataques basados en timing o canales laterales.
- El page fault generado por lectura de memoria protegida puede ser detectado por el proceso.
