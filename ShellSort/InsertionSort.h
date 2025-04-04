#ifndef INSERTIONSORT_H_
#define INSERTIONSORT_H_

#pragma once
#include <iostream>		
#include <iomanip>		
#include <vector>		
#include <cmath>		
#include <chrono>		
#include <emmintrin.h>	
#include <xmmintrin.h>
#include <immintrin.h>
#include <time.h>
#include <stdlib.h> 
#include <string> 

using namespace std;

const unsigned int vecSize = pow(2, 14);

void initSeed();
bool sortVerifyInt(vector<int> vec);
bool sortVerifyFloat(vector<float> vec);

vector<int> generateIntVector(int vecSize);
vector<float> generateFloatVector(int vecSize);
std::chrono::milliseconds BenchInsertionSort();

vector<int> insertionSortExecutionC(vector<int> vec);
vector<float> insertionSortExecutionC(vector<float> vec);
vector<int> insertionSortExecutionAsm(vector<int> vec);
vector<float> insertionSortExecutionAsm(vector<float> vec);
vector<int> insertionSortExecutionSSE(vector<int> vec);
vector<float> insertionSortExecutionSSE(vector<float> vec);


#endif // !INSERTIONSORT_H_
