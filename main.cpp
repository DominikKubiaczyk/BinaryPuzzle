#include<stdio.h>
#include"conio2.h"
#include <time.h>
#include <stdlib.h>
using namespace std;

#define LEWO 2
#define PRAWO 46
#define ROZMIAR 12
#define STARTX PRAWO+1
#define STARTY 4
#define TAK 1
#define NIE 0

int zn = 0, x = STARTX, y = STARTY, attr = BLACK, back = CYAN, zero = 0;
char txt[32];
char pozycja[32];


typedef struct plansza //deklarujemy typ będący strukturą dla pola planszy
{
        int x;
        int y;
        char wartosc;
        int czy_modyfikowalna;
}; 

struct plansza t[ROZMIAR][ROZMIAR]; //deklarujemy dwuwymiarową tablicę struktur

int sprawdz1(struct plansza t[ROZMIAR][ROZMIAR], int x, int y, char value) //funkcja sprawdzajaca pierwszą regułę dla danego pola względem danej wartości
{
        if (x <= ROZMIAR - 3)
        {
                if (t[x + 1][y].wartosc == value && t[x + 2][y].wartosc == value) { return 0; }
        }
        if (x >= 1 && x < (ROZMIAR - 1))
        {
                if (t[x - 1][y].wartosc == value && t[x + 1][y].wartosc == value) { return 0; }
        }
        if (x >= 2)
        {
                if (t[x - 1][y].wartosc == value && t[x - 2][y].wartosc == value) { return 0; }
        }
        if (y <= ROZMIAR - 3)
        {
                if (t[x][y+1].wartosc == value && t[x][y+2].wartosc == value) { return 0; }
        }
        if (y >= 1 && y < (ROZMIAR - 1))
        {
                if (t[x][y-1].wartosc == value && t[x][y+1].wartosc == value) { return 0; }
        }
        if (y >= 2)
        {
                if (t[x][y-1].wartosc == value && t[x][y-2].wartosc == value) { return 0; }
        }

        return 1;
}

int sprawdz2(struct plansza t[ROZMIAR][ROZMIAR], int x, int y, char value) // funkcja sprawdzajaca drugą regułe dla danego pola względem danej wartości
{
        int wiersz=0, kolumna=0;
        for (int i = 0;i < ROZMIAR;i++)
        {
                if (t[i][y].wartosc == value) { kolumna += 1; }
        }
        for (int j = 0;j < ROZMIAR;j++)
        {
                if (t[x][j].wartosc == value) { wiersz += 1; }
        }
        if (kolumna < (ROZMIAR / 2) && wiersz < (ROZMIAR / 2)) { return 1; }
        else { return 0; }
}

void iloscizerijedynek(struct plansza t[ROZMIAR][ROZMIAR]) // funkcja sumująca ilość zer i jedynek w każdej kolumnie/wierszu
{
        
        for (int i = 0;i < ROZMIAR;i++)
        {
                int wiersz1 = 0, wiersz0 = 0, kolumna1 = 0, kolumna0 = 0;
                char wiersz[50], kolumna3[50], kolumna2[50];
                for (int j = 0;j < ROZMIAR;j++) //pętla zliczająca ilość zer i jednek
                {
                        if (t[i][j].wartosc == '1') {kolumna1 += 1 ; }
                        if (t[i][j].wartosc == '0') { kolumna0 += 1; }
                        if (t[j][i].wartosc == '1') { wiersz1 += 1; }
                        if (t[j][i].wartosc == '0') { wiersz0 += 1; }
                }
                gotoxy(PRAWO+14, i + 4);
                textcolor(MAGENTA);
                sprintf_s(wiersz, "0: %d, 1: %d", wiersz0, wiersz1);
                cputs(wiersz); //wypisywanie ilosci zer i jedynek przy odpowiednim wierszu

                gotoxy(i+PRAWO+1, 17);
                textcolor(MAGENTA);
                cputs("0");
                gotoxy(i +PRAWO+1, 18);
                cputs("|");
                gotoxy(i + PRAWO+1, 19);
                sprintf_s(kolumna3, "%d", kolumna0);
                cputs(kolumna3);
                gotoxy(i + PRAWO+1, 20);
                cputs("_");
                gotoxy(i + PRAWO+1,21);
                cputs("1");
                gotoxy(i + PRAWO+1, 22);
                cputs("|");
                gotoxy(i + PRAWO+1, 23);
                sprintf_s(kolumna2, "%d", kolumna1);
                cputs(kolumna2); // wypisywanie ilości zer i jedynek pod odpowiednimi kolumnami
                
        }
}

int sprawdz3wiersz(struct plansza t[ROZMIAR][ROZMIAR], int x, int y, char value) // funkcja sprawdzajaca trzecią regułę względem wierszy
{
        for (int i = 0;i < ROZMIAR;i++)
        {
                if (i != x)
                {
                        if (t[i][y].wartosc == '.') { return 1; }
                }
        }

        for (int w = 0;w < ROZMIAR;w++)
        {
                        for (int k = 0;k < ROZMIAR;k++)
                        {
                                if (k != x && w != y)
                                {
                                        if (t[k][w].wartosc != t[k][y].wartosc) { return 1; }
                                }
                        }
                        if (t[x][w].wartosc == value) { return 0; }
        }
        return 1;
}

int sprawdz3kolumna(struct plansza t[ROZMIAR][ROZMIAR], int x, int y, char value) //funkcja sprawdzająca trzecią regułę względem kolumn
{
        for (int i = 0;i < ROZMIAR;i++)
        {
                if (i != y)
                {
                        if (t[x][i].wartosc == '.') { return 1; }
                }
        }

        for (int w = 0;w < ROZMIAR;w++)
        {
                for (int k = 0;k < ROZMIAR;k++)
                {
                        if (w != x && k != y)
                        {
                                if (t[w][k].wartosc != t[x][k].wartosc) { return 1; }
                        }
                }
                if (t[w][y].wartosc == value) { return 0; }
        }
        return 1;
}

void nowa(struct plansza t[ROZMIAR][ROZMIAR]) //funkcja tworząca domyślną nową planszę startową
{
        for (int i = 0;i < ROZMIAR;i++)
        {
                for (int j = 0; j < ROZMIAR; j++)
                {
                        t[i][j].x = i + 56;
                        t[i][j].y = j + 4;
                        t[i][j].wartosc = '.';
                        t[i][j].czy_modyfikowalna = TAK;
                }
        }
        
        t[3][0].wartosc = '1'; t[3][0].czy_modyfikowalna = NIE;
        t[11][0].wartosc = '1'; t[11][0].czy_modyfikowalna = NIE;
        t[11][3].wartosc = '0';t[11][3].czy_modyfikowalna = NIE;
        t[11][7].wartosc = '0';t[11][7].czy_modyfikowalna = NIE;
        t[11][10].wartosc = '0';t[11][10].czy_modyfikowalna = NIE;
        t[0][4].wartosc = '1';t[0][4].czy_modyfikowalna = NIE;
        t[0][7].wartosc = '0';t[0][7].czy_modyfikowalna = NIE;
        t[1][1].wartosc = '0';t[1][1].czy_modyfikowalna = NIE;
        t[1][3].wartosc = '0';t[1][3].czy_modyfikowalna = NIE;
        t[1][8].wartosc = '1';t[1][8].czy_modyfikowalna = NIE;
        t[2][4].wartosc = '1';t[2][4].czy_modyfikowalna = NIE;
        t[2][6].wartosc = '1';t[2][6].czy_modyfikowalna = NIE;
        t[2][9].wartosc = '0';t[2][9].czy_modyfikowalna = NIE;
        t[2][10].wartosc = '0';t[2][10].czy_modyfikowalna = NIE;
        t[3][3].wartosc = '1';t[3][3].czy_modyfikowalna = NIE;
        t[3][6].wartosc = '1';t[3][6].czy_modyfikowalna = NIE;
        t[3][11].wartosc = '1';t[3][11].czy_modyfikowalna = NIE;
        t[4][8].wartosc = '1';t[4][8].czy_modyfikowalna = NIE;
        t[5][2].wartosc = '0';t[5][2].czy_modyfikowalna = NIE;
        t[5][3].wartosc = '0';t[5][3].czy_modyfikowalna = NIE;
        t[5][5].wartosc = '0';t[5][5].czy_modyfikowalna = NIE;
        t[5][9].wartosc = '0';t[5][9].czy_modyfikowalna = NIE;
        t[5][10].wartosc = '0';t[5][10].czy_modyfikowalna = NIE;
        t[6][7].wartosc = '0';t[6][7].czy_modyfikowalna = NIE;
        t[6][10].wartosc = '0';t[6][10].czy_modyfikowalna = NIE;
        t[6][11].wartosc = '0';t[6][11].czy_modyfikowalna = NIE;
        t[7][3].wartosc = '0';t[7][3].czy_modyfikowalna = NIE;
        t[7][4].wartosc = '0';t[7][4].czy_modyfikowalna = NIE;
        t[7][9].wartosc = '0';t[7][9].czy_modyfikowalna = NIE;
        t[8][1].wartosc = '1';t[8][1].czy_modyfikowalna = NIE;
        t[8][5].wartosc = '1';t[8][5].czy_modyfikowalna = NIE;
        t[8][6].wartosc = '1';t[8][6].czy_modyfikowalna = NIE;
        t[9][3].wartosc = '0';t[9][3].czy_modyfikowalna = NIE;
        t[9][11].wartosc = '1';t[9][11].czy_modyfikowalna = NIE;
        t[10][4].wartosc = '1';t[10][4].czy_modyfikowalna = NIE;
        t[10][8].wartosc = '1';t[10][8].czy_modyfikowalna = NIE;
}

void drukuj_nowa(struct plansza t[ROZMIAR][ROZMIAR]) //funkcja wypisująca na ekran planszę
{
        for (int i = 0;i <= (ROZMIAR + 1);i++)
        {
                for (int j = 0;j <= (ROZMIAR + 1);j++)
                {
                        if (i == 0 || i == (ROZMIAR + 1))
                        {
                                gotoxy(i + PRAWO, j + 3);
                                textcolor(attr);
                                textbackground(back);
                                putch('|');
                        }
                        else if (j == 0 || j == (ROZMIAR + 1))
                        {
                                gotoxy(i + PRAWO, j + 3);
                                textcolor(attr);
                                textbackground(back);
                                putch('-');
                        }
                        else
                        {
                                gotoxy(i + PRAWO, j + 3);
                                textcolor(attr);
                                textbackground(back);
                                putch(t[i - 1][j - 1].wartosc);
                        }
                }
        }
}

void legenda(void) // funkcja wypisujaca legendę na ekran
{
        gotoxy(LEWO, 1);
        cputs("PUZZLE BINARNE");
        gotoxy(LEWO, 3);
        // wyświetlamy na ekranie, w miejscu kursora napis kursor przesuwa się w prawo o długość napisu
        cputs("STRZALKI = poruszanie po planszy");
        gotoxy(LEWO, 4);
        cputs("ESC = wyjscie z programu");
        gotoxy(LEWO, 5);
        cputs("N = nowa gra");
        gotoxy(LEWO, 6);
        cputs("0 1 = wpisywanie cyfr na plansze");
        gotoxy(LEWO, 7);
        cputs("O = losowe wypelnianie planszy");
        gotoxy(LEWO, 8);
        cputs("P = prosta podpowiedz:");
        gotoxy(LEWO, 11);
        cputs("D = ilosc 0 i 1 w w. i k.");
}

void zmienne_rzeczy(void) //funkcja wypisująca pod legendą zmienne rzeczy jak pozycja kursora, oraz wypisująca imię i nazwisko autora
{
        int zero = 0;
        if (zero) sprintf_s(txt, "kod klawisza: 0x00 0x%02x", zn);
        else sprintf_s(txt, "kod klawisza: 0x%02x", zn);
        gotoxy(LEWO, 15);
        textcolor(BLACK);
        cputs(txt);
        gotoxy(LEWO, 18);
        textcolor(BLACK);
        cputs("Dominik Kubiaczyk 171739");
        gotoxy(LEWO, 20);
        cputs("Wykonane: a,b,c,d,e,f,g,h,k;");
        sprintf_s(pozycja, "Twoja pozycja: (%2d , %2d)", y - 3, x - PRAWO);
        gotoxy(LEWO, 22);
        textcolor(BLACK);
        cputs(pozycja);
}

void losowo(struct plansza t[ROZMIAR][ROZMIAR]) // funkcja tworząca losowo wypełnioną planszę
{
        for (int i = 0;i < ROZMIAR;i++)
        {
                for (int j = 0; j < ROZMIAR; j++)
                {
                        t[i][j].x = i + 56;
                        t[i][j].y = j + 4;
                        t[i][j].wartosc = '.';
                        t[i][j].czy_modyfikowalna = TAK;
                }
        }

        srand(time(NULL));
        int ilosc = (rand() % 100)+1;

        for (int i = 0;i < ilosc;i++)
        {
                int kolumna = (rand() % ROZMIAR) + 1;
                int wiersz = (rand() % ROZMIAR) + 1;
                int value = (rand() % 2);

                if (t[kolumna][wiersz].czy_modyfikowalna == TAK)
                {
                        if (value == 1)
                        {
                                if(sprawdz1(t,kolumna,wiersz,'1')==1 && sprawdz2(t,kolumna,wiersz,'1')==1 && sprawdz3kolumna(t,kolumna,wiersz,'1')==1 && sprawdz3wiersz(t,kolumna,wiersz,'1')==1)
                                {
                                        t[kolumna][wiersz].wartosc = '1';t[kolumna][wiersz].czy_modyfikowalna = NIE;
                                }
                        }
                        if (value == 0)
                        {
                                if (sprawdz1(t, kolumna, wiersz, '0') == 1 && sprawdz2(t, kolumna, wiersz, '0') == 1 && sprawdz3kolumna(t, kolumna, wiersz, '0') == 1 && sprawdz3wiersz(t, kolumna, wiersz, '0') == 1)
                                {
                                        t[kolumna][wiersz].wartosc = '0';t[kolumna][wiersz].czy_modyfikowalna = NIE;
                                }
                        }
                }
        }
        drukuj_nowa(t);
}

void prostapodpowiedz(struct plansza t[ROZMIAR][ROZMIAR], int x, int y) // funkcja wypisująca podpowiedź: co można wpisać w pole na którym znajduje sie kursor
{
        if (t[x][y].czy_modyfikowalna == NIE)
        {
                gotoxy(LEWO, 9);
                textcolor(MAGENTA);
                cputs("Nie modyfikowalne                ");
        }
        else if (sprawdz1(t, x, y, '1') == 1 && sprawdz1(t, x, y, '0') == 1 && sprawdz2(t, x, y, '1') == 1 && sprawdz2(t, x, y, '0') == 1 && sprawdz3kolumna(t, x, y, '1') == 1 && sprawdz3wiersz(t, x, y, '1') == 1 && sprawdz3kolumna(t, x, y, '0') == 1 && sprawdz3wiersz(t, x, y, '0') == 1)
        {
                gotoxy(LEWO,9);
                textcolor(MAGENTA);
                cputs("Mozna wpisac 0 i 1          ");
        }
        else
        {
                if (sprawdz1(t, x, y, '1') == 1 && sprawdz2(t, x, y, '1') == 1 && sprawdz3kolumna(t, x, y, '1') == 1 && sprawdz3wiersz(t, x, y, '1') == 1)
                {
                        gotoxy(LEWO,9);
                        textcolor(MAGENTA);
                        cputs("Mozna wpisac 1          ");
                }
                else if (sprawdz1(t, x, y, '0') == 1 && sprawdz2(t, x, y, '0') == 1 && sprawdz3kolumna(t, x, y, '0') == 1 && sprawdz3wiersz(t, x, y, '0') == 1)
                {
                        gotoxy(LEWO, 9);
                        textcolor(MAGENTA);
                        cputs("Mozna wpisac 0          ");
                }
                else
                {
                        gotoxy(LEWO, 9);
                        textcolor(MAGENTA);
                        cputs("Nie mozna juz wpisac nic");
                }
        }
}

void dzialanie(void)
{
        int a = x - PRAWO-1;
        int b = y - 4; //tworzymy dwie pomocnicze zmienne

        zero = 0;
        zn = getch(); // nie chcemy, aby klawisz 'H' działał jako strzałka w górę więc sprawdzamy, czy pierwszym znakiem było zero
        if (zn == 0) {
                zero = 1;               // jeżeli tak, to czytamy kolejny znak wiedząc już, że będzie to znak specjalny
                zn = getch();           
                if (zn == 0x48) {
                        y--; if (y < 4) { y = 15; } //gdy kursor dojedzie do konca planszy to wraca na drugą jej stronę
                }       
                else if (zn == 0x50) {
                        y++; if (y > 15) {
                                y = 4;
                        }
                }
                else if (zn == 0x4b) {
                        x--; if (x < PRAWO+1) {
                                x = PRAWO+12;
                        }
                }
                else if (zn == 0x4d) {
                        x++; if (x > PRAWO+12) {
                                x = PRAWO+1;
                        }
                }
        }

        else if (zn == '1' && t[a][b].czy_modyfikowalna == TAK  && sprawdz2(t, a, b, '1') && sprawdz1(t,a,b,'1') && sprawdz3wiersz(t,a,b,'1') && sprawdz3kolumna(t,a,b,'1')) {
                gotoxy(x, y);textcolor(YELLOW);cputs("1"); t[a][b].wartosc = '1'; // sprawdzamy wszystkie reguły i jeśli są spełnione to wpisujemy w pole '1', a jeśli nie to nie pozwalamy wpisać '1'
        }
        else if (zn == '0' && t[a][b].czy_modyfikowalna == TAK  && sprawdz2(t,a,b,'0') && sprawdz1(t,a,b,'0') && sprawdz3wiersz(t,a,b,'0') && sprawdz3kolumna(t,a,b,'0')) {
                gotoxy(x, y);textcolor(YELLOW);cputs("0"); t[a][b].wartosc = '0'; // analogicznie jak wyżej tylko dla '0'
        }
        else if(zn=='n') //jeśli zostanie wciśnięty klawisz 'n' to rozpoczynamy nową grę, czyli tworzymy i wypisujemy nową domyślną planszę
        {
                nowa(t);
                drukuj_nowa(t);
        }
        else if (zn == 'o') // jeśli zostanie wciśnięty klawisz 'o' to tworzymy nową planszę losowo wypełnioną
        {
                losowo(t);
        }
        else if (zn == 'd') // jeśli zostanie wciśnięty klawisz 'd' to zostaje wypisana ilość zer i jedynek w kolumnach i wierszach
        {
                iloscizerijedynek(t);
        }
        else if (zn == 'p')
        {
                prostapodpowiedz(t, a, b); // jeśli zostanie wciśnięty klawisz 'p' to ukazuje się podpowiedź - co można wpisać w dane pole
        }
}

int main()
{
        settitle("Dominik Kubiaczyk 171739");//tytuł okienka ustawiamy na imię, nazwisko i nr indeksu autora
        nowa(t);//tworzymy nową planszę
        textbackground(CYAN);//ustawianie koloru
        clrscr();// czyścimy ekran: wypełniamy spacjami z ustawionym kolorem tła
        textcolor(BLACK);//ustawianie koloru czcionki                    
        legenda();//wypisujemy legendę
        drukuj_nowa(t);//drukujemy planszę

        do {                     //pętla wykonująca odpowiednie funkje do momentu aż zostanie wciśnięty klawisz 'esc'
                zmienne_rzeczy();
                gotoxy(x, y);
                textcolor(attr);
                textbackground(back);
                dzialanie();            
        }while (zn != 0x1b); //klawisz 'esc' kończy grę

        return 0;
}