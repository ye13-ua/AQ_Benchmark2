#include "InsertionSort.h"

/////////////////////// C++ ///////////////////////

void initSeed() {
	srand(time(NULL));
}

bool sortVerifyInt(vector<int> vec) {
	for (unsigned int i = 0; i < vecSize - 1; i++) {
		if (vec[i] > vec[i + 1]) {
			return false;
		}
	} return true;
}

bool sortVerifyFloat(vector<float> vec) {
	for (unsigned int i = 0; i < vecSize - 1; i++) {
		if (vec[i] > vec[i + 1]) {
			return false;
		}
	} return true;
}

vector<int> generateIntVector(int vecSize)
{
	vector<int> vec(vecSize);
	for (unsigned int i = 0; i < vecSize; i++) {
		vec[i] = rand() % 10;
	}

	return vec;
}

vector<float> generateFloatVector(int vecSize)
{
	vector<float> vec(vecSize);
	for (unsigned int i = 0; i < vecSize; i++) {
		vec[i] = (float)(rand() % 10 + rand()/(float)SHRT_MAX); // * 1.13;
	}

	return vec;
}

vector<int> insertionSortExecutionC(vector<int> randomIntVector)
{
	//ORDENAMOS EL VECTOR DE ENTEROS
	for (int i = 1; i < vecSize; ++i) {
		int key = randomIntVector[i];
		int j = i - 1;
		
		while (j >= 0 && randomIntVector[j] > key) {
			randomIntVector[j + 1] = randomIntVector[j];
			j = j - 1;
		}
		randomIntVector[j + 1] = key;
	}

	return randomIntVector;
}

vector<float> insertionSortExecutionC(vector<float> randomFloatVector)
{
	//ORDENAMOS EL VECTOR DE DECIMALES
	for (int i = 1; i < vecSize; ++i) {
		float key = randomFloatVector[i];
		int j = i - 1;

		while (j >= 0 && randomFloatVector[j] > key) {
			randomFloatVector[j + 1] = randomFloatVector[j];
			j = j - 1;
		}
		randomFloatVector[j + 1] = key;
	}

	return randomFloatVector;
}


/////////////////////// ASM ///////////////////////

vector<int> insertionSortExecutionAsm(vector<int> randomIntVector)
{
	int* intPtr = &randomIntVector[0];

	//ORDENAMOS EL VECTOR DE ENTEROS
	for (int i = 1; i < vecSize; ++i) {
		__asm {
			mov ecx, i						// ecx = i
			mov eax, intPtr					// eax = &randomIntVector[0]
			mov edx, [eax + ecx * 4]		// edx = key = randomIntVector[i]
			dec ecx							// j = i - 1

			loop_start :
			cmp ecx, 0					    // while (j >= 0)
				jl insert_key

				mov ebx, [eax + ecx * 4]    // ebx = randomIntVector[j]
				cmp ebx, edx                // if randomIntVector[j] > key
				jle insert_key

				mov[eax + ecx * 4 + 4], ebx // randomIntVector[j + 1] = randomIntVector[j]
				dec ecx
				jmp loop_start

				insert_key :
			mov[eax + ecx * 4 + 4], edx		// randomIntVector[j + 1] = key
		}
	}

	return randomIntVector;
}

vector<float> insertionSortExecutionAsm(vector<float> randomFloatVector)
{
	float* floatPtr = &randomFloatVector[0];

	//ORDENAMOS EL VECTOR DE DECIMALES
	for (int i = 1; i < vecSize; ++i) {
		__asm {
			mov ecx, i							// ecx = i
			mov eax, floatPtr					// eax = &randomFloatVector[0]
			movss xmm0, [eax + ecx * 4]			// xmm0 = key
			dec ecx								// j = i - 1

			loop_start :
			cmp ecx, 0							// while (j >= 0)
				jl insert_key

				movss xmm1, [eax + ecx * 4]		// xmm1 = randomFloatVector[j]
				comiss xmm1, xmm0				// compare xmm1 > xmm0 ?
				jbe insert_key					// if !(xmm1 > xmm0), break

				movss[eax + ecx * 4 + 4], xmm1	// randomFloatVector[j + 1] = randomFloatVector[j]
				dec ecx
				jmp loop_start

				insert_key :
			movss[eax + ecx * 4 + 4], xmm0		// randomFloatVector[j + 1] = key
		}
	}

	return randomFloatVector;
}


/////////////////////// SSE ///////////////////////

vector<int> insertionSortExecutionSSE(vector<int> randomIntVector)
{
	// ORDENAMOS EL VECTOR DE ENTEROS (simulación, no paralelismo real)
	for (int i = 1; i < vecSize; ++i) {
		int key = randomIntVector[i];
		__m128i keyVec = _mm_set1_epi32(key); // Vector con 4 copias de key

		int j = i - 1;

		while (j >= 3) {
			__m128i block = _mm_loadu_si128((__m128i*) & randomIntVector[j - 3]);	// Cargar bloque [j-3, j]
			__m128i cmp = _mm_cmpgt_epi32(block, keyVec);							// Comparar bloque > key
			int mask = _mm_movemask_epi8(cmp);										// Crear máscara
			
			if (mask == 0xFFFF) {													// Todos mayores (cada comparación ocupa 4 bytes)
				_mm_storeu_si128((__m128i*) & randomIntVector[j - 3 + 1], block);	// Desplazamos bloque
				j -= 4;
			}
			else {
				break;
			}
		}

		while (j >= 0 && randomIntVector[j] > key) {
			randomIntVector[j + 1] = randomIntVector[j];
			j--;
		}

		randomIntVector[j + 1] = key;
	}

	return randomIntVector;
}


vector<float> insertionSortExecutionSSE(vector<float> randomFloatVector)
{
	// ORDENAR EL VECTOR DE DECIMALES
	for (int i = 1; i < vecSize; ++i) {
		float key = randomFloatVector[i];
		__m128 keyVec = _mm_set1_ps(key); // Carga clave en los 4 slots

		int j = i - 1;

		while (j >= 3) {
			__m128 currentBlock = _mm_loadu_ps(&randomFloatVector[j - 3]);  // Bloque de 4 floats
			__m128 cmp = _mm_cmpgt_ps(currentBlock, keyVec);				// Comparar todos contra key
			int mask = _mm_movemask_ps(cmp);								// Máscara con resultado de comparación

			if (mask == 0b1111) {
				_mm_storeu_ps(&randomFloatVector[j - 3 + 1], currentBlock); // Mover bloque 4 posiciones hacia adelante
				j -= 4;
			}
			else {
				break; 
			}
		}

		while (j >= 0 && randomFloatVector[j] > key) {
			randomFloatVector[j + 1] = randomFloatVector[j];
			j--;
		}

		randomFloatVector[j + 1] = key;
	}

	return randomFloatVector;
}


/////////////////////// Benchmarks ///////////////////////

chrono::milliseconds BenchInsertionSort()
{
	initSeed();
	cout << "\n/-I-n-s-e-r-t-i-o-n-S-o-r-t---" << flush;
	vector<int> newRandomIntVector = generateIntVector(vecSize);
	vector<float> newRandomFloatVector = generateFloatVector(vecSize);

	cout << "\n>-C-/-C-+-+-----------" << endl;
	auto startCInt = chrono::high_resolution_clock::now();
	auto sortedCInt = insertionSortExecutionC(newRandomIntVector);
	auto durationCInt = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startCInt);
	cout << "| INT:   " << (sortVerifyInt(sortedCInt) ? "[OK - " + to_string(durationCInt.count()) + "ms]" : "[BAD]") << endl;

	auto startCFloat = chrono::high_resolution_clock::now();
	auto sortedCFloat = insertionSortExecutionC(newRandomFloatVector);
	auto durationCFloat = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startCFloat);
	cout << "| FLOAT:   " << (sortVerifyFloat(sortedCFloat) ? "[OK - " + to_string(durationCFloat.count()) + "ms]" : "[BAD]") << endl;


	cout << ">-A-S-M-x-8-6---------" << endl;
	auto startAsmInt = chrono::high_resolution_clock::now();
	auto sortedAsmInt = insertionSortExecutionAsm(newRandomIntVector);
	auto durationAsmInt = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startAsmInt);
	cout << "| INT:   " << (sortVerifyInt(sortedAsmInt) ? "[OK - " + to_string(durationAsmInt.count()) + "ms]" : "[BAD]") << endl;

	auto startAsmFloat = chrono::high_resolution_clock::now();
	auto sortedAsmFloat = insertionSortExecutionAsm(newRandomFloatVector);
	auto durationAsmFloat = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startAsmFloat);
	cout << "| FLOAT:   " << (sortVerifyFloat(sortedAsmFloat) ? "[OK - " + to_string(durationAsmFloat.count()) + "ms]" : "[BAD]") << endl;


	cout << ">-A-S-M-x-8-6-S-S-E---" << endl;
	auto startSSEInt = chrono::high_resolution_clock::now();
	auto sortedSSEInt = insertionSortExecutionSSE(newRandomIntVector);
	auto durationSSEInt = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startSSEInt);
	cout << "| INT:   " << (sortVerifyInt(sortedSSEInt) ? "[OK - " + to_string(durationSSEInt.count()) + "ms]" : "[BAD]") << endl;

	auto startSSEFloat = chrono::high_resolution_clock::now();
	auto sortedSSEFloat = insertionSortExecutionSSE(newRandomFloatVector);
	auto durationSSEFloat = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startSSEFloat);
	cout << "| FLOAT:   " << (sortVerifyFloat(sortedSSEFloat) ? "[OK - " + to_string(durationSSEFloat.count()) + "ms]" : "[BAD]") << endl;


	auto totalTime = chrono::duration_cast<chrono::milliseconds>(durationCInt + durationCFloat + durationAsmInt + durationAsmFloat + durationSSEInt + durationSSEFloat);
	cout << "\n| TOTALTIME: [" << totalTime.count() << "ms]" << flush;

	auto totalCTime = chrono::duration_cast<chrono::milliseconds>(durationCInt + durationCFloat);
	auto totalASMTime = chrono::duration_cast<chrono::milliseconds>(durationAsmInt + durationAsmFloat);
	auto totalSSETime = chrono::duration_cast<chrono::milliseconds>(durationSSEInt + durationSSEFloat);

	std::cout << "\n|\n| COMPARING TO C/C++:\n| ASM BOOST: " << (totalCTime / totalASMTime) * 100 << "%";
	std::cout << "\n| SSE BOOST: " << (totalCTime / totalSSETime) * 100 << "%";
	std::cout << "\n" << R"(\)" << "" << std::flush;

	return chrono::milliseconds();
}