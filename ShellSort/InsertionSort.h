#ifndef INSERTIONSORT_H_
#define INSERTIONSORT_H_

#pragma once
#include <iostream>		
#include <iomanip>		// setw()
#include <vector>		// Vectores
//#include <cstdlib>		// Números (pseudo)aleatorios
//#include <ctime>		// Tiempo para semilla aleatoria
#include <cmath>		// potencias
#include <chrono>		// Cronómetro del tiempo
#include <emmintrin.h>	// Intel SSE
#include <time.h>
#include <stdlib.h> 

using namespace std;

const unsigned int vecSize = pow(2, 3);

class InsertionSort
{
	private:
		vector<int> randomIntVector;
		vector<float> randomFloatVector;

	public:
		InsertionSort();
		vector<int> generateIntVector(int vecSize);
		vector<float> generateFloatVector(int vecSize);
		std::chrono::milliseconds BenchInsertionSort();

		void insertionSortExecutionC();
		void insertionSortExecutionAsm();
		void insertionSortExecutionSSE();



	// BenchSort
	//std::chrono::milliseconds BenchShellSort();

	// Ejecuciónes de algoritmos con contadores integrados
	//void ShellSortCExecution();
	//void ShellSortAsmExecution();
	//void ShellSortAsmSSEExecution();

	// Algoritmos sueltos
	//void shellSort(std::vector<int>& seq);
	//void shellSort(std::vector<float>& seq);

	//void shellSortAsm(std::vector<int>& seq);
	//void shellSortAsm(std::vector<float>& seq);

	//void shellSortAsmSSE(std::vector<int>& seq);
	//void shellSortAsmSSE(std::vector<float>& seq);




};

#endif // !INSERTIONSORT_H_
