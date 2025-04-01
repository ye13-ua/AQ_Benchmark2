#include <iostream> // cout
#include <vector>   // Vectores de datos
#include <chrono>   // Cronómetro del tiempo

// Headers de algoritmos
#include "ShellSort.h"
#include "CountingSort.h"
#include "Criba.h"

// Banderas de ejecución de algoritmos
bool flagShell = false; // s
bool flagBucket = false; // b
bool flagCounting = false; // c
bool flagInsertion = false; // i
bool flagCribaEratos = false; // e

// Título
// Se guarda dentro del codigo para ahorrar la necesidad de manipular archivos
char title[] = R"(
__/\\\\\\\\\\\\\____/\\\\\\\\\\\\\\\__/\\\\\_____/\\\________/\\\\\\\\\__/\\\________/\\\____/\\\\\\\\\_________________________        
 _\/\\\/////////\\\_\/\\\///////////__\/\\\\\\___\/\\\_____/\\\////////__\/\\\_______\/\\\__/\\\///////\\\_______________________       
  _\/\\\_______\/\\\_\/\\\_____________\/\\\/\\\__\/\\\___/\\\/___________\/\\\_______\/\\\_\///______\//\\\______________________      
   _\/\\\\\\\\\\\\\\__\/\\\\\\\\\\\_____\/\\\//\\\_\/\\\__/\\\_____________\/\\\\\\\\\\\\\\\___________/\\\/_____________/\\\\\____     
    _\/\\\/////////\\\_\/\\\///////______\/\\\\//\\\\/\\\_\/\\\_____________\/\\\/////////\\\________/\\\//_____________/\\\///\\\__    
     _\/\\\_______\/\\\_\/\\\_____________\/\\\_\//\\\/\\\_\//\\\____________\/\\\_______\/\\\_____/\\\//_______________/\\\__\//\\\_   
      _\/\\\_______\/\\\_\/\\\_____________\/\\\__\//\\\\\\__\///\\\__________\/\\\_______\/\\\___/\\\/_________________\//\\\__/\\\__  
       _\/\\\\\\\\\\\\\/__\/\\\\\\\\\\\\\\\_\/\\\___\//\\\\\____\////\\\\\\\\\_\/\\\_______\/\\\__/\\\\\\\\\\\\\\\__/\\\__\///\\\\\/___ 
        _\/////////////____\///////////////__\///_____\/////________\/////////__\///________\///__\///////////////__\///_____\/////_____
)";

bool parameter_parser(int argc, char * argv[]) {
    // Mostramos por pantalla los parámetros introducidos
    std::cout << "\t{" << argv[0] << "} - FOR MORE INFO: readme.md\n";
    std::cout << "\n\t[PARAMETERS: " << argc-1 << "]\n";
    for (unsigned int p = 1; p < argc; p++) {
        std::cout << "\t[" << p << ". " << argv[p] << "]\n";
    }
    // Sin parámetros se ejecuta todo por defecto
    if (argc == 1) {
        flagBucket = flagCounting = flagCribaEratos = flagInsertion = flagShell = true;
    }
    // Con parámetros se evalua uno por uno
    for (unsigned int i = 1; i<argc; i++) {
        // Cadapa rámetro válido tiene que empezar con '-' para ser procesado
        if (argv[i][0]=='-') {
            for (unsigned int j = 1; argv[i][j] != '\0'; j++) {
                switch (argv[i][j]) {
                case 's': flagShell = true; break;
                case 'b': flagBucket = true; break;
                case 'c': flagCounting = true; break;
                case 'i': flagInsertion = true; break;
                case 'e': flagCribaEratos =  true; break;
                default:
                    std::cout << "\tWARNING: UNKNOWN char PARAMETER AT ARGUMENT ["
                        << i << "] WITH VALUE: [" << argv[i][j] << "] - VALUE IGNORED\n";
                    break;
                }
            }
        }
        else {
            std::cout << "\n\tWARNING: INVALID PARAMETER AT [" << i << "] WITH VALUE: [" << argv[i] << "] - PARAMETER IGNORED\n";
        }
    }
    // En caso de que se intoduce al menos un parámetro, y no es válido
    if (!flagBucket && !flagCounting && !flagCribaEratos && !flagInsertion && !flagShell) return false;
    
    // Lista con Benches a ejecutar
    std::cout << "\n\tBENCH2.o: ["<< flagShell << flagBucket << flagCounting
              << flagInsertion << flagCribaEratos << "]\n";
    
    return true;
}

int main(int argc, char * argv[]) {

    // Tiempo total del benchmark
    std::chrono::milliseconds TotalBenchTime = std::chrono::milliseconds(0);

    // Escribe el titulo
    std::cout << title;

    // Leemos y validamos los parámetros
    if (!parameter_parser(argc, argv)) return -1;

    // Ejecución de los algoritmos
    if (flagShell) TotalBenchTime += BenchShellSort(); // ShellSort
    if (flagCounting) TotalBenchTime += BenchCountingSort();
    if (flagInsertion);
    if (flagCribaEratos) TotalBenchTime += BenchCriba(); // Criba

    std::cout << "\n [GLOBAL EXECUTION TIME: " << TotalBenchTime.count() <<"ms]\n PRESS ENTER TO EXIT:";

    // Salida del programa
    std::cin.get();
    return 0;
}
