#include "InsertionSort.h"

/////////////////////// C++ ///////////////////////

InsertionSort::InsertionSort() {
	randomIntVector = generateIntVector(vecSize);
	randomFloatVector = generateFloatVector(vecSize);

	for (int i = 0; i < vecSize; i++) {
		cout << randomIntVector[i] << ", ";
	}
	cout << endl;
	for (int i = 0; i < vecSize; i++) {
		cout << randomFloatVector[i] << ", ";
	}
	cout << endl;
}

vector<int> InsertionSort::generateIntVector(int vecSize)
{
	srand(time(NULL));
	vector<int> vec(vecSize);
	for (unsigned int i = 0; i < vecSize; i++) {
		vec[i] = rand() % vecSize + 1;
	}

	return vec;
}

vector<float> InsertionSort::generateFloatVector(int vecSize)
{
	//srand((unsigned)time(NULL));
	vector<float> vec(vecSize);
	for (unsigned int i = 0; i < vecSize; i++) {
		vec[i] = (float)(rand() % 10) / (float)(rand() % 10); // * 1.13;
	}

	return vec;
}

void InsertionSort::insertionSortExecutionC()
{
	//ORDENAMOS EL VECTOR DE ENTEROS
	for (int i = 1; i < vecSize; ++i) {
		int key = randomIntVector[i];
		int j = i - 1;

		/* Move elements of arr[0..i-1], that are
		   greater than key, to one position ahead
		   of their current position */
		while (j >= 0 && randomIntVector[j] > key) {
			randomIntVector[j + 1] = randomIntVector[j];
			j = j - 1;
		}
		randomIntVector[j + 1] = key;
	}

	for (int i = 0; i < vecSize; i++) {
		cout << randomIntVector[i] << ", ";
	}
	cout << endl;

	//ORDENAMOS EL VECTOR DE DECIMALES
	for (int i = 1; i < vecSize; ++i) {
		int key = randomFloatVector[i];
		int j = i - 1;

		/* Move elements of arr[0..i-1], that are
		   greater than key, to one position ahead
		   of their current position */
		while (j >= 0 && randomFloatVector[j] > key) {
			randomFloatVector[j + 1] = randomFloatVector[j];
			j = j - 1;
		}
		randomFloatVector[j + 1] = key;
	}

	for (int i = 0; i < vecSize; i++) {
		cout << randomFloatVector[i] << ", ";
	}
	cout << endl;
}

/////////////////////// ASM ///////////////////////

void InsertionSort::insertionSortExecutionAsm()
{
	vector<int> newRandomIntVector;
	vector<float> newRandomFloatVector;
	newRandomIntVector = generateIntVector(vecSize);
	newRandomFloatVector = generateFloatVector(vecSize);

	int* intPtr = &newRandomIntVector[0];
	float* floatPtr = &randomFloatVector[0];
	cout << "Vector de enteros sin ordenar (ASM):" << endl;
	for (int i = 0; i < vecSize; i++) {
		cout << newRandomIntVector[i] << ", ";
	}
	cout << endl;
	//ORDENAMOS EL VECTOR DE ENTEROS
	for (int i = 1; i < vecSize; ++i) {
		__asm {
			mov ecx, i                 // ecx = i
			mov eax, intPtr  // eax = &newRandomIntVector[0]
			mov edx, [eax + ecx * 4]    // edx = key = newRandomIntVector[i]
			dec ecx                   // j = i - 1

			loop_start :
			cmp ecx, -1               // while (j >= 0)
				jl insert_key

				mov ebx, [eax + ecx * 4]    // ebx = newRandomIntVector[j]
				cmp ebx, edx              // if newRandomIntVector[j] > key
				jle insert_key

				mov[eax + ecx * 4 + 4], ebx // newRandomIntVector[j + 1] = newRandomIntVector[j]
				dec ecx
				jmp loop_start

				insert_key :
			mov[eax + ecx * 4 + 4], edx // newRandomIntVector[j + 1] = key
		}
	}

	cout << "Vector de enteros ordenado (ASM):" << endl;
	for (int i = 0; i < vecSize; i++) {
		cout << newRandomIntVector[i] << ", ";
	}
	cout << endl;

	cout << "Vector de flotantes sin ordenar (ASM parcial):" << endl;
	for (int i = 0; i < vecSize; i++) {
		cout << newRandomFloatVector[i] << ", ";
	}
	cout << endl;

	//ORDENAMOS EL VECTOR DE DECIMALES
	for (int i = 1; i < vecSize; ++i) {
		float key;
		int j = i - 1;
		__asm {
			mov ecx, i
			mov eax, floatPtr
			movss xmm0, [eax + ecx * 4]  // xmm0 = key
			movss key, xmm0
		}

		while (j >= 0 && newRandomFloatVector[j] > key) {
			newRandomFloatVector[j + 1] = newRandomFloatVector[j];
			j--;
		}
		newRandomFloatVector[j + 1] = key;
	}

	cout << "Vector de flotantes ordenado (ASM parcial):" << endl;
	for (int i = 0; i < vecSize; i++) {
		cout << newRandomFloatVector[i] << ", ";
	}
	cout << endl;
}


/////////////////////// SSE ///////////////////////

void InsertionSort::insertionSortExecutionSSE()
{
	// ORDENAMOS EL VECTOR DE ENTEROS (simulación, no paralelismo real)
	for (int i = 1; i < vecSize; ++i) {
		int key = randomIntVector[i];
		int j = i - 1;

		while (j >= 0) {
			__m128i current = _mm_set1_epi32(randomIntVector[j]); // simula carga
			__m128i keyVal = _mm_set1_epi32(key);
			__m128i cmp = _mm_cmpgt_epi32(current, keyVal);

			if (_mm_cvtsi128_si32(cmp) == 0) break;

			randomIntVector[j + 1] = randomIntVector[j];
			j--;
		}
		randomIntVector[j + 1] = key;
	}

	cout << "Vector de enteros ordenado (SSE):" << endl;
	for (int i = 0; i < vecSize; i++) {
		cout << randomIntVector[i] << ", ";
	}
	cout << endl;

	// ORDENAR EL VECTOR DE DECIMALES
	for (int i = 1; i < vecSize; ++i) {
		float key = randomFloatVector[i];
		int j = i - 1;

		while (j >= 0) {
			__m128 current = _mm_load_ss(&randomFloatVector[j]); // carga escalar
			__m128 keyVal = _mm_set_ss(key);
			__m128 cmp = _mm_cmpgt_ss(current, keyVal);

			if (_mm_comieq_ss(cmp, _mm_setzero_ps())) break;

			randomFloatVector[j + 1] = randomFloatVector[j];
			j--;
		}
		randomFloatVector[j + 1] = key;
	}

	cout << "Vector de flotantes ordenado (SSE):" << endl;
	for (int i = 0; i < vecSize; i++) {
		cout << randomFloatVector[i] << ", ";
	}
	cout << endl;
}


/////////////////////// Benchmarks ///////////////////////

chrono::milliseconds InsertionSort::BenchInsertionSort()
{
	cout << "\n/-I-n-s-e-r-t-i-o-n-S-o-r-t---" << endl;

	//TODO

	insertionSortExecutionC();
	insertionSortExecutionAsm();
	insertionSortExecutionSSE();
	return chrono::milliseconds();
}