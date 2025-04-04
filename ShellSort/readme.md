
# [AQ UA24-25] BencH2.o 
GITHUB OFICIAL: https://github.com/ye13-ua/AQ_Benchmark2

RECOMENDACIÓN: LEER readme.md CON ESTE EDITOR DE MD https://stackedit.io/app#
## Autores
 - Yevhenii Edelhsteyn
    > Tiempo de ejecución del benchmark: 4590ms
 - Marat Galliulin
    > Tiempo de ejecución del benchmark: 10245ms
 - Jesús Ballesteros Navarro
    > Tiempo de ejecución del benchmark: 14527ms
 - Gladys Andrea Peveroni Martinez
    > Tiempo de ejecución del benchmark: 9632ms
 - Enrique Mira-Perceval Lillo
     > Tiempo de ejecución del benchmark:

# Objetivo del BENCH2.o
Se desea implementar un benchmark que mida la capacidad de la CPU ejecutando varios algoritmos de ordenación de vectores sobre vectores de gran tamaño observando el tiempo en ms que tardan en cada ocasión cada algoritmo, se implementan dos vectores uno de enteros y otro de racionales.
Todos los algoritmos se implementan y se prueban en las tres versiones pedidas del benchmark, es decir, en C++, en x86 y en x86 usando SSE.
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
2. 'c' - CountingSort
3. 'i' - InsertionSort
4. 'r' - RadixSort
5. 'e' - CribaEratostenes
## Ejemplos

> Entrada ---> Salida

Nada ---> Todos los algoritmos
`-srcie` ---> Todos los algoritmos
`-se -i -cr` ---> Todos los algoritmos
`-rs` ---> ShellSort y RadixtSort

> El interpretador de argumentos ignora todos los parámetros que no empiezan con '-' o no forman parte de la lista de par´metros previa:

`-asrcieo1234` ---> Todos los algoritmos + Warnings (Ignora a, o, 1, 2, 3, 4)
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
> readme.md []: ShellSort = true, RadixSort = false, etc.

# Algoritmos integrados
## Algoritmos de ordenamiento
1. ShellSort 
	> Yevhenii Edelshteyn
	> ShellSort.cpp || ShellSort.h
2. CountingSort
    > Jesús Ballesteros Navarro
	> CoutingSort.cpp || CoutingSort.h
3. InstertionSort
    > Gladys Andrea Peveroni Martinez
    > InsertionSort.cpp || InsertionSort.h
4. RadixSort
    > Enrique Mira-Perceval Lillo
    > RadixSort.cpp || RadixSort.h

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

### RadixSort

[Programiz - CountingSort](https://www.programiz.com/dsa/radix-sort)

RadixSort es un algoritmo de ordenación no comparativo que ordena números procesando sus dígitos individuales. 
Funciona distribuyendo los elementos según cada dígito, comenzando por el dígito menos significativo (LSD - Least Significant Digit) 
hasta el más significativo (MSD - Most Significant Digit), utilizando otro algoritmo estable como CountingSort para ordenar según cada posición.

Es especialmente eficiente cuando se trata de ordenar grandes cantidades de enteros con un número limitado de dígitos.

Algoritmo:
-1 Encontrar el valor máximo en el arreglo para saber cuántos dígitos tiene el número más grande.
-2 Iterar sobre cada posición decimal (unidades, decenas, centenas...) desde el dígito menos significativo hasta el más significativo.
-3 En cada iteración, ordenar los elementos según el dígito actual, utilizando un algoritmo estable (como CountingSort).
-4 Repetir el proceso hasta haber procesado todos los dígitos.

Complejidad:
Caso peor: O(d × (n + k))
Caso mejor: O(d × (n + k))
Caso promedio: O(d × (n + k))
Donde:
n es el número de elementos.
k es el rango de dígitos posibles (por ejemplo, 10 para base decimal).
d es el número de dígitos del número más grande.

Complejidad espacial: O(n + k), por el uso de arreglos temporales y de conteo.

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
InsertionSort

[Programiz - Insertion Sort](https://www.programiz.com/dsa/insertion-sort)

![InsertionSortgif](https://upload.wikimedia.org/wikipedia/commons/0/0f/Insertion-sort-example-300px.gif)

Insertion Sort es un algoritmo de ordenación basado en comparación.
Construye la lista ordenada uno a uno, insertando cada nuevo elemento en su lugar correspondiente, similar a cómo ordenarías cartas en la mano.

Este algoritmo es eficiente para listas pequeñas o listas que ya están casi ordenadas.
Algoritmo:

- 1 Empieza desde el segundo elemento (índice 1), considerando el primero como parte de la lista ordenada.
- 2 Toma el elemento actual como la "clave".
- 3 Compara la clave con los elementos a su izquierda, y los va desplazando hacia la derecha si son mayores que la clave.
- 4 Inserta la clave en la posición correcta dentro de la parte ya ordenada.
- 5 Repite el proceso hasta el final de la lista.


Complejidad caso peor: O(n²) – Cuando la lista está en orden inverso
Complejidad caso mejor: O(n) – Cuando la lista ya está ordenada
Complejidad caso promedio: O(n²)
Complejidad espacial: O(1) – In-place, no necesita memoria adicional significativa.

Características:
Algoritmo estable (mantiene el orden relativo de elementos iguales).
No es adecuado para listas grandes debido a su complejidad cuadrática.

Es excelente para:
Listas pequeñas
Listas casi ordenadas
Integración en algoritmos híbridos (como TimSort)


## Algoritmos matemáticos
1. Criba de Eratosthenes
> Marat Galiullin
> Criba.h || Criba.cpp

### Criba de Eratosthenes
[Descripcion Global de metodo](https://micalculadoracientifica.com/criba-de-eratostenes/)

![Criba_Gif](https://divulgadores.com/wp-content/uploads/2018/09/criba_de_eratostenes_animacion.gif)

Tenemos la ejecutación de metodo de "Criba de Eratosthenes", que es un metodo sencillo para encontrar todos los números primos menores que un número dado.

Funciona como:
>1) **Lista inicial:** Escribe todos los números desde 2 hasta el límite deseado.
>2. **Marcar múltiplos:** Comienza con el primer número (2) y elimina todos sus múltiplos
>3. **Repetir el proceso:** Pasa al siguiente número que no ha sido eliminado (3) y elimina sus múltiplos. Continúa con el siguiente número no marcado.
>4. **Finalizar:** Cuando el cuadrado del siguiente número no marcado es mayor que el límite, detén el proceso. Los números que quedan sin eliminar son primos.

> Es un metodo eficiente y facil de entender, que solo tiene la referencia a la lista de numeros y su eliminación en proceso sistematico. También se puede encontrar primos hasta el valor más grande posible sin necesidad de recorrer todos números en manera individual.


# Resultados de los benchamrks
Esta es la gráfica con los resultados de ejecución del benchamrk en 3 máquinas. Podemos pareciar una anomalía en cuanto al ordenador de Marat.

![Resultados de ejecuión de los benchmarks](https://i.postimg.cc/ZRLb38bZ/Screenshot-20.png)
