#pragma once

#ifndef RADIXSORT_H
#define RADIXSORT_H

#include <vector>
#include <chrono>

// Declaraciones de funciones
int getMax(const std::vector<int>& arr);
void radixSort(std::vector<int>& arr);
void radixSortAsmSSE(std::vector<int>& arr);
bool sortVerify(const std::vector<int>& seq);

// Ensamblador
void getMaxASM(const std::vector<int>& arr, int& max_val);
void countingSortASM(std::vector<int>& arr, std::vector<int>& output, int exp);
void radixSortASM(std::vector<int>& arr);

// Ejecuciones
void RadixSortCExecution();
void RadixSortAsmSSEExecution();
void RadixSortAsmExecution();

std::chrono::milliseconds BenchRadixSort();

#endif