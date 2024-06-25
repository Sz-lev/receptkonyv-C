#include <stdio.h>

#include "kinezet.h"
#include "econio.h"

void fejlec()
{
    puts("\t     ____________________   ____________________     ");
    puts("\t    /|                   \\ /                    |\\   ");
    puts("\t .-/||                    |                     ||\\-.");
    puts("\t | |||                    |                     ||| |");
    puts("\t | |||      RECEPTES      |        KONYV        ||| |");
    puts("\t | |||                    |                     ||| |");
    puts("\t | |||                    |                     ||| |");
    puts("\t | |||                    |                     ||| |");
    puts("\t | |||                    |                     ||| |");
    puts("\t | |||                    |                     ||| |");
    puts("\t | |||                    |                     ||| |");
    puts("\t | |||                    |                     ||| |");
    puts("\t | ||--------------------\\|/---------------------|| |");
    puts("\t | /======================|=======================\\ |");
    puts("\t \\----------------------~___~-----------------------/");
    printf("\n\n");
}

void hiba_uzenet(char * szoveg)
{
    econio_textcolor(4);
    puts(szoveg);
    econio_textcolor(11);
}

void barna_sarga(char * szo)
{
    econio_textcolor(6);
    printf("%s\n", szo);
    econio_textcolor(14);
}


void barna_kek(char * szo)
{
    econio_textcolor(6);
    printf("%s\n", szo);
    econio_textcolor(11);
}


void sarga_kek(char * szo)
{
    econio_textcolor(14);
    printf("%s\n", szo);
    econio_textcolor(11);
}
