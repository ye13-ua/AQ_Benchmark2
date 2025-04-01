

#include <iostream> // Cout
#include <iomanip>  // setw()
#include <vector>   // Vectores
#include <cstdlib>  // Números (pseudo)aleatorios
#include <ctime>    // Tiempo para semilla aleatoria
#include <cmath>    // potencias
#include <chrono>   // Cronómetro del tiempo
#include <emmintrin.h> // Intel SSE


using namespace std;

const unsigned int vecSize = static_cast<unsigned int>(std::pow(2, 16));
std::chrono::milliseconds GlobalCountingTime = std::chrono::milliseconds(0);
std::chrono::milliseconds GlobalCountingAsmTime = std::chrono::milliseconds(0);
std::chrono::milliseconds GlobalCountingAsmSSETime = std::chrono::milliseconds(1);

static bool sortVerify(vector<int> seq) {
	for (unsigned int i = 0; i < vecSize - 1; i++) {
		if (seq[i] > seq[i + 1]) {
			return false;
		}
	} return true;
}
static bool sortVerify(vector<float> seq) {
	for (unsigned int i = 0; i < vecSize - 1; i++) {
		if (seq[i] > seq[i + 1]) {
			return false;
		}
	} return true;
}

vector<int> generavector() {
	vector<int> seq(vecSize);
	for (unsigned int i = 0; i < vecSize; i++) {
		seq[i] = rand() % vecSize + 1;
	}

	return seq;
}

vector<float> generavectorf() {
	vector<float> seq(vecSize);
	for (unsigned int i = 0; i < vecSize; i++) {
		seq[i] = (float)(rand() % vecSize + 1) * 1.13;
	}

	return seq;
}


void countingSort(vector<int>& seq) {
	int size = seq.size();
	int max = *max_element(seq.begin(), seq.end());
	int min = *min_element(seq.begin(), seq.end());
	int range = max - min + 1;
	vector<int> count(range), output(size);
	for (int i = 0; i < size; i++)
		count[seq[i] - min]++;
	for (int i = 1; i < count.size(); i++)
		count[i] += count[i - 1];
	for (int i = size - 1; i >= 0; i--) {
		output[count[seq[i] - min] - 1] = seq[i];
		count[seq[i] - min]--;
	}
	for (int i = 0; i < size; i++)
		seq[i] = output[i];
}

void countingSort(vector<float>& seq) {
	int size = seq.size();
	float max = *max_element(seq.begin(), seq.end());
	float min = *min_element(seq.begin(), seq.end());
	int range = max - min + 1;
	vector<int> count(range), output(size);
	for (int i = 0; i < size; i++)
		count[seq[i] - min]++;
	for (int i = 1; i < count.size(); i++)
		count[i] += count[i - 1];
	for (int i = size - 1; i >= 0; i--) {
		output[count[seq[i] - min] - 1] = seq[i];
		count[seq[i] - min]--;
	}
	for (int i = 0; i < size; i++)
		seq[i] = output[i];
}

void countingSortAsm(vector<int>& seq) {
	int* arr = &seq[0];
	int size = seq.size();
	int temp = size;
	int max = *max_element(seq.begin(), seq.end());
	int min = *min_element(seq.begin(), seq.end());
	int range = max - min + 1;
	vector<int> count(range, 0);
	vector<int> output(size);

	// Contar ocurrencias
	for (int i = 0; i < size; i++) {
		count[seq[i] - min]++;
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

					mov edi, [esi + eax * 4]  // Calcular seq[i]
					sub edi, ebx   // Calcular seq[i] - min

					// Obtener count[seq[i] - min]
					mov ebx, [ecx + edi * 4]  // ebx = count[seq[i] - min]
					dec ebx                 // count[seq[i] - min]--
					mov[ecx + edi * 4], ebx  // Actualizar count[seq[i] - min]

					// Verificar límites antes de acceder a output
					cmp ebx, 0
					jl build_done
					cmp ebx, temp
					jge build_done

					// Obtener nuevamente seq[i]
					mov edi, [esi + eax * 4]  // edi = seq[i]

					// Colocar en output
					mov[edx + ebx * 4], edi  // output[count[seq[i] - min] - 1] = seq[i]

					dec eax
					jmp build_loop

					build_done :
				mov ebx, min
	}

	// Copiar output a seq
	for (int i = 0; i < size; i++) {
		seq[i] = output[i];
	}
}

void countingSortAsmSSE(vector<int>& seq) {

}

void CountingSortExecution() {
	srand(0);

	vector<int> vectint = generavector();
	vector<float> vectfloat = generavectorf();
	auto totalTimeS = std::chrono::high_resolution_clock::now();

	auto start = chrono::high_resolution_clock::now();
	countingSort(vectint);
	auto end = chrono::high_resolution_clock::now();
	auto time = chrono::duration_cast<chrono::milliseconds>(end - start);

	start = chrono::high_resolution_clock::now();
	countingSort(vectfloat);
	end = chrono::high_resolution_clock::now();
	auto timef = chrono::duration_cast<chrono::milliseconds>(end - start);

	auto totalTimeE = std::chrono::high_resolution_clock::now();
	
	cout << "\n>-C-/-C-+-+-----------\n" << flush;

	// Imprimir valores de ms siguiendo formato usado en ShellSort
	cout << "| INT:   " << flush;   sortVerify(vectint) ? cout << "[OK - " << time.count() << "ms]" << flush : cout << "[BAD]" << std::flush;
	cout << "\n| FLOAT: " << flush; sortVerify(vectfloat) ? cout << "[OK - " << timef.count() << "ms]" << flush : cout << "[BAD]" << std::flush;

	// Imprimimos el tiempo total de la ejecución
	auto totalTime = std::chrono::duration_cast<std::chrono::milliseconds>(totalTimeE - totalTimeS);
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

	cout << "\n>-A-S-M-x-8-6---------\n" << std::flush;

	// Imprimir valores de ms igual que en la version del algoritmo de C++
	cout << "| INT:   " << std::flush;   sortVerify(vectint) ? cout << "[OK - " << time.count() << "ms]" << flush : cout << "[BAD]" << std::flush;
	
	cout << "\n| TOTALTIME: [" << time.count() << "ms]" << std::flush;

	GlobalCountingAsmTime = time;
}

chrono::milliseconds BenchCountingSort() {
	cout << "\n/-C-o-u-n-t-i-n-g-S-o-r-t---" << flush;
	
	
	CountingSortExecution();
	CountingSortAsmExecution();
	// El porcentaje que indica el aumento de rendimiento en respecto a versión anterior

	cout << "\n|\n| COMPARING TO C/C++:\n| ASM BOOST: " << (GlobalCountingTime / GlobalCountingAsmTime) * 100 << "%";
	cout << "\n| SSE BOOST: " << (GlobalCountingTime / GlobalCountingAsmSSETime) * 100 << "%";
	cout << "\n" << R"(\)" << "" << flush;

	// Devuelve el tiempo total que necesitó el ordenador para ejecutar el benchmark (todas las versiones del algoritmo)
	return (GlobalCountingAsmTime + GlobalCountingTime + GlobalCountingAsmSSETime);
}