#include <iostream>
#include "functions.h"
void column()
{
    int n = 10000;
    int* a = new int[n*n];
    for (int i = 0; i < n*n; i++){
        a[i] = rand() % 10;
    }
    volatile int sum = 0;
    Timer t;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            sum += a[i + j*n];
        }
    }
    delete [] a;
}

