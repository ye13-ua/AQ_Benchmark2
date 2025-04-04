#ifndef CRIBA_H_
#define CRIBA_H_

#include <stdbool.h>
#include <time.h>
#include <cmath>
#include <stdio.h>
#include <chrono>
#include <iostream>
#include <emmintrin.h> //Include para ejecutacion de SSE

using namespace std;

//Funcion de iniciar todo el ciclo, de benchmark
void iniciamosCiclo();

//Ejecutacion de codigo en lenguajes diferentes
void cribaC();
void cribaASM();
void cribaSSE();

//Funcion de medir el tiempo de cada benchmark
using namespace std::chrono;
milliseconds obtenerTiempo(int);

//Funciones principales de demonstrar resultados
void ejecutarBenchmark();
std::chrono::milliseconds BenchCriba();

#endif
