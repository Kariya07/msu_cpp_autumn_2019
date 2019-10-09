#include <iostream>
#include "functions.h"
void column()
{
    int n = 10000;
    int** a = new int*[n];
    for (int i = 0; i < n; i++){
        a[i] = new int[n];
    }
    for (int i = 0; i < n; i++){
        for( int j = 0; j < n; j++){
            a[i][j] = rand() % 10;
        }
    }
    volatile int sum = 0;
    Timer t;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            sum += a[j][i];
        }
    }
    for (int i = 0; i < n; i++){
        delete[] a[i];
    }
    delete[] a;
}

