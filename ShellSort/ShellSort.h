#pragma once
// shellsort.h
#ifndef SHELLSORT_H
#define SHELLSORT_H

#include <vector>
#include <chrono>

// BenchSort
std::chrono::milliseconds BenchShellSort();

// Ejecuciónes de algoritmos con contadores integrados
void ShellSortCExecution();
void ShellSortAsmExecution();
void ShellSortAsmSslExecution();

// Algoritmos sueltos
void shellSort(std::vector<int>& seq);
void shellSort(std::vector<float>& seq);

void shellSortAsm(std::vector<int>& seq);
void shellSortAsm(std::vector<float>& seq);

void shellSortAsmSsl(std::vector<int>& seq);
void shellSortAsmSsl(std::vector<float>& seq);

#endif