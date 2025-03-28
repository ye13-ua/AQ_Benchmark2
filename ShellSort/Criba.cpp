#include <stdio.h>
#include <chrono>
#include <iostream>
#include <emmintrin.h> //Include para ejecutacion de SSE

#define MAX_PRIME 1000000  // El numero limite de numeros primos
#define REPETICIONES 180  // Repiticion de proceso, para mejorar y demonstrar el resultado en manera m�s clara

using namespace std;

//Vector de elementos de numeros hasta MAX_PRIME
bool esPrimo[MAX_PRIME + 1];

void iniciamosCiclo()
{
    for (int i = 2; i <= MAX_PRIME; i++)
    {
        esPrimo[i] = true;
    }
}

void cribaC()
{
    for (int i = 2; i * i <= MAX_PRIME; i++)
    {
        if (esPrimo[i])
        {
            for (int j = i * i; j <= MAX_PRIME; j += i)
            {
                esPrimo[j] = false;
            }
        }
    }
}

void cribaASM()
{
    for (int i = 2; i * i <= MAX_PRIME; i++) {
        if (!esPrimo[i]) continue; // Optimizamos el codigo en C++ codigo para obtener

        int j = i * i;             // Definimos el valor de j como cuadrado de i

        __asm {
            mov ecx, j           // j = i * i
            mov eax, i           // Ponemos valor de i al eax, para incrementos futuros

            ciclo_j :
            cmp ecx, MAX_PRIME   // if (j > MAX_PRIME), exit
                jg fin_j

                mov esi, ecx                    // Ponemos valor de j al esi para ejecutacion
                mov byte ptr esPrimo[esi], 0    // esPrimo[j] = false

                add ecx, eax         // j += i
                jmp ciclo_j          // Repetir

                fin_j :
        }
    }
}

void cribaSSE() {
    __m128i zeroVector = _mm_setzero_si128();  // 128-bit register filled with 0s

    for (int i = 2; i * i <= MAX_PRIME; i++) {
        if (!esPrimo[i]) continue;

        int j = i * i;

        // Process in chunks of 16 bytes (128 bits) at a time
        for (; j + 16 <= MAX_PRIME; j += 16) {
            _mm_storeu_si128((__m128i*) & esPrimo[j], zeroVector);
        }

        // Handle remaining elements one by one
        for (; j <= MAX_PRIME; j += i) {
            esPrimo[j] = false;
        }
    }
}


std::chrono::milliseconds obtenerTiempo(int caso)
{
    using namespace std::chrono;

    auto start = milliseconds(0);
    auto end = milliseconds(0);

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
    }
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
    else
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

void ejecutarBenchmark()
{
    std::chrono::milliseconds tiempoC, tiempoASM, tiempoSSE;

    iniciamosCiclo();
    tiempoC = obtenerTiempo(1);

    printf("------------C----C++------------\n");
    printf("Tiempo de ejecucion en C: %ld milisegundos\n", tiempoC.count());
    printf("\n");

    iniciamosCiclo();
    tiempoASM = obtenerTiempo(2);

    printf("--------------ASM---------------\n");
    printf("Tiempo de ejecucion en ASM: %ld milisegundos\n", tiempoASM.count());
    printf("\n");

    iniciamosCiclo();
    tiempoSSE = obtenerTiempo(3);

    printf("--------------SSE---------------\n");
    printf("Tiempo de ejecucion en SSE: %ld milisegundos\n", tiempoSSE.count());
    printf("\n");

    double mejora = ((double)(tiempoC.count() - tiempoASM.count()) / tiempoC.count()) * 100;
    printf("Mejora con ASM: %.2f%%\n", mejora);
}

int main() {

    using namespace std::chrono;

    auto startBench = high_resolution_clock::now();

    printf("BENCHMARK: CRIBA DE ERAT�STENES\n");
    ejecutarBenchmark();

    auto endBench = high_resolution_clock::now();
    auto tiempoBench = duration_cast<milliseconds>(endBench - startBench);

    printf("El tiempo de ejecutacion de benchmark es: %ld milisegundos\n", tiempoBench.count());
    printf("\nPulse <Return> para finalizar...\n");
    getchar();
    return 0;
}
