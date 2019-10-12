#include <iostream>
#include "functions.h"
void row()
{
    int n = 10000;
    int* a = new int[n*n];
    for (int i = 0; i < n*n; i++){
        a[i] = rand() % 10;
    }
    volatile int sum = 0;
    Timer t;
    for (int i = 0; i < n*n; i++){
        sum += a[i];
    }
    delete[] a;
}
