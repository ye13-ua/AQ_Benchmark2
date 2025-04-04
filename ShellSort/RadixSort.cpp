#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono>
#include <emmintrin.h>
#include <algorithm>
#include <string>

// Tamaño de vector
const unsigned int vecSize = static_cast<unsigned int>(std::pow(2, 16));

// Variables globales para tiempos
std::chrono::milliseconds GlobalCTime = std::chrono::milliseconds(0);
std::chrono::milliseconds GlobalAsmTime = std::chrono::milliseconds(0);
std::chrono::milliseconds GlobalAsmSSETime = std::chrono::milliseconds(0);

// Función auxiliar para obtener el máximo valor en un vector
int getMax(const std::vector<int>& arr) {
    return *std::max_element(arr.begin(), arr.end());
}

// Radix Sort en C++ para enteros
void radixSort(std::vector<int>& arr) {
    int max = getMax(arr);

    for (int exp = 1; max / exp > 0; exp *= 10) {
        std::vector<int> output(arr.size());
        std::vector<int> count(10, 0);

        // Contar ocurrencias de cada dígito
        for (size_t i = 0; i < arr.size(); i++)
            count[(arr[i] / exp) % 10]++;

        // Cambiar count[i] para que contenga la posición actual
        // del dígito en output[]
        for (int i = 1; i < 10; i++)
            count[i] += count[i - 1];

        // Construir el array de salida
        for (int i = arr.size() - 1; i >= 0; i--) {
            output[count[(arr[i] / exp) % 10] - 1] = arr[i];
            count[(arr[i] / exp) % 10]--;
        }

        // Copiar el array de salida al original
        arr = output;
    }
}

// Radix Sort con SSE 
void radixSortAsmSSE(std::vector<int>& arr) {
    int max = getMax(arr);
    int exp = 1;
    std::vector<int> output(arr.size());

    while (max / exp > 0) {
        std::vector<int> count(10, 0);
        int* arr_ptr = arr.data();
        size_t size = arr.size();

        // Procesamiento escalar para contar dígitos
        for (size_t i = 0; i < size; i++) {
            count[(arr[i] / exp) % 10]++;
        }

        // Acumular contadores
        for (int i = 1; i < 10; i++) {
            count[i] += count[i - 1];
        }

        // Usar SSE para la fase de redistribución (cuando exp = 1)
        if (exp == 1) {
            int* output_ptr = output.data();
            int* count_ptr = count.data();

            // Procesar en bloques de 4 elementos
            size_t sse_size = size - (size % 4);

            for (int i = size - 1; i >= static_cast<int>(sse_size); i--) {
                int digit = (arr[i] / exp) % 10;
                output[count[digit] - 1] = arr[i];
                count[digit]--;
            }

            for (int i = sse_size - 4; i >= 0; i -= 4) {
                __m128i data = _mm_loadu_si128((__m128i*)(arr_ptr + i));

                // Calcular dígitos para 4 elementos
                int digits[4];
                for (int j = 0; j < 4; j++) {
                    digits[j] = (arr[i + j] / exp) % 10;
                }

                // Obtener posiciones usando los contadores
                int positions[4];
                for (int j = 0; j < 4; j++) {
                    positions[j] = --count[digits[j]];
                }

                // Reordenar datos según las posiciones calculadas
                for (int j = 0; j < 4; j++) {
                    output[positions[j]] = arr[i + j];
                }
            }
        }
        else {
            // Versión escalar para otros exponentes
            for (int i = size - 1; i >= 0; i--) {
                int digit = (arr[i] / exp) % 10;
                output[count[digit] - 1] = arr[i];
                count[digit]--;
            }
        }

        arr = output;
        exp *= 10;
    }
}

// Generador de números aleatorios
std::vector<int> generateInt() {
    std::vector<int> seq(vecSize);
    for (unsigned int i = 0; i < vecSize; i++) {
        seq[i] = rand() % vecSize + 1;
    }
    return seq;
}

// Verificador de ordenación
bool sortVerify(const std::vector<int>& seq) {
    for (unsigned int i = 0; i < seq.size() - 1; i++) {
        if (seq[i] > seq[i + 1]) {
            return false;
        }
    }
    return true;
}

// ======== VERSIÓN ENSAMBLADOR  ========
void getMaxASM(const std::vector<int>& arr, int& max_val) {
    const int* arr_ptr = arr.data();
    int n = static_cast<int>(arr.size());
    int* non_const_ptr = const_cast<int*>(arr_ptr);

    __asm {
        MOV ECX, n
        MOV ESI, non_const_ptr
        MOV EAX, [ESI]      // Primer elemento
        MOV max_val, EAX    // Inicializar max_val
        DEC ECX             // Ya procesamos el primer elemento

        getMaxLoop :
        ADD ESI, 4          // Siguiente elemento
            MOV EBX, [ESI]      // Cargar elemento actual
            CMP EBX, max_val    // Comparar con max_val
            JLE noUpdate        // Si no es mayor, saltar
            MOV max_val, EBX    // Actualizar max_val
            noUpdate :
        LOOP getMaxLoop     // Decrementar ECX y saltar si no es cero
    }
}

void countingSortASM(std::vector<int>& arr, std::vector<int>& output, int exp) {
    int* arr_ptr = arr.data();
    int* output_ptr = output.data();
    int n = static_cast<int>(arr.size());
    int dcount[10] = { 0 };

    __asm {
        ; Inicializar dcount a ceros
        LEA EDI, dcount
        MOV ECX, 10
        XOR EAX, EAX
        REP STOSD

        ; Contar ocurrencias de cada dígito
        MOV ECX, n
        MOV ESI, arr_ptr
        countLoop :
        MOV EAX, [ESI]; Cargar arr[i]
            CDQ; Extender EAX a EDX : EAX(para división)
            MOV EBX, exp
            DIV EBX; EAX = arr[i] / exp
            XOR EDX, EDX; Limpiar EDX para nueva división
            MOV EBX, 10
            DIV EBX; EDX = (arr[i] / exp) % 10
            LEA EDI, dcount
            MOV EBX, EDX
            SHL EBX, 2; Multiplicar por 4 (tamaño de int)
            ADD EDI, EBX
            INC DWORD PTR[EDI]; Incrementar dcount[digit]
            ADD ESI, 4; Mover al siguiente elemento
            LOOP countLoop

            ; Acumular los contadores
            LEA ESI, dcount
            MOV ECX, 9
            MOV EBX, 4
            accumulateLoop:
        MOV EAX, [ESI + EBX - 4]; dcount[i - 1]
            ADD[ESI + EBX], EAX; dcount[i] += dcount[i - 1]
            ADD EBX, 4
            LOOP accumulateLoop

            ; Construir el array de salida
            MOV ECX, n
            MOV ESI, arr_ptr
            MOV EAX, ECX
            DEC EAX
            SHL EAX, 2; Multiplicar por 4 (tamaño de int)
            ADD ESI, EAX; Empezar desde el final del array
            buildLoop:
        MOV EAX, [ESI]; Cargar arr[i]
            PUSH EAX; Guardar arr[i] temporalmente
            CDQ; Extender EAX a EDX : EAX
            MOV EBX, exp
            DIV EBX; EAX = arr[i] / exp
            XOR EDX, EDX; Limpiar EDX
            MOV EBX, 10
            DIV EBX; EDX = (arr[i] / exp) % 10
            LEA EDI, dcount
            MOV EBX, EDX
            SHL EBX, 2; Multiplicar por 4 (tamaño de int)
            ADD EDI, EBX
            DEC DWORD PTR[EDI]; Decrementar dcount[digit]
            MOV EBX, [EDI]; Obtener nuevo índice
            POP EAX; Recuperar arr[i]
            MOV EDI, output_ptr; Obtener puntero a output
            SHL EBX, 2; Multiplicar por 4 (tamaño de int)
            ADD EDI, EBX
            MOV[EDI], EAX; output[dcount[digit]] = arr[i]
            SUB ESI, 4; Mover al elemento anterior
            LOOP buildLoop
    }

    // Copiar output a arr
    arr.assign(output.begin(), output.end());
}

void radixSortASM(std::vector<int>& arr) {
    if (arr.empty()) return;

    int max_val = getMax(arr);


    std::vector<int> output(arr.size());
    for (int exp = 1; max_val / exp > 0; exp *= 10) {
        countingSortASM(arr, output, exp);
    }
}

// Módulos de ejecución
void RadixSortCExecution() {
    srand(0);
    std::vector<int> intSeq = generateInt();

    auto start = std::chrono::high_resolution_clock::now();
    radixSort(intSeq);
    auto end = std::chrono::high_resolution_clock::now();

    auto timeMili = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    GlobalCTime = timeMili;

    std::cout << "\n>-C-/-C-+-+-----------\n";
    std::cout << "| INT:   " << (sortVerify(intSeq) ? "[OK - " + std::to_string(timeMili.count()) + "ms]" : "[BAD]") << "\n";
}

void RadixSortAsmSSEExecution() {
    srand(0);
    std::vector<int> intSeq = generateInt();

    auto start = std::chrono::high_resolution_clock::now();
    radixSortAsmSSE(intSeq);
    auto end = std::chrono::high_resolution_clock::now();

    auto timeMili = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    GlobalAsmSSETime = timeMili;

    std::cout << "\n>-A-S-M-x-8-6-S-S-E---\n";
    std::cout << "| INT:   " << (sortVerify(intSeq) ? "[OK - " + std::to_string(timeMili.count()) + "ms]" : "[BAD]") << "\n";
}

void RadixSortAsmExecution() {
    srand(0);
    std::vector<int> intSeq = generateInt();

    auto start = std::chrono::high_resolution_clock::now();
    radixSortASM(intSeq);
    auto end = std::chrono::high_resolution_clock::now();

    auto timeMili = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    GlobalAsmTime = timeMili;

    std::cout << "\n>-A-S-M---x-8-6-------\n";
    std::cout << "| INT:   " << (sortVerify(intSeq) ? "[OK - " + std::to_string(timeMili.count()) + "ms]" : "[BAD]") << "\n";
}

std::chrono::milliseconds BenchRadixSort() {
    std::cout << "\n/-R-a-d-i-x-S-o-r-t---";

    RadixSortAsmExecution();
    RadixSortAsmSSEExecution();
    RadixSortCExecution();

    // Verificar división por cero
    if (GlobalAsmTime.count() != 0 && GlobalAsmSSETime.count() != 0) {
        // Calcular porcentajes y redondear
        double asmBoost = std::round((static_cast<double>(GlobalCTime.count()) / GlobalAsmTime.count()) * 100.0);
        double sseBoost = std::round((static_cast<double>(GlobalCTime.count()) / GlobalAsmSSETime.count()) * 100.0);

        // Imprimir resultados
        std::cout << "\n|\n| COMPARING TO C/C++:\n"
            << "| ASM BOOST: " << asmBoost << "%\n"
            << "| SSE BOOST: " << sseBoost << "%\n"
            << R"(\)" << std::endl;
    }
    else {
        std::cout << "\n|\n| ERROR: División por cero. GlobalAsmTime o GlobalAsmSSETime son cero.\n"
            << R"(\)" << std::endl;
    }

    return (GlobalAsmTime + GlobalCTime + GlobalAsmSSETime);
}