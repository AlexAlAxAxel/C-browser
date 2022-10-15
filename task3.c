#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "program.h"


//functia particulara de sortare pentru task-ul 3
int cmp_task3(sites a, sites b)
{
    //ordonare descrescatoare dupa numarul de accesari
    if(a.nr_acces < b.nr_acces)
        return -1;
    else
        return 1;    
}


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
    int z=-1, i, j;
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
        
        //inchidem fisierele, pe rand, dupa ce am terminat de lucrat cu ele
        fclose(s);
    }

    int (*cmps)(sites, sites) = &cmp_task3;

    //t este numarul de cuvinte citite de la tastatura - 1
    int t = -1;
    fgets(aux, 100, stdin);
    aux[strlen(aux) - 1] = '\0';

    //a este vectorul de cuvinte citite de la tastatura
    char *p, a[100][100], exclude[100] = "";
    int sa_compus = 1;
    char string_compus[100] = "";

    //punem cuvintele citite in vectorul de cuvinte, cuvantul exclus in 
    //stringul exclude si secventa citita cu "" in stringul string_compus
    p = strtok(aux, " ");
    while(p != NULL)
    {
        //daca inca nu s-a terminat secventa citita
        if(sa_compus == 0)
        {
            strcat(string_compus, " ");
            strcat(string_compus, p);
        }

        //incepe citirea stringului compus
        if(p[0] == '\"')
        {
            sa_compus = 0;
            strcpy(string_compus, p + 1);
        }

        else
            //daca s-a terminat citirea cuvantului compus, verifica daca
            //trebuie exclus sau nu cuvantul respectiv
            if(sa_compus == 1)
            {
                if(p[0] == '-')
                strcpy(exclude, p + 1);
                else
                {
                    t++;
                    strcpy(a[t], p);
                }
            }

        if(p[strlen(p) - 1] == '\"')
        {
            sa_compus = 1;
            string_compus[strlen(string_compus) - 1] = '\0';
            t++;
            strcpy(a[t], string_compus);
        }

        p=strtok(NULL," ");
    }

    //h este numarul de site-uri care respecta conditia cautarii
    int h=-1;
    //caracterele de dinainte si dupa stringul gasit
    char inainte, dupa;
    char *sir;
    //vectorul in care pastram site-urile care respecta conditia cautarii
    sites v2[z];
    for(i = 0; i <= z; i++)
        for(j = 0; j <= t; j++)
        {
            //daca paragraful are stil
            if(strstr(v[i].cod, ";\">") != 0)
                sir = strstr(v[i].cod, ";\">") + 3;
            else
                sir = strstr(v[i].cod, "<p>") + 3;
            //daca exista un cuvant care trebuie exclus si se afla in cod
            if(strcmp(exclude, "") != 0 && strstr(sir, exclude) != 0)
            {
                inainte = (strstr(sir, exclude) - 1)[0];
                dupa = (strstr(sir, exclude) + strlen(exclude))[0];

                if((inainte==' ' || inainte=='>' || inainte=='\n') && (dupa==' ' || dupa=='<' || dupa=='\n'))
                     break;
            }
            //daca exista un cuvant compus si se afla in sir
            if(strcmp(string_compus,"") != 0 && strstr(v[i].cod, string_compus) != 0)
            {
                h++;
                v2[h] = v[i];
                break;
            }
            //daca gaseste stringul in cod, trebuie sa verificam daca este cuvant
            if(strstr(sir, a[j]) != 0)
            {
                inainte = (strstr(sir, a[j]) - 1)[0];
                dupa = (strstr(sir, a[j]) + strlen(a[j]))[0];

                if((inainte==' ' || inainte=='>' || inainte=='\n') && (dupa==' ' || dupa=='<' || dupa=='\n'))
                {
                    h++;
                    v2[h] = v[i];
                    break;
                }
            }
        }

    //aplicam sortarea pe vectorul cu site-urile ce respecta cautarea
    sort(v2,h,cmps);
    for(i = 0; i <= h ; i++)
        printf("%s\n", v2[i].URL);

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
