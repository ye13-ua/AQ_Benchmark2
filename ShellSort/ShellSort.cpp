// Explicaciñon mucho mejor que la mie de Shell sort: https://www.geeksforgeeks.org/shell-sort/

// PENDIENTES: ARREGLAR ASM FLOAT

#include <iostream> // Cout
#include <iomanip>  // setw()
#include <vector>   // Vectores
#include <cstdlib>  // Números (pseudo)aleatorios
#include <ctime>    // Tiempo para semilla aleatoria
#include <cmath>    // potencias
#include <chrono>   // Cronómetro del tiempo
#include <emmintrin.h> // Intel SSE

// Tamaño de vector genérico
const unsigned int vecSize = static_cast<unsigned int>(std::pow(2,16));
// Tamaño de vector extendido - no usado
const unsigned int vecSizeExt = static_cast<unsigned int>(std::pow(2, 32));

std::chrono::milliseconds GlobalCTime = std::chrono::milliseconds(0);
std::chrono::milliseconds GlobalAsmTime = std::chrono::milliseconds(0);
std::chrono::milliseconds GlobalAsmSSETime = std::chrono::milliseconds(0);

// En su base, el shell sort es insertion sort pero con el problema de elementos de valor muy bajo en una posición muy profunda.

// C++ Shell sort para enteros 
void shellSort(std::vector<int> & seq) {
    unsigned int size = seq.size(); // En caso de que el valor sea dinámico

    // Gap inicial es tamaño/2 y cada iteración se divide entre 2
    // Gap define la distancia entre elementos comparados
    for (unsigned int gap = size / 2; gap>0; gap /= 2) {
        // Iniciamos la i a gap
        // Básicamente realicamos el insertion sort
        for (unsigned int i = gap; i<size; i++) {
            // Guardamos elemento i como temporal
            int temp = seq[i];
            unsigned int j;
            // Iniciamos j a i e iteramos hasta que j no sea inferior a gap o el valor de [j-gap] sea inferio al valor guardado anteriormente
            // Básicamente conparamos el elemento pivote con el elemento gap posiciónes por detras
            for (j = i; j>=gap && seq[j-gap]>temp; j-=gap) {
                // Deslizamos elementos más grandes hacia la derecha
                seq[j] = seq[j - gap];
            }
            // Ponemos el valor guardado en su posición ordenada
            seq[j] = temp;
        }
    }
}
// C++ Shell sort para coma flotante
// Idéntico al C++ Shell sort deenteros pero con tipo de dato float
void shellSort(std::vector<float> & seq) {
    unsigned int size = seq.size();

    for (unsigned int gap = size / 2; gap > 0; gap /= 2) {
        for (unsigned int i = gap; i < size; i++) {
            float temp = seq[i];
            unsigned int j;
            for (j = i; j >= gap && seq[j - gap] > temp; j -= gap) {
                seq[j] = seq[j - gap];
            }
            seq[j] = temp;
        }
    }
}

// Sobre inline asm:
// Sistemas de GCC:
// __asm__ con volatile entre paréntesis con comillas (""); GCC Syntax
// Sistemas de MSVC:
// Usar __asm / _asm o asm entre llaves {}; Intel Syntax

// ASM Shell sort para enteros
void shellSortAsm(std::vector<int> & seq) {
    // Segemnto C/C++
    int* seq_ptr = &seq[0]; // Primer elemento puntero
    unsigned int size = seq.size(); // Tamaño
    unsigned int gap = size / 2;  // Gap = Tamaño/2

    // Utilizamos el bucle de C para envolver el script de asm
    while (gap>0) {
        // Bucle de recorrido gap
        for (unsigned int i = gap; i < size; i++) {
            int temp = seq_ptr[i]; // Valor actual en temporal
            unsigned int j = i;
            __asm {
                mov eax, j // EAX = índice
                mov ebx, gap // EBX = gap o distancia
                mov edi, seq_ptr // EDI = punto inicial del vector

                inner_loop :
                // Si j < gap, salir del bucle interno asm
                cmp eax, ebx
                    jb inner_loop_exit

                    // Calculamos la dirección &arr[0] + (eax - ebx) * 4
                    mov ecx, eax
                    sub ecx, ebx // ECX = j - gap
                    mov edx, dword ptr[edi + ecx * 4] // EDX = cargamos valor de [inicio - ecx(j-gap) * 4]

                    // Compara el valor obtenido con el guardado como temp
                    // Si el valor nuevo es menos o igual a guardado, salimos
                    cmp edx, temp
                    jle inner_loop_exit

                    //Si no, se mueve el nuevo a la posición de [j]
                    mov dword ptr[edi + eax * 4], edx

                    // j-gap (j en eax)
                    sub eax, ebx
                    jmp inner_loop

                    inner_loop_exit :
                // Guardamos el valor de eax en j
                mov j, eax
            }
            // Se mueve el puntero tras ejecutaar parte asm
            seq_ptr[j] = temp;
        }
        // Reducir el gap en 2
        gap /= 2;
    }
}

// ASM Shell sort para floats
// Temporalmente/Permanentemente reemplazado por copia del ASM para enteros debido a problemas de inestabilidad
// Si se lee este mensaje,implica que lo más probable es que no me dio tiempo a reemplkazarlo por código adaptado a FPU de manera apropiada
void shellSortAsm(std::vector<float> & seq) {
    // Segemnto C/C++
    float* seq_ptr = &seq[0]; 
    unsigned int size = seq.size(); 
    unsigned int gap = size / 2;

    while (gap > 0) {
        for (unsigned int i = gap; i < size; i++) {
            float temp = seq_ptr[i]; 
            unsigned int j = i;
            __asm {
                mov eax, j          
                mov ebx, gap        
                mov edi, seq_ptr

                inner_loop :
                cmp eax, ebx
                    jb inner_loop_exit

                    mov ecx, eax
                    sub ecx, ebx        
                    mov edx, dword ptr[edi + ecx * 4]

                    cmp edx, temp
                    jle inner_loop_exit

                    mov dword ptr[edi + eax * 4], edx

                    sub eax, ebx
                    jmp inner_loop

                    inner_loop_exit :
                mov j, eax
            }
            seq_ptr[j] = temp;
        }
        gap /= 2;
    }
}

// ASM ShellSort con SSE/SIMD para enteros
// Algo similar al ASM normal pero con más trucos
// Lo único es que no me salió implementar una paralelización superior, por locual la paralelización de verdad ocurre solo parcialmente,
// esto reduce considerablemente el rendimiento en el contexto del tamaño de problema establecido.
void shellSortAsmSSE(std::vector<int>& seq) {
    int* arr = seq.data(); // Puntero a datos del vector
    unsigned int size = seq.size(); // Tamaño
    unsigned int gap = size / 2; // Gap

    // Bucle externo
    while (gap > 0) {
        // Bucel gap
        for (unsigned int i = gap; i < size; i++) {
            int temp = arr[i]; // Guardamos valor actual
            int j = i;
            // Condición de uso SSE
            if (gap == 1 && j >= 4) {
                // Vector temporal con 4 elementos
                __m128i tempVec = _mm_set1_epi32(temp);
                // Se procesan 4 elementos anteriores
                while (j >= 4) {
                    // Elementos desde j-4 hasta j-1
                    __m128i block = _mm_loadu_si128((__m128i*)(arr + j - 4));
                    // Comparamos elementos de dos bloques de 4
                    __m128i cmp = _mm_cmpgt_epi32(block, tempVec);
                    // Se genera una máscara en base a la comparación
                    int mask = _mm_movemask_epi8(cmp);
                    
                    // La máscara equivaldrá a 0 si ningún elemento es mayor que temp
                    if (mask == 0)
                        break;
                   
                    // Si eso no es así, se procesa el j-1
                    if (arr[j - 1] > temp) {
                        arr[j] = arr[j - 1]; // Movemos j-1 a posición j
                        j--;
                    }
                    else {
                        break;
                    }
                }
            }
            
            // En el caso de no poder usar paralelización SSE, utilizamos algoritmo estándart
            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = temp;
        }
        gap /= 2;
    }
}

// A diferencia de ASM inline normal, con SSE es más facil implementar el float tras implementar int;
// Solo hace falta sustituir las funciónes y los tipos por apropiados en vez de reescribir elementos para FPU
// ej: __m128i pasa a ser __m128 y las funciones sustituyen el _si182, _epi32, etc. por _ps

// ASM ShellSort con SSE/SIMD para flotantes
void shellSortAsmSSE(std::vector<float>& seq) {
    float* arr = seq.data();
    unsigned int size = seq.size();
    unsigned int gap = size / 2;

    while (gap > 0) {
        for (unsigned int i = gap; i < size; i++) {
            float temp = arr[i];
            int j = i;
            
            if (gap == 1 && j >= 4) {
                
                __m128 tempVec = _mm_set1_ps(temp);
                
                while (j >= 4) {
                    
                    __m128 block = _mm_loadu_ps(arr + j - 4);

                    __m128 cmp = _mm_cmpgt_ps(block, tempVec);

                    int mask = _mm_movemask_ps(cmp);

                    if (mask == 0)
                        break;

                    if (arr[j - 1] > temp) {
                        arr[j] = arr[j - 1];
                        j--;
                    }
                    else {
                        break;
                    }
                }
            }

            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = temp;
        }
        gap /= 2;
    }
}

// Generador de vector con número aleatorios de tipo entero
std::vector<int> generateInt() {
    std::vector<int> seq(vecSize);
    for (unsigned int i = 0; i<vecSize; i++) {
        seq[i] = rand() % vecSize + 1;
    }
    return seq;
}

// Generador de un valor pseudo aleatorio en coma flotante
float randFloat() {
    return (float)(rand() % vecSize + 1) * 1.13;
}

// Generador de vector con números aleatorios de tipo flotante
std::vector<float> generateFloat() {
    std::vector<float> seq(vecSize);
    for (unsigned int i = 0; i < vecSize; i++) {
        seq[i] = randFloat();
    }
    return seq;
}

// Verifica el orden de los valores en array INT/FLOAT
bool sortVerify(std::vector<int> seq) {
    for (unsigned int i = 0; i < vecSize - 1; i++) {
        if (seq[i] > seq[i + 1]) {
            return false;
        }
    } return true;
}
bool sortVerify(std::vector<float> seq) {
    for (unsigned int i = 0; i < vecSize - 1; i++) {
        if (seq[i] > seq[i + 1]) {
            return false;
        }
    } return true;
}

// Funciónes para imprimir los vectores de números de manera agradable
void printArray(std::vector<int> seq) {
    std::cout << "INT VECTOR DATA:\n";
    for (unsigned int i = 0; i < seq.size(); i++) {
        std::cout << std::setw(8) << seq[i] << " ";
        if (i % 10 == 0) std::cout << "\n";
    }std::cout << "\n";
}
void printArray(std::vector<float> seq) {
    std::cout << "FLOAT VECTOR DATA:\n";
    for (unsigned int i = 0; i < seq.size(); i++) {
        std::cout << std::setw(8) << std::setprecision(7) << seq[i] << " ";
        if (i % 10 == 0) std::cout << "\n";
    }std::cout << "\n";
}

// Módulo de ejecución principal de algoritmos en C/C++
void ShellSortCExecution() {

    // Redundante si se invoca una vez en el main ya que con un único generador nos sirve.
    // Generamos la semilla principal
    // Usamos el valor de srand(0) para una generación constante,
    // y srand(static_cast<unsigned>(time(nullptr))); para pseudoaleatoria
    // A modo experimental se ve que los resultados de ASM son máss volatiles que los de C/C++
    srand(0);

    // Generamos dos vectores nuevos de valores aleatorios
    std::vector<int> intSeq = generateInt();
    std::vector<float> floatSeq = generateFloat();

    // "auto" equvale a "std::chrono::high_resolution_clock::time_point" excepto en el caso de duration_cast
    // Iniciamos contar el tiempo total de ejecución de los dos algoritmos
    auto totalCTimeMiliStart = std::chrono::high_resolution_clock::now();
    
    // Ordenamos el vector de enteros y contamos timepo
    auto intStart = std::chrono::high_resolution_clock::now();
    shellSort(intSeq);
    auto intEnd = std::chrono::high_resolution_clock::now();
    auto intTimeMili = std::chrono::duration_cast<std::chrono::milliseconds>(intEnd - intStart);

    // Lo mismo para punto flotante
    auto floatStart = std::chrono::high_resolution_clock::now();
    shellSort(floatSeq);
    auto floatEnd = std::chrono::high_resolution_clock::now();
    auto floatTimeMili = std::chrono::duration_cast<std::chrono::milliseconds>(floatEnd-floatStart);

    // Terminamos el cronometro general
    auto totalCTimeMiliEnd = std::chrono::high_resolution_clock::now();

    std::cout << "\n>-C-/-C-+-+-----------\n" << std::flush; // Separador
    // Verificamos el resultado final e imprimimos valores de tiempo locales
    std::cout << "| INT:   " << std::flush;   sortVerify(intSeq)   ? std::cout << "[OK - " << intTimeMili.count() <<"ms]" << std::flush : std::cout << "[BAD]" << std::flush;
    std::cout << "\n| FLOAT: " << std::flush; sortVerify(floatSeq) ? std::cout << "[OK - " << floatTimeMili.count() << "ms]" << std::flush : std::cout << "[BAD]" << std::flush;

    // Imprimimos el tiempo total de la ejecución
    auto totalCTime = std::chrono::duration_cast<std::chrono::milliseconds>(totalCTimeMiliEnd-totalCTimeMiliStart);
    std::cout << "\n| TOTALTIME: [" << totalCTime.count() << "ms]" << std::flush;

    // Guardamos el valor global de timepo de ejecución del algoritmo
    GlobalCTime = totalCTime;
}

// VIrtualmente lo mismo pero se usan losalgoritmos con _asm
void ShellSortAsmExecution() {

    // Redundante si se invoca una vez en el main ya que con un único generador nos sirve
    // Generamos la semilla principal
    // Usamos el valor de srand(0) para una generación constante,
    // y srand(static_cast<unsigned>(time(nullptr))); para pseudoaleatoria
    srand(0);

    // Generamos nuevos vectores
    std::vector<int> intSeq = generateInt();
    std::vector<float> floatSeq = generateFloat();

    // "auto" equvale a "std::chrono::high_resolution_clock::time_point" excepto en el acso de duration_cast
    // Iniciamos contar el tiempo total de ejecución de los dos algoritmos
    auto totalAsmTimeMiliStart = std::chrono::high_resolution_clock::now();

    // Ordenamos el vector de enteros y contamos timepo
    auto intStart = std::chrono::high_resolution_clock::now();
    shellSortAsm(intSeq);
    auto intEnd = std::chrono::high_resolution_clock::now();
    auto intTimeMili = std::chrono::duration_cast<std::chrono::milliseconds>(intEnd - intStart);

    // Lo mismo para punto flotante
    auto floatStart = std::chrono::high_resolution_clock::now();
    shellSortAsm(floatSeq);
    auto floatEnd = std::chrono::high_resolution_clock::now();
    auto floatTimeMili = std::chrono::duration_cast<std::chrono::milliseconds>(floatEnd - floatStart);

    auto totalAsmTimeMiliEnd = std::chrono::high_resolution_clock::now();
    std::cout << "\n>-A-S-M-x-8-6---------\n" << std::flush; // Separador
    std::cout << "| INT:   " << std::flush;   sortVerify(intSeq) ? std::cout << "[OK - " << intTimeMili.count() << "ms]" << std::flush : std::cout << "[BAD]" << std::flush;
    std::cout << "\n| FLOAT: " << std::flush; sortVerify(floatSeq) ? std::cout << "[OK - " << floatTimeMili.count() << "ms]" << std::flush : std::cout << "[BAD]" << std::flush;

    auto totalAsmTime = std::chrono::duration_cast<std::chrono::milliseconds>(totalAsmTimeMiliEnd - totalAsmTimeMiliStart);
    std::cout << "\n| TOTALTIME: [" << totalAsmTime.count() << "ms]" << std::flush;

    GlobalAsmTime = totalAsmTime;
}

// Implementación con reglas SSE/SIMD
void ShellSortAsmSSEExecution() {
    
    // srand(static_cast<unsigned>(time(nullptr)));
    srand(0);

    // Vectores nuevos
    std::vector<int> intSeq = generateInt();
    std::vector<float> floatSeq = generateFloat();

    // Tiempo inicial
    auto totalAsmSSETimeMiliStart = std::chrono::high_resolution_clock::now();

    // Entero
    auto intStart = std::chrono::high_resolution_clock::now();
    shellSortAsmSSE(intSeq);
    auto intEnd = std::chrono::high_resolution_clock::now();
    auto intTimeMili = std::chrono::duration_cast<std::chrono::milliseconds>(intEnd - intStart);

    // Flotante
    auto floatStart = std::chrono::high_resolution_clock::now();
    shellSortAsmSSE(floatSeq);
    auto floatEnd = std::chrono::high_resolution_clock::now();
    auto floatTimeMili = std::chrono::duration_cast<std::chrono::milliseconds>(floatEnd - floatStart);

    // Interfaz
    std::cout << "\n>-A-S-M-x-8-6-S-S-E---\n" << std::flush; // Separador
    std::cout << "| INT:   " << std::flush;   sortVerify(intSeq) ? std::cout << "[OK - " << intTimeMili.count() << "ms]" << std::flush : std::cout << "[BAD]" << std::flush;
    std::cout << "\n| FLOAT: " << std::flush; sortVerify(floatSeq) ? std::cout << "[OK - " << floatTimeMili.count() << "ms]" << std::flush : std::cout << "[BAD]" << std::flush;

    // Tiepo fianl
    auto totalAsmSSETimeMiliEnd = std::chrono::high_resolution_clock::now();

    // Tiempo  total
    auto totalAsmSSETime = std::chrono::duration_cast<std::chrono::milliseconds>(totalAsmSSETimeMiliEnd - totalAsmSSETimeMiliStart);
    std::cout << "\n| TOTALTIME: [" << totalAsmSSETime.count() << "ms]" << std::flush;

    GlobalAsmSSETime = totalAsmSSETime;
}

std::chrono::milliseconds BenchShellSort() {
    std::cout << "\n/-S-h-e-l-l-S-o-r-t---" << std::flush;

    // Ejecución de los segmentos separados
    ShellSortCExecution();
    ShellSortAsmExecution();
    ShellSortAsmSSEExecution();
    

    // El porcentaje que indica el aumento de rendimiento en respecto a versión anterior
    std::cout << "\n|\n| COMPARING TO C/C++:\n| ASM BOOST: " << (GlobalCTime / GlobalAsmTime) * 100 << "%";
    std::cout << "\n| SSE BOOST: " << (GlobalCTime / GlobalAsmSSETime) * 100 << "%";
    std::cout << "\n" <<R"(\)"<< "" << std::flush;

    // Devuelve el timepo total que necesitó el ordenador para ejecutar el benchmark
    return (GlobalAsmTime + GlobalCTime + GlobalAsmSSETime);
}