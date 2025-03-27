
# [AQ UA24-25] BencH2.o 
## Autores

 - Yevhenii Edelhsteyn
 - Marat Galliulin
 - a
 - a
 - a

# Uso de BENCH2.o
## Parámetros
Cada parámetro válido, tiene que empezar con el caracter '-' y pueden ir tanto juntos como separados
1. 's' - ShellSort
2. 'b' - BucketSort
3. 'c' - CountingSort
4. 'i' - InsertionSort
5. 'e' - CribaEratostenes
## Ejemplos

> Entrada ---> Salida

Nada ---> Todos los algoritmos
`-sbcie` ---> Todos los algoritmos
`-se -i -cb` ---> Todos los algoritmos
`-bs` ---> ShellSort y BucketSort

> El interpretador de argumentos ignora todos los parámetros que no empiezan con '-' o no forman parte de la lista de par´metros previa:

`-asbcieo1234` ---> Todos los algoritmos + Warnings (Ignora a, o, 1, 2, 3, 4)
`-a` ---> Error ya que no se solicita ningún algoritmo, idéntico a `-`*
`12345` ---> Argumento completo ignorado por no empezar con '-'

> *Es importante separar ejecución sin parámetros de usuario, de la ejecución con parámetro '-' vacío. Uno ejecuta todos los algoritmos,
> otro espera la lista de algoritmos a ejecutar, y como no recibe
> ninguno, sale con error.

Ejemplo del log del programa: `-aco 12345 -esi`

    [PARAMETERS: 4]
    [0. ..\benchmark.exe]
    [1. -aco]
    [2. 12345]
    [3. -esi]
    WARNING: UNKNOWN char PARAMETER AT argument [1] with value: [a] - VALUE IGNORED
    WARNING: UNKNOWN char PARAMETER AT argument [1] with value: [o] - VALUE IGNORED
    
    WARNING: UNKNOWN - PARAMETER AT [2] with value: [12345] - PARAMETER IGNORED
    
    BENCH2.o: [10111]

> Donde el parámetro 0 es el propio programa (cosas del C/C++)
> y [10111] es la lista de algoritmos en orden de la lista del
> readme.md []: ShellSort = true, bucketSort = false, etc.

# Algoritmos integrados
## Algoritmos de ordenamiento
1. ShellSort 
	> Yevhenii Edelshteyn
	> ShellSort.cpp || ShellSort.h
3. BucketSort
4. CountingSort
5. InstertionSort

### ShellSort
[GeeksForGeeks - ShellSort](https://www.geeksforgeeks.org/shell-sort/)
![ShellSortgif](https://upload.wikimedia.org/wikipedia/commons/d/d8/Sorting_shellsort_anim.gif)

Evolución/Variante del InsertionSort. Cambia fundamentalmente la manera de recorrer los datos. Mientras que uno de los mayores problemas del InsertionSort implican que un elemento puede necesitar muchas iteraciónes de movimiento para alcanzar su posición correcta, el insertion sort implementa una manera de intercambiar de posición dos valores lejanos entre si.
Algoritmo:

> 1 - Inicializar gap (separación/distancia) a un valor h (en este caso h = tamaño de problema/2)
> 2 - Dividir la lista en partes de tamaño h
> 3 - Organizar cada sublista con InsertionSort
> 4 - Repetir 2 hasta que no este ordenado

Complejidad caso peor: O(n^2^)
Complejiadd caso mejor: Ω(n log(n))
Complejidad caso promedio: O(n^1.25^)
Complejidad espacial: O(1), es constante ya que opera sobre el mismo set de datos

> Tanto el caso mejor como peor son superiores en ShellSort en comparación con InsertionSort, pero el caso promeidio de InsertionSort es considerablemente peor que el caso promedio de ShellSort O(n ^2^) > O(n ^1.25^)

### BucketSort
### CountingSort
### InsertionSort

## Algoritmos matemáticos
1. Criba de Eratosthenes
### Criba de Eratosthenes
