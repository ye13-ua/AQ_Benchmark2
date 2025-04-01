#pragma once //CountingSort.h
#ifndef COUNTINGSORT_H
#define COUNTINGSORT_H

#include <vector>
#include <chrono>	
#include <iostream>

std::chrono::milliseconds BenchCountingSort();

//Ejecución del algoritmo
void CountingSortExecution();
void CountingSortAsmExecution();
void CountingSortAsmSSExecution();

//Implementación del algoritmo
void countingSort(vector<int>& seq);
void countingSort(vector<float>& seq);

void countingSortAsm(vector<int>& seq);

void countingSortAsmSSE(vector<int>& seq);

#endif