#ifndef PROGRAM_H_
#define PROGRAM_H_

//structura cu care se retin datele fiecarui site
typedef struct sites
{
    char URL[50], titlu[50];
    int lungime, nr_acces, checksum, nr_titlu;
    char *cod, *sir;
    enum color { white=7, black=0, red=1, green=2, blue=4, yellow=3 } bg_color, font_color;
}sites;

void swap(sites *x, sites *y);

void sort(sites v[], int n, int (*cmp)(sites x, sites y));

#endif