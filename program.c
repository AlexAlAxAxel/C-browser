#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "program.h"

//functia de interschimbare a doua elemente de tip sites
void swap(sites *x, sites *y) {
    sites aux;
    aux = *x;
    *x = *y;
    *y = aux;
}

//functia generala de sortare pentru task-urile 2 si 3
void sort(sites v[], int n, int (*cmp)(sites x, sites y))
{
    int i, j;
    for(i = 0; i <= n-1; i++)
        for(j = i+1; j <= n; j++)
            if(cmp(v[i], v[j]) == -1)
                swap(&v[i], &v[j]);
}