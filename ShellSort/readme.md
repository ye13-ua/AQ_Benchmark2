
# [AQ UA24-25] BencH2.o 
## Autores

 - Yevhenii Edelhsteyn
 - Marat Galliulin
 - Jesús Ballesteros Navarro
 - a
 - a

# Uso de BENCH2.o
## Archivos
### Headers
Cada algoritmo tiene su header apropiado para la comunicación entre diferentes archivos.
Dentro de dicho header siempre estará al menos la función que implementa el algoritmo completo.
### CPP
Cada algoritmo tiene su archivo fuente con los métodos "privados". Cada algoritmo estarásiempre comentado de manera que nos parezca apropiada al contexto. La única regla global que siguen los algoritmos es el tipo de datos de salida. Obligatoriamente **std::chrono::milliseconds**.
### Misc
- Source.cpp es el archivo con el main del programa, contiene parcialmente la interfáz y el cálculo del tiempo global.
- readme.md existe exclusivamente para que pudan leer esto.
## Ejecución
Si se desea, se puede ejecutar con el ejecutabel tipo .exe o descarggando el proyecto de github.
- En el caso de usar el ejecutable, simplemente hacerle doble click.
- En el caso de github+VisualStudio, es imprescindible tener el depurador en el **modo x86 (Por defecto es x64)**. El **Debugger/Linker encontrará el main facilelmente de manera autónoma**.

- Aparte, si se quieren utilizar argumentos de entrada, recomendamos usar la opción del debug en VisualStudio.
Para utilizar parámetros de entrada, hacer click derecho sobre el proyecto `Bench2o` y en **`Propiedades de configuración`** ---> **`Depuración`**, estará la casilla **Argumentos de comandos** donde se pueden introducir los parámetros.
> Ej: -aco 12345 -esi
## Parámetros
Cada parámetro válido, tiene que empezar con el caracter '-' y pueden ir tanto juntos como separados
1. 's' - ShellSort
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
2. CountingSort
    > Jesús Ballesteros Navarro
	> CoutingSort.cpp || CoutingSort.h
3. InstertionSort

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


### CountingSort
[Programiz - CountingSort](https://www.programiz.com/dsa/counting-sort#:~:text=Counting%20sort%20is%20a%20sorting,index%20of%20the%20auxiliary%20array.)
CountingSort es un algoritmo de ordenación no comparativo. 
En lugar de comparar elementos, CountingSort cuenta la cantidad de veces que cada valor ocurre en el conjunto de datos, y luego utiliza esa información para colocar los elementos en el orden correcto. 
Es muy eficiente cuando los elementos que se desean ordenar están dentro de un rango pequeño y conocido.
Algoritmo:

- 1 Encontrar el valor máximo (max) en la lista de entrada. 
- 2 Crear un array de conteo donde cada índice representará un valor en el conjunto de datos y almacenará cuántas veces aparece ese valor. 
- 3 Para cada elemento en la lista original, incrementar el valor correspondiente en el array de conteo. 
- 4 Modificar el array de conteo para reflejar las posiciones acumuladas de los elementos. 
- 5 Colocar los elementos en la lista de salida utilizando el array de conteo.

Complejidad caso peor: O(n + k), donde n es el número de elementos a ordenar y k es el rango de los números (es decir, el valor máximo). 
Complejidad caso mejor: O(n + k) 
Complejidad caso promedio: O(n + k)
Complejidad espacial: O(n + k), ya que requiere espacio adicional para el array de conteo.

> CountingSort es extremadamente eficiente para conjuntos de datos con un rango limitado de valores.

### InsertionSort

## Algoritmos matemáticos
1. Criba de Eratosthenes
### Criba de Eratosthenes
