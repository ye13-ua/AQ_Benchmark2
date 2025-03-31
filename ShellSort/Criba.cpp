#include "Criba.h"

#define MAX_PRIME 1000000  // El numero limite de numeros primos
#define REPETICIONES 180  // Repiticion de proceso, para mejorar y demonstrar el resultado en manera m�s clara

using namespace std;

//Vector de elementos de numeros hasta MAX_PRIME
bool esPrimo[MAX_PRIME + 1];

//Funcion para iniciar el ciclo de benchmark
void iniciamosCiclo()
{   //Creamos bucle for iniciamos de primer numero primo, que es 2 hasta el numero limite
    for (int i = 2; i <= MAX_PRIME; i++)
    {   //Definimos todos componentes como true
        esPrimo[i] = true;
    }
}

//Benchmark con codigo de C++, que ejecute la funcion
void cribaC()
{   //Bucle for, para ejecutar todos valores posibles
    for (int i = 2; i * i <= MAX_PRIME; i++)
    {   //Si valor i de vector es primo (true) ==> comprobamos todos sus multiplos como falso
        if (esPrimo[i])
        {
            for (int j = i * i; j <= MAX_PRIME; j += i)
            {
                esPrimo[j] = false;
            }
        }
    }
}

//Funcion de Benchmark escrita por ASM
void cribaASM()
{
    for (int i = 2; i * i <= MAX_PRIME; i++) {
        if (!esPrimo[i]) continue; // Optimizamos el codigo en C++ para simplificar el codigo

        int j = i * i;             // Definimos el valor de j como cuadrado de i

        __asm {
            mov ecx, j           // j = i * i
            mov eax, i           // Ponemos valor de i al eax, para incrementos futuros

            //Etiqueta j de obtener el valor de ciclo, alternativamente j
            ciclo_j :
            cmp ecx, MAX_PRIME   // if (j > MAX_PRIME), exit
                jg fin_j             // Si el valor mayor ==> saltamos al fin_j

                mov esi, ecx                    // Ponemos valor de j al esi para ejecutacion
                mov byte ptr esPrimo[esi], 0    // esPrimo[j] = false

                add ecx, eax         // j += i
                jmp ciclo_j          // Repetir

                fin_j :
        }
    }
}

//Funcion de benchmark ejecuta en SSE 
void cribaSSE() {
    __m128i zeroVector = _mm_setzero_si128();  // registro de 128bit de obtener todos valores = 0

    //Bucle for de convertir todos valores necesarios
    for (int i = 2; i * i <= MAX_PRIME; i++) {
        if (!esPrimo[i]) continue;  //Si el valor i de vector no es primo ==> continuamos el codigo

        int j = i * i;  //Valor cuadrado de i como j

        // Tenemos chuckos de 128 bits al un ejecutacion a la vez
        for (; j + 16 <= MAX_PRIME; j += 16) {
            _mm_storeu_si128((__m128i*) & esPrimo[j], zeroVector);
        }

        // Ponemos todos los valores multiplos como false uno a uno
        for (; j <= MAX_PRIME; j += i) {
            esPrimo[j] = false;
        }
    }
}

//Funcion de ejecutir benchmark con medida del tiempo total
std::chrono::milliseconds obtenerTiempo(int caso)
{
    using namespace std::chrono;

    //Difinimos a la memoria los valores de empieza y final
    auto start = milliseconds(0);
    auto end = milliseconds(0);

    //Si el argumento de caso es 1 ==> ejecutamos C++
    if (caso == 1)
    {
        // Empezamos la medida
        auto start = high_resolution_clock::now();

        //Corremos el algoritmo N veces
        for (int i = 0; i < REPETICIONES; i++)
        {
            cribaC();
        }

        // Paramos la medida
        auto end = high_resolution_clock::now();

        // Return tiempo en "milliseconds"
        return duration_cast<milliseconds>(end - start);
    }   //Si el valor de caso es 2 ==> ejecutamos codigo de ASM
    else if (caso == 2)
    {
        // Empezamos la medida
        auto start = high_resolution_clock::now();

        //Corremos el algoritmo N veces
        for (int i = 0; i < REPETICIONES; i++)
        {
            cribaASM();
        }

        // Paramos la medida
        auto end = high_resolution_clock::now();

        // Return tiempo en "milliseconds"
        return duration_cast<milliseconds>(end - start);
    }
    else   //El caso alternativo, en nuestro caso es 3 ==> ejecutamos benchmark de codigo SSE
    {
        // Empezamos la medida
        auto start = high_resolution_clock::now();

        //Corremos el algoritmo N veces
        for (int i = 0; i < REPETICIONES; i++)
        {
            cribaSSE();
        }

        // Paramos la medida
        auto end = high_resolution_clock::now();

        // Return tiempo en "milliseconds"
        return duration_cast<milliseconds>(end - start);
    }
}

//Funcion de ejecutacion de Benchmark, como obtener el diseño de resultados de todos benchmark con su tiempo
void ejecutarBenchmark()
{
    std::chrono::milliseconds tiempoC, tiempoASM, tiempoSSE;

    //Iniciamos el ciclo y definimos al nuestro variable el codigo C++ para definir su tiempo
    iniciamosCiclo();
    tiempoC = obtenerTiempo(1);

    printf("------------C----C++------------\n");
    printf("Tiempo de ejecucion en C: %ld milisegundos\n", tiempoC.count());
    printf("\n");

    //Iniciamos el ciclo y definimos al nuestro variable el codigo ASM para definir su tiempo
    iniciamosCiclo();
    tiempoASM = obtenerTiempo(2);

    printf("--------------ASM---------------\n");
    printf("Tiempo de ejecucion en ASM: %ld milisegundos\n", tiempoASM.count());
    printf("\n");

    //Iniciamos el ciclo y definimos al nuestro variable el codigo SSE para definir su tiempo
    iniciamosCiclo();
    tiempoSSE = obtenerTiempo(3);

    printf("--------------SSE---------------\n");
    printf("Tiempo de ejecucion en SSE: %ld milisegundos\n", tiempoSSE.count());
    printf("\n");

    //Escribimos el valor de porcentaje de mejora entre tiempo de ejecutacion Benchmark C++ y Benchmark ASM
    double mejora = ((double)(tiempoC.count() - tiempoASM.count()) / tiempoC.count()) * 100;
    printf("Mejora con ASM respeto a C++: %.2f%%\n", mejora);

    //Escribimos el valor de porcentaje de mejora entre tiempo de ejecutacion Benchmark SSE y Benchmark ASM
    double mejoraSSE = ((double)(tiempoASM.count() - tiempoSSE.count()) / tiempoASM.count()) * 100;
    printf("Mejora con SSE respeto a ASM: %.2f%%\n", mejoraSSE);
}

//Funcion main de metodo Criba
std::chrono::milliseconds mainCriba() {

    using namespace std::chrono;

    //Definimos el valor de tiempo de iniciar
    auto startBench = high_resolution_clock::now();

    //Ejecutamos benchmark y su diseño en terminal
    printf("BENCHMARK: CRIBA DE ERATÓSTENES\n");
    ejecutarBenchmark();

    //Terminamos el tiempo de ejecutacion de benchmark con calculacion de tiempo
    auto endBench = high_resolution_clock::now();
    auto tiempoBench = duration_cast<milliseconds>(endBench - startBench);

    //Ponemos la representación de tiempo total de benchmark y ponemos la pausa
    printf("El tiempo de ejecutacion de benchmark es: %ld milisegundos\n", tiempoBench.count());
    printf("\nPulse <Return> para finalizar...\n");
    getchar();

    return tiempoBench;
}
