# Informe Tarea 2  Lottery Scheduling
## Integrantes:
- Bastian De La Fuente
- Dubalio Pérez
## Funcionamiento y lógica de la implementación
- Se reemplazó el planificador Round Robin de xv6 por un planificador de tipo Lottery Scheduling.  
- En este esquema, cada proceso tiene una cantidad de tickets que representa su probabilidad de ser elegido para usar la CPU.  
- En cada iteración del scheduler se suman los tickets de todos los procesos en estado RUNNABLE, se genera un número aleatorio entre 1 y el total, y se recorre la tabla de procesos acumulando tickets hasta encontrar el proceso ganador.  
- Ese proceso se ejecuta y su contador de ejecuciones (run_slices) se incrementa.  
- En promedio, los procesos con más tickets reciben más tiempo de CPU, manteniendo una distribución proporcional pero no determinista.

## Modificaciones realizadas
- **kernel/proc.h**:  
  Se agregaron dos nuevos campos a la estructura `proc`:  
  `int tickets` (cantidad de tickets de cada proceso) y `uint64 run_slices` (contador de veces que fue elegido por el scheduler).

- **kernel/proc.c**:  
  En la función `allocproc()` se inicializaron los campos `tickets = 100` y `run_slices = 0`.  
  Se modificó la función `scheduler()` para implementar la lógica de lotería:  
  - Se calcula el total de tickets de procesos RUNNABLE.  
  - Si el total es 0, el scheduler continúa el ciclo sin ejecutar nada.  
  - Se genera un número aleatorio entre 1 y el total.  
  - Se recorre la lista de procesos acumulando tickets hasta encontrar el ganador.  
  - Se ejecuta el proceso ganador y se incrementa su campo `run_slices`.  
  Además, se agregó una pequeña función de generación de números aleatorios (`krand` y `rand_range`) dentro del mismo archivo.  
  En `procdump()` se modificó el `printf` para mostrar también los campos `tickets` y `run_slices`, permitiendo observar el reparto de CPU.

- **kernel/sysproc.c**:  
  Se creó la syscall `sys_settickets()` para permitir que un proceso cambie su cantidad de tickets.  
  Si el valor recibido es menor que 1, se asigna automáticamente 1.  
  El código lee el argumento con `argint(0, &n)` y actualiza `myproc()->tickets`.

- **kernel/syscall.h, kernel/syscall.c, user/user.h, user/usys.pl**:  
  Se registró la nueva syscall `settickets` (número de syscall, prototipo, entrada en la tabla del kernel y stub de usuario).

- **user/demo.c**:  
  Programa de prueba que crea 10 procesos con distintos valores de tickets (`50, 100, 150, ... 500`).  
  Cada proceso imprime su PID y tickets asignados, y luego entra en un bucle infinito para consumir CPU.  
  El padre muestra un mensaje final para indicar que la demo está corriendo y que se puede usar `Ctrl+A, P` (en QEMU) para ver los tickets y slices en `procdump`.

## Dificultades encontradas y soluciones implementadas
- **Problemas con `sleep()` y funciones de usuario**:  
  Al intentar usar `sleep()` o `yield()` en el programa `demo.c`, aparecían errores de compilación y linker porque esas funciones no estaban declaradas o mapeadas como syscalls en esta versión del xv6.  
  La solución fue eliminar las llamadas a `sleep()` y dejar un bucle infinito para mantener los procesos activos, permitiendo al scheduler repartir la CPU de forma natural.  

- **Impresión desordenada en consola**:  
  Los procesos hijos imprimen al mismo tiempo, lo que provoca texto intercalado.  
  Se comprobó que esto es normal debido a la concurrencia y no afecta el funcionamiento del scheduler.

## Posibles problemas de este tipo de scheduler
El Lottery Scheduling introduce aleatoriedad, por lo que no garantiza un reparto exacto del tiempo de CPU en el corto plazo.  
Un proceso con menos tickets puede tener más suerte y ser elegido varias veces seguidas.  
Esto genera falta de determinismo y posibles variaciones temporales en el rendimiento.  
También puede presentar ineficiencia si hay muy pocos procesos o si los tickets no se ajustan correctamente.  
En entornos con requerimientos de tiempo real, no es apropiado, ya que no asegura tiempos de respuesta predecibles.  
Sin embargo, su ventaja principal es la simplicidad y la proporcionalidad probabilística en la asignación de CPU, que se cumple en promedio.

