#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "program.h"

int main()
{   
    //v este vectorul in care se pastreaza datele site-urilor
    sites *v;
    //initializam vectorul cu o marime de 3 elemente, conform cerintei
    v = malloc(3 * sizeof(sites));
    int dimensiune = 3;

    FILE *f, *s;
    f = fopen("master.txt", "rt");

    char site[30], aux[100], culoare_font[7], culoare_bg[7];
    //z este numarul de site-uri retinute in vector - 1
    //adica pozitia ultimului site retinut in vector
    int z=-1, i;
    //cu vectorul site citim, pe rand, numele fiecarui site din fisierul master.txt
    while(fscanf(f, "%s", site) != EOF)
    {
        z++;
        //daca numarul de site-uri este mai mare decat marimea alocata,
        //realocam vectorul cu o marime mai mare cu 3 elemente
        if(z >= dimensiune)
        {
            dimensiune = dimensiune + 3;
            v = realloc(v, dimensiune * sizeof(sites));
        }
        //deschiderea, pe rand, a fisierelor
        s = fopen(site, "rt");

        //prelucrarea, pe rand, a datelor din fisiere
        fscanf(s, "%s%d%d%d", v[z].URL, &v[z].lungime, &v[z].nr_acces, &v[z].checksum);
        //alocam cu 1 in plus pentru terminatorul de sir '\0'
        v[z].cod = (char*)malloc((v[z].lungime + 1) * sizeof(char));

        //citim fiecare linie din codul site-ului si le concatenam in
        //vectorul v[z].cod
        v[z].cod[0] = '\0';

        //citim newline-ul care face trecerea de la fscanf la fgets
        fgets(aux, 100, s);

        //citim primul rand separat, ca sa nu ii concatenam un endline inainte
        fgets(aux, 100, s);
        aux[strlen(aux) - 1] = '\0';
        strcat(v[z].cod, aux);

        while(fgets(aux, 100, s) != NULL)
        {
            //scoatem newline-ul de la sfarsitul sirului citit prin fgets
            aux[strlen(aux) - 1] = '\0';
            strcat(v[z].cod, "\n");
            strcat(v[z].cod, aux);
        }
        //dupa ultimul rand citit nu mai exista un newline, de aceea, in loc de
        //newline, va sterge ultimul caracter, care este mereu '>'
        v[z].cod[v[z].lungime - 1] = '>';
        v[z].cod[v[z].lungime] = '\0';

        //numarul de caractere ale titlului
        //scadem 7 pentru ca stringul "<title>" are 7 caractere
        v[z].nr_titlu = strstr(v[z].cod, "</title>") - strstr(v[z].cod, "<title>") - 7;

        //copiem caracterele respective in v[z].titlu
        for(i = 0; i <= v[z].nr_titlu - 1; i++)
            v[z].titlu[i] = (strstr(v[z].cod, "<title>") + 7)[i];
        v[z].titlu[v[z].nr_titlu] = '\0';

        //culoarea textului
        //conditia de existenta a culorii textului (daca gaseste "color" si
        //nu are caracterul '-' inainte)
        if(strstr(v[z].cod, "color:") != 0 && (strstr(v[z].cod, "color:") - 1)[0] != '-')
        {
            //copiaza culoarea in stringul culoare_font
            i = 0;
            strcpy(culoare_font,"");
            while((strstr(v[z].cod, "color:") + 6)[i] != ';')
            {
                culoare_font[i] = (strstr(v[z].cod, "color:") + 6)[i];
                i++;
            }
            culoare_font[i] = '\0';
        }
        else
            //culoarea default
            strcpy(culoare_font, "black");

        //culoarea de background
        //conditia de existenta a culorii de background
        if(strstr(v[z].cod, "background-color:")!=0)
        {
            //copiaza culoarea in stringul culoare_bg
            i = 0;
            strcpy(culoare_bg, "");
            while((strstr(v[z].cod, "background-color:") + 17)[i] != ';')
            {
                culoare_bg[i] = (strstr(v[z].cod, "background-color:") + 17)[i];
                i++;
            }
            culoare_bg[i] = '\0';
        }
        else
            //culoarea default
            strcpy(culoare_bg,"white");

        //compara culorile gasite cu cele din enunt pentru a-i da valoarea
        //corespunzatoare din enum-ul din structura
        if(strcmp(culoare_font, "white") == 0)
            v[z].font_color = white;
        else
            if(strcmp(culoare_font, "black") == 0)
                v[z].font_color = black;
            else
                if(strcmp(culoare_font, "red") == 0)
                    v[z].font_color = red;
                else
                    if(strcmp(culoare_font, "green") == 0)
                        v[z].font_color = green;
                    else
                        if(strcmp(culoare_font, "blue") == 0)
                            v[z].font_color = blue;
                        else
                            v[z].font_color = yellow;

        if(strcmp(culoare_bg, "white") == 0)
            v[z].bg_color = white;
        else
            if(strcmp(culoare_bg, "black") == 0)
                v[z].bg_color = black;
            else
                if(strcmp(culoare_bg, "red") == 0)
                    v[z].bg_color = red;
                else
                    if(strcmp(culoare_bg, "green") == 0)
                        v[z].bg_color = green;
                    else
                        if(strcmp(culoare_bg, "blue") == 0)
                            v[z].bg_color = blue;
                        else
                            v[z].bg_color = yellow;
        
        printf("%s %d %s\n",v[z].URL, v[z].nr_acces, v[z].titlu);
        //inchidem fisierele, pe rand, dupa ce am terminat de lucrat cu ele
        fclose(s);
    }

    //eliberam memoria
    for(i = 0; i <= z; i++)
        {
            free(v[i].cod);
            v[i].cod = NULL;
        }
    free(v);
    //inchidem fisierul initial
    fclose(f);
    return 0;
}
