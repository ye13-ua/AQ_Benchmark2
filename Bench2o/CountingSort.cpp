

#include <iostream> // Cout
#include <iomanip>  // setw()
#include <vector>   // Vectores
#include <cstdlib>  // Números (pseudo)aleatorios
#include <ctime>    // Tiempo para semilla aleatoria
#include <cmath>    // potencias
#include <chrono>   // Cronómetro del tiempo
#include <emmintrin.h> // Intel SSE


using namespace std;

const unsigned int vecSize = static_cast<unsigned int>(pow(2, 16));
chrono::milliseconds GlobalCountingTime = chrono::milliseconds(0);
chrono::milliseconds GlobalCountingAsmTime = chrono::milliseconds(0);
chrono::milliseconds GlobalCountingAsmSSETime = chrono::milliseconds(1);

static bool sortVerify(vector<int> vec) {
	for (unsigned int i = 0; i < vecSize - 1; i++) {
		if (vec[i] > vec[i + 1]) {
			return false;
		}
	} return true;
}
static bool sortVerify(vector<float> vec) {
	for (unsigned int i = 0; i < vecSize - 1; i++) {
		if (vec[i] > vec[i + 1]) {
			return false;
		}
	} return true;
}

vector<int> generavector() {
	vector<int> vec(vecSize);
	for (unsigned int i = 0; i < vecSize; i++) {
		vec[i] = rand() % vecSize + 1;
	}

	return vec;
}

vector<float> generavectorf() {
	vector<float> vec(vecSize);
	for (unsigned int i = 0; i < vecSize; i++) {
		vec[i] = (float)(rand() % vecSize + 1) * 1.13;
	}

	return vec;
}


void countingSort(vector<int>& vec) {
	int size = vec.size();
	int max = *max_element(vec.begin(), vec.end());
	int min = *min_element(vec.begin(), vec.end());
	int range = max - min + 1;
	vector<int> count(range), output(size);
	for (int i = 0; i < size; i++)
		count[vec[i] - min]++;
	for (int i = 1; i < count.size(); i++)
		count[i] += count[i - 1];
	for (int i = size - 1; i >= 0; i--) {
		output[count[vec[i] - min] - 1] = vec[i];
		count[vec[i] - min]--;
	}
	for (int i = 0; i < size; i++)
		vec[i] = output[i];
}

void countingSort(vector<float>& vec) {
	int size = vec.size();
	float max = *max_element(vec.begin(), vec.end());
	float min = *min_element(vec.begin(), vec.end());
	int range = max - min + 1;
	vector<int> count(range), output(size);
	for (int i = 0; i < size; i++)
		count[vec[i] - min]++;
	for (int i = 1; i < count.size(); i++)
		count[i] += count[i - 1];
	for (int i = size - 1; i >= 0; i--) {
		output[count[vec[i] - min] - 1] = vec[i];
		count[vec[i] - min]--;
	}
	for (int i = 0; i < size; i++)
		vec[i] = output[i];
}

void countingSortAsm(vector<int>& vec) {
	int* arr = &vec[0];
	int size = vec.size();
	int temp = size;
	int max = *max_element(vec.begin(), vec.end());
	int min = *min_element(vec.begin(), vec.end());
	int range = max - min + 1;
	vector<int> count(range, 0);
	vector<int> output(size);

	// Contar ocurrencias
	for (int i = 0; i < size; i++) {
		count[vec[i] - min]++;
	}

	// Calcular count acumulativo
	for (int i = 1; i < count.size(); i++) {
		count[i] += count[i - 1];
	}

	__asm {
		mov esi, arr         // Puntero al array original
		mov edi, temp        // Tamaño del array
		mov ebx, min         // Valor mínimo
		lea ecx, count       // Puntero al array count
		lea edx, output      // Puntero al array output

		// Obtener el puntero real de los vectores
		mov ecx, [ecx]       // Obtener el puntero real a count[0]
			mov edx, [edx]       // Obtener el puntero real a output[0]

				// Inicializar i = size - 1
				mov eax, edi
					dec eax

					build_loop :
				cmp eax, 0 // Verificar si i >= 0
					jl build_done

					mov edi, [esi + eax * 4]  // Calcular vec[i]
					sub edi, ebx   // Calcular vec[i] - min

					// Obtener count[vec[i] - min]
					mov ebx, [ecx + edi * 4]  // ebx = count[vec[i] - min]
					dec ebx                 // count[vec[i] - min]--
					mov[ecx + edi * 4], ebx  // Actualizar count[vec[i] - min]

					// Verificar límites antes de acceder a output
					cmp ebx, 0
					jl build_done
					cmp ebx, temp
					jge build_done

					// Obtener nuevamente vec[i]
					mov edi, [esi + eax * 4]  // edi = vec[i]

					// Colocar en output
					mov[edx + ebx * 4], edi  // output[count[vec[i] - min] - 1] = vec[i]

					dec eax
					jmp build_loop

					build_done :
				mov ebx, min
	}

	// Copiar output a vec
	for (int i = 0; i < size; i++) {
		vec[i] = output[i];
	}
}


void countingSortAsmSSE(vector<int>& vec) {
	int size = vec.size();
	int maxVal = *max_element(vec.begin(), vec.end());
	int minVal = *min_element(vec.begin(), vec.end());
	int range = maxVal - minVal + 1;
	vector<int> count(range, 0);
	vector<int> output(size);

	for (int i = 0; i < size; i++) {
		count[vec[i] - minVal]++;
	}

	for (int i = 1; i < range; i++) {
		count[i] += count[i - 1];
	}

	for (int i = size - 1; i >= 0; i--) {
		int index = vec[i] - minVal;
		int pos = count[index] - 1;
		output[pos] = vec[i];
		count[index]--;
	}

	// Copiar el vector de salida a vec utilizando SSE (procesamiento en bloques de 4 enteros)
	int i = 0;
	for (; i <= size - 4; i += 4) {
		__m128i data = _mm_loadu_si128(reinterpret_cast<const __m128i*>(&output[i])); // Cargar 4 enteros de output
		_mm_storeu_si128(reinterpret_cast<__m128i*>(&vec[i]), data);	// Almacenar 4 enteros en vec
	}
	// Copiar los elementos restantes si hay
	for (; i < size; i++) {
		vec[i] = output[i];
	}
}

void countingSortAsmSSE(vector<float>& vec) {
	int size = vec.size();
	if (size == 0) return;
	float maxVal = *max_element(vec.begin(), vec.end());
	float minVal = *min_element(vec.begin(), vec.end());
	int range = static_cast<int>(maxVal - minVal + 1);
	vector<int> count(range, 0);
	vector<float> output(size);

	for (int i = 0; i < size; i++) {
		int index = static_cast<int>(vec[i] - minVal);
		count[index]++;
	}

	for (int i = 1; i < range; i++) {
		count[i] += count[i - 1];
	}

	for (int i = size - 1; i >= 0; i--) {
		int index = static_cast<int>(vec[i] - minVal);
		int pos = count[index] - 1;
		output[pos] = vec[i];
		count[index]--;
	}

	// Uso de SSE para copiar el vector de salida a vec (procesamiento en bloques de 4 floats)
	int i = 0;
	for (; i <= size - 4; i += 4) {
		__m128 data = _mm_loadu_ps(reinterpret_cast<const float*>(&output[i])); // Carga 4 floats de output
		_mm_storeu_ps(reinterpret_cast<float*>(&vec[i]), data);                 // Almacena 4 floats en vec
	}
	// Copiar los elementos restantes, si los hubiera
	for (; i < size; i++) {
		vec[i] = output[i];
	}
}

void CountingSortExecution() {
	srand(0);

	vector<int> vectint = generavector();
	vector<float> vectfloat = generavectorf();
	auto totalTimeS = chrono::high_resolution_clock::now();

	auto start = chrono::high_resolution_clock::now();
	countingSort(vectint);
	auto end = chrono::high_resolution_clock::now();
	auto time = chrono::duration_cast<chrono::milliseconds>(end - start);

	start = chrono::high_resolution_clock::now();
	countingSort(vectfloat);
	end = chrono::high_resolution_clock::now();
	auto timef = chrono::duration_cast<chrono::milliseconds>(end - start);

	auto totalTimeE = chrono::high_resolution_clock::now();
	
	cout << "\n>-C-/-C-+-+-----------\n" << flush;

	// Imprimir valores de ms siguiendo formato usado en ShellSort
	cout << "| INT:   " << flush;   sortVerify(vectint) ? cout << "[OK - " << time.count() << "ms]" << flush : cout << "[BAD]" << flush;
	cout << "\n| FLOAT: " << flush; sortVerify(vectfloat) ? cout << "[OK - " << timef.count() << "ms]" << flush : cout << "[BAD]" << flush;

	// Imprimimos el tiempo total de la ejecución
	auto totalTime = chrono::duration_cast<chrono::milliseconds>(totalTimeE - totalTimeS);
	cout << "\n| TOTALTIME: [" << totalTime.count() << "ms]" << flush;

	GlobalCountingTime = totalTime;
}

void CountingSortAsmExecution() { //En las versiones de x86 y SSE solo se ha implementado la version de ints
	srand(0);
	vector<int> vectint = generavector();

	auto start = chrono::high_resolution_clock::now();
	countingSortAsm(vectint);
	auto end = chrono::high_resolution_clock::now();

	auto time = chrono::duration_cast<chrono::milliseconds>(end - start);

	cout << "\n>-A-S-M-x-8-6---------\n" << flush;

	// Imprimir valores de ms igual que en la version del algoritmo de C++
	cout << "| INT:   " << flush;   sortVerify(vectint) ? cout << "[OK - " << time.count() << "ms]" << flush : cout << "[BAD]" << flush;
	
	cout << "\n| TOTALTIME: [" << time.count() << "ms]" << flush;

	GlobalCountingAsmTime = time;
}

void CountingSortAsmSSEExecution() {
	srand(0);
	vector<int> vectint = generavector();
	vector<float> vectfloat = generavectorf();
	auto totalTimeS = chrono::high_resolution_clock::now();

	auto start = chrono::high_resolution_clock::now();
	countingSortAsmSSE(vectint);
	auto end = chrono::high_resolution_clock::now();
	auto time = chrono::duration_cast<chrono::milliseconds>(end - start);

	start = chrono::high_resolution_clock::now();
	countingSortAsmSSE(vectfloat);
	end = chrono::high_resolution_clock::now();
	auto timef = chrono::duration_cast<chrono::milliseconds>(end - start);

	auto totalTimeE = chrono::high_resolution_clock::now();

	cout << "\n>-A-S-M-x-8-6-S-S-E---\n" << flush;
	// Imprimir valores de ms igual que en la version del algoritmo de C++
	cout << "| INT:   " << flush;   sortVerify(vectint) ? cout << "[OK - " << time.count() << "ms]" << flush : cout << "[BAD]" << flush;
	cout << "\n| FLOAT: " << flush; sortVerify(vectfloat) ? cout << "[OK - " << timef.count() << "ms]" << flush : cout << "[BAD]" << flush;

	auto totalTime = chrono::duration_cast<chrono::milliseconds>(totalTimeE - totalTimeS);

	cout << "\n| TOTALTIME: [" << totalTime.count() << "ms]" << flush;
	GlobalCountingAsmSSETime = totalTime;
}

chrono::milliseconds BenchCountingSort() {
	cout << "\n/-C-o-u-n-t-i-n-g-S-o-r-t---" << flush;
	
	
	CountingSortExecution();
	CountingSortAsmExecution();
	CountingSortAsmSSEExecution();

	cout << "\n|\n| COMPARING TO C/C++:\n| ASM BOOST: " << (GlobalCountingTime / GlobalCountingAsmTime) * 100 << "%";
	cout << "\n| SSE BOOST: " << (GlobalCountingTime / GlobalCountingAsmSSETime) * 100 << "%";
	cout << "\n" << R"(\)" << "" << flush;

	// Devuelve el tiempo total que necesitó el ordenador para ejecutar el benchmark (todas las versiones del algoritmo)
	return (GlobalCountingAsmTime + GlobalCountingTime + GlobalCountingAsmSSETime);
}