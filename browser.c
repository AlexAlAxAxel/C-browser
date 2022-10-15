#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "program.h"
#include <ncurses.h>

//functia particulara de sortare pentru task-ul 2
int cmp_task2(sites a, sites b)
{
    //in sir1 si sir2 extragem codul, incepand cu continutul, pentru a le
    //putea compara conform cerintei
    char *sir1, *sir2;

    //daca exista stil pentru scris in cod
    if(strstr(a.cod, ";\">") != 0)
        sir1=strstr(a.cod, ";\">") + 3;
    else
        sir1=strstr(a.cod, "<p>") + 3;

    if(strstr(b.cod, ";\">")!=0)
        sir2=strstr(b.cod, ";\">") + 3;
    else
        sir2=strstr(b.cod, "<p>") + 3;

    //ordonare crescatooare dupa continut, iar in caz de egalitate, descrescatoare
    //dupa numarul de accesari
    if(strcmp(sir1, sir2) > 0 || (strcmp(sir1, sir2) == 0 && a.nr_acces < b.nr_acces))
        return -1;
    else
        return 1;    
}

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
        v[z].sir = (char*)malloc((v[z].lungime + 1) * sizeof(char));

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

    initscr();

    int width, row,col;
    getmaxyx(stdscr, row, col);
    start_color();
    char mesg[] = "Browserul lui Biju";
    refresh();
    noecho();
    keypad(stdscr, TRUE);
    
    int ch, nr_chr, cuvant_gasit, nr_site, site_precedent;
    char cautare[100], cautare_copie[100];
    WINDOW *rez_s, *rez_a, *rez_s_pag, *rez_a_pag;

    int (*cmps[2])(sites, sites) = {cmp_task2, cmp_task3};

    prima_pagina:
    mvprintw(1, (col-strlen(mesg))/2, "%s", mesg);
    
    //legenda
    mvprintw(row - 6, 1 , "%s", "Apasati C pentru a incepe o cautare noua");
    mvprintw(row - 2, 1 , "%s", "Apasati Q pentru a inchide programul");
    curs_set(0);
    while(1)
    {
        ch = getch();
    
        if(ch == 'c')
        {
            cautare:
            //legenda
            mvprintw(row - 6, 1 , "%s", "                                        ");
            mvprintw(row - 5, 1 , "%s", "Apasati Enter cand ati terminat de tastat");
            mvprintw(row - 4, 1 , "%s", "                                 ");
            mvprintw(row - 3, 1 , "%s", "                                  ");
            mvprintw(row - 2, 1 , "%s", "                                    ");

            curs_set(1);
            strcpy(cautare, "");
            cuvant_gasit = 0;
            
            //construieste search bar-ul
            for(i = col / 10; i <= col / 10 * 9; i++)
            {
                mvprintw(row / 2,i, "%c", '-');
                mvprintw(row / 2 + 2, i, "%c", '-');
            }
            mvprintw(row / 2 + 1, col / 10, "%c", '|');
            mvprintw(row / 2 + 1, col / 10 * 9, "%c", '|');

            //goleste eventualul continut afisat
            for(i = col / 10 + 1; i <= col / 10 * 9 - 1; i++)
                mvprintw(row / 2 + 1, i, "%c", ' ');

            move(row / 2 + 1, col / 10 + 1);
            echo();
            refresh();

            //nr_chr e numarul de caractere cautate - 1
            nr_chr = -1;

            while(1)
            {
                ch = getch();
                if(ch != '\n')
                {
                    nr_chr++;
                    cautare[nr_chr] = ch;
                }
                else
                    if(ch == '\n')
                    {
                        //legenda
                        mvprintw(row - 6, 1 , "%s", "Apasati C pentru a incepe o cautare noua");
                        mvprintw(row - 5, 1 , "%s", "                                         ");
                        mvprintw(row - 4, 1 , "%s", "Apasati S pentru o cautare simpla");
                        mvprintw(row - 3, 1 , "%s", "Apasati A pentru o cautare avansata");
                        mvprintw(row - 2, 1 , "%s", "Apasati Q pentru a inchide programul");

                        nr_chr++;
                        cautare[nr_chr] = '\0';
                        strcpy(cautare_copie, cautare);
                        cuvant_gasit = 1;
                        //dezactiveaza cursorul
                        curs_set(0);
                        break;
                    }
            }

            if(cuvant_gasit == 1)
            {
                noecho();
                while(1)
                {
                    ch = getch();

                    //inchide programul
                    if(ch == 'q')
                        goto inchide_programul;

                    //cautare simpla
                    if (ch == 's')
                    {
                        //taskul 2

                        //t este numarul de cuvinte din search bar - 1
                        int t = -1; 
                        //a este vectorul de cuvinte din search bar
                        char *p, a[100][100]; 
                        p = strtok(cautare, " ");
                        while (p != NULL)
                        {
                            t++;
                            strcpy(a[t], p);
                            p = strtok(NULL, " ");
                        }

                        int h = -1;
                        //caracterele de dinainte si dupa cuvantul cautat
                        char inainte, dupa;
                        char *sir;
                        sites v2[z];
                        for (i = 0; i <= z; i++)
                            for (j = 0; j <= t; j++)
                            {
                                //daca paragraful are stil
                                if (strstr(v[i].cod, ";\">") != 0)
                                    sir = strstr(v[i].cod, ";\">")+2;
                                else
                                    sir = strstr(v[i].cod, "<p>")+2;

                                strcpy(v[i].sir, sir);
                                v[i].sir[strlen(v[i].sir) - 12] = '\0';

                                if (strstr(sir, a[j]) != 0)
                                {
                                    inainte = (strstr(sir, a[j]) - 1)[0];
                                    dupa = (strstr(sir, a[j]) + strlen(a[j]))[0];

                                    if ((inainte == ' ' || inainte == '>' || inainte == '\n') && (dupa == ' ' || dupa == '<' || dupa == '\n'))
                                    {
                                        h++;
                                        v2[h] = v[i];
                                        break;
                                    }
                                }
                            }
                        sort(v2, h, cmps[0]);

                        //fereastra rezultatelor prin cautare simpla
                        rez_s = newwin(row,col,0,0);
                        refresh();

                        rezultate_simple:
                        //legenda
                        mvprintw(row - 4, 1 , "%s", "Folositi tastele Key_Up si Key_Down pentru a schimba site-urile");
                        mvprintw(row - 3, 1 , "%s", "Apasati Enter pentru a selecta site-ul curent");
                        mvprintw(row - 2, 1 , "%s", "Apasati B pentru a reveni la pagina de cautare");

                        //construim search bar-ul sus
                        for(i = col / 10; i <= col / 10 * 9; i++)
                        {
                            mvprintw(1, i, "%c", '-');
                            mvprintw(3, i, "%c", '-');
                        }
                        mvprintw(2, col / 10, "%c", '|');
                        mvprintw(2, col / 10 * 9,"%c", '|');
                        mvprintw(2, col / 10 + 1, "%s", cautare_copie);

                        attron(A_STANDOUT);
                        if(h >= 0)
                            mvprintw(row / 5, 1, "%s   %s", v2[0].URL, v2[0].titlu);
                        else
                            mvprintw(row / 2, (col - 27) / 2, "%s", "Nu au fost gasite rezultate");
                        attroff(A_STANDOUT);
                        for(i = 1; i <= h; i++)
                            mvprintw(row / 5 + i * 2, 1, "%s   %s", v2[i].URL, v2[i].titlu);
                        curs_set(0);
                        wrefresh(rez_s);
                        nr_site = 0;
                        
                        while(1)
                        {
                            ch = getch();

                            if(ch == 'b')
                            {
                                redrawwin(rez_s);
                                wrefresh(rez_s);
                                goto prima_pagina;
                            }

                            if(h >= 0)
                            {
                                if(ch == KEY_DOWN)
                                {
                                    site_precedent = nr_site;
                                    nr_site++;
                                    if(nr_site > h)
                                        nr_site = 0;
                                    attron(A_STANDOUT);
                                    mvprintw(row / 5 + nr_site * 2, 1, "%s   %s", v2[nr_site].URL, v2[nr_site].titlu);
                                    attroff(A_STANDOUT);
                                    mvprintw(row / 5 + site_precedent * 2, 1, "%s   %s", v2[site_precedent].URL, v2[site_precedent].titlu);
                                }

                                if(ch == KEY_UP)
                                {
                                    site_precedent = nr_site;
                                    nr_site--;
                                    if(nr_site < 0)
                                        nr_site = h;
                                    attron(A_STANDOUT);
                                    mvprintw(row / 5 + nr_site * 2, 1, "%s   %s", v2[nr_site].URL, v2[nr_site].titlu);
                                    attroff(A_STANDOUT);
                                    mvprintw(row / 5 + site_precedent * 2, 1, "%s   %s", v2[site_precedent].URL, v2[site_precedent].titlu);
                                }

                                if(ch == '\n')
                                {
                                    rez_s_pag = newwin(row, col, 0, 0);
                                    refresh();
                                    attron(A_BOLD);
                                    mvprintw(1, (col-strlen(v2[nr_site].titlu)) / 2 , "%s", v2[nr_site].titlu);
                                    attroff(A_BOLD);
                                    mvprintw(row-2, 1 , "%s", "Apasati B pentru a reveni la pagina rezultatelor");
                                    
                                    //setam culoarea
                                    init_pair(1, v2[nr_site].font_color, v2[nr_site].bg_color);
                                    wattron(rez_s_pag, COLOR_PAIR(1));
                                    mvwprintw(rez_s_pag,row / 5, 0, "%s", v2[nr_site].sir + 1);
                                    wrefresh(rez_s_pag);
                                    break;
                                }
                            }
                        }

                        while(1)
                        {
                            ch = getch();

                            if(ch == 'b')
                            {
                                redrawwin(rez_s);
                                wrefresh(rez_s);
                                goto rezultate_simple;
                            }
                                
                        }
                    }
                    

                    //cautare avansata
                    if (ch == 'a')
                    {
                        //taskul 3

                        //t2 este numarul de cuvinte din search bar - 1
                        int t2 = -1; 
                        //a2 este vectorul de cuvinte citite din search bar
                        char *p2, a2[100][100], exclude[100] = ""; 
                        int sa_compus = 1;
                        char string_compus[100] = "";
                        p2 = strtok(cautare, " ");
                        while (p2 != NULL)
                        {
                            if (sa_compus == 0)
                            {
                                strcat(string_compus, " ");
                                strcat(string_compus, p2);
                            }

                            if (p2[0] == '\"')
                            {
                                sa_compus = 0;
                                strcpy(string_compus, p2 + 1);
                            }
                            else
                                if (sa_compus == 1)
                                {
                                    if (p2[0] == '-')
                                        strcpy(exclude, p2 + 1);
                                    else
                                    {
                                        t2++;
                                        strcpy(a2[t2], p2);
                                    }
                                }

                            if (p2[strlen(p2) - 1] == '\"')
                            {
                                sa_compus = 1;
                                string_compus[strlen(string_compus) - 1] = '\0';
                                t2++;
                                strcpy(a2[t2], string_compus);
                            }

                            p2 = strtok(NULL, " ");
                        }

                        int h2 = -1;
                        char inainte2, dupa2, *sir2; //caracterele de dinainte si dupa cuvantul cautat
                        sites v3[z];
                        for (i = 0; i <= z; i++)
                            for (j = 0; j <= t2; j++)
                            {
                                //daca paragraful are stil
                                if (strstr(v[i].cod, ";\">") != 0)
                                    sir2 = strstr(v[i].cod, ";\">") + 2;
                                else
                                    sir2 = strstr(v[i].cod, "<p>") + 2;

                                strcpy(v[i].sir, sir2);
                                v[i].sir[strlen(v[i].sir) - 12] = '\0';

                                if (strcmp(exclude, "") != 0)
                                {
                                    if (strstr(sir2, exclude) != 0)
                                    {
                                        inainte2 = (strstr(sir2, exclude) - 1)[0];
                                        dupa2 = (strstr(sir2, exclude) + strlen(exclude))[0];

                                        if ((inainte2 == ' ' || inainte2 == '>' || inainte2 == '\n') && (dupa2 == ' ' || dupa2 == '<' || dupa2 == '\n'))
                                            break;
                                    }
                                }

                                if (strcmp(string_compus, "") != 0)
                                    if (strstr(v[i].cod, string_compus) != 0)
                                    {
                                        h2++;
                                        v3[h2] = v[i];
                                        break;
                                    }

                                if (strstr(sir2, a2[j]) != 0)
                                {
                                    inainte2 = (strstr(sir2, a2[j]) - 1)[0];
                                    dupa2 = (strstr(sir2, a2[j]) + strlen(a2[j]))[0];

                                    if ((inainte2 == ' ' || inainte2 == '>' || inainte2 == '\n') && (dupa2 == ' ' || dupa2 == '<' || dupa2 == '\n'))
                                    {
                                        h2++;
                                        v3[h2] = v[i];
                                        break;
                                    }
                                }
                            }

                        sort(v3, h2, cmps[1]);

                        //fereastra rezultatelor prin cautare avansata
                        rez_a = newwin(row,col,0,0);
                        refresh();

                        rezultate_avansate:
                        //legenda
                        mvprintw(row - 4, 1 , "%s", "Folositi tastele Key_Up si Key_Down pentru a schimba site-urile");
                        mvprintw(row - 3, 1 , "%s", "Apasati Enter pentru a selecta site-ul curent");
                        mvprintw(row - 2, 1 , "%s", "Apasati B pentru a reveni la pagina de cautare");

                        //construim search bar-ul sus
                        for(i = col / 10; i <= col / 10 * 9; i++)
                        {
                            mvprintw(1, i, "%c", '-');
                            mvprintw(3, i, "%c", '-');
                        }
                        mvprintw(2, col / 10, "%c", '|');
                        mvprintw(2, col / 10 * 9, "%c", '|');
                        mvprintw(2, col / 10 + 1, "%s", cautare_copie);

                        attron(A_STANDOUT);
                        if(h2 >= 0)
                            mvprintw(row / 5, 1, "%s   %s", v3[0].URL, v3[0].titlu);
                        else
                            mvprintw(row / 2, (col - 27) / 2, "%s", "Nu au fost gasite rezultate");
                        attroff(A_STANDOUT);
                        for(i = 1; i <= h2; i++)
                            mvprintw(row / 5 + i * 2, 1, "%s   %s", v3[i].URL, v3[i].titlu);
                        curs_set(0);
                        wrefresh(rez_a);
                        nr_site = 0;
                        
                        while(1)
                        {
                            ch = getch();

                            if(ch == 'b')
                            {
                                redrawwin(rez_a);
                                wrefresh(rez_a);
                                goto prima_pagina;
                            }

                            if(h2 >= 0)
                            {
                                if(ch == KEY_DOWN)
                                {
                                    site_precedent = nr_site;
                                    nr_site++;
                                    if(nr_site > h2)
                                        nr_site = 0;
                                    attron(A_STANDOUT);
                                    mvprintw(row / 5 + nr_site * 2, 1, "%s   %s", v3[nr_site].URL, v3[nr_site].titlu);
                                    attroff(A_STANDOUT);
                                    mvprintw(row / 5 + site_precedent * 2, 1, "%s   %s", v3[site_precedent].URL, v3[site_precedent].titlu);
                                }

                                if(ch == KEY_UP)
                                {
                                    site_precedent = nr_site;
                                    nr_site--;
                                    if(nr_site < 0)
                                        nr_site = h2;
                                    attron(A_STANDOUT);
                                    mvprintw(row / 5 + nr_site * 2, 1, "%s   %s", v3[nr_site].URL, v3[nr_site].titlu);
                                    attroff(A_STANDOUT);
                                    mvprintw(row / 5 + site_precedent * 2, 1, "%s   %s", v3[site_precedent].URL, v3[site_precedent].titlu);
                                }

                                if(ch == '\n')
                                {
                                    rez_a_pag = newwin(row,col,0,0);
                                    refresh();
                                    attron(A_BOLD);
                                    mvprintw(1, (col-strlen(v3[nr_site].titlu)) / 2 , "%s", v3[nr_site].titlu);
                                    attroff(A_BOLD);
                                    mvprintw(row-2, 1, "%s", "Apasati B pentru a reveni la pagina rezultatelor");
                                    
                                    //setam culoarea
                                    init_pair(1, v3[nr_site].font_color, v3[nr_site].bg_color);
                                    wattron(rez_a_pag,COLOR_PAIR(1));
                                    mvwprintw(rez_a_pag, row / 5, 0, "%s",v3[nr_site].sir + 1);
                                    wrefresh(rez_a_pag);
                                    break;
                                }
                            }
                        }

                        while(1)
                        {
                            ch = getch();

                            if(ch == 'b')
                            {
                                redrawwin(rez_a);
                                wrefresh(rez_a);
                                goto rezultate_avansate;
                            }
                        }


                    }

                    //cauta din nou
                    if(ch == 'c')
                        goto cautare;
                }
            }
        }

        if(ch == 'q')
        {
            inchide_programul:
            endwin();
            break;
        }    
    }

    //eliberam memoria
    for(i = 0; i <= z; i++)
        {
            free(v[i].cod);
            v[i].cod = NULL;
            free(v[i].sir);
            v[i].sir = NULL;
        }
    free(v);
    //inchidem fisierul initial
    fclose(f);
    return 0;
}