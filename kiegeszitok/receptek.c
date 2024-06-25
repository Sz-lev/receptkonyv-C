#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "osszetevok.h"
#include "menu.h"
#include "adat_kezeles.h"
#include "uj_recept.h"
#include "debugmalloc.h"
#include "receptek.h"
#include "kinezet.h"
#include "econio.h"


void recept_menu(Listak* adatok)
{
    int hibakod = 0;
    int valasztott = 0;

    while (!adatok->fomenu)
    {
        econio_clrscr();

        rec_menu_listazas();

        if (hibakod != 0)
            hibakod = rec_menu_hibauzenet(hibakod);

        valasztott = menu_valaszto();

        switch(valasztott)
        {
        case 1:
            if (adatok->elso_rec != NULL)
            {
                adatok->kezdobetu = "0";
                rec_lista_nezet(adatok);
            }

            else
                hibakod = 2;
            break;

        case 2:
            if (adatok->elso_rec != NULL)
                {
                    adatok->kezdobetu = kezdobetu_beker();

                    rec_lista_nezet(adatok);

                    free(adatok->kezdobetu);
                    adatok->kezdobetu = NULL;
                }
            else
                hibakod = 2;
            break;

        case 3:
            if (adatok->elso_rec != NULL)
                {
                    adatok->kezdobetu = keresett_szo_beker();

                    rec_lista_nezet(adatok);

                    free(adatok->kezdobetu);
                    adatok->kezdobetu = NULL;
                }
            else
                hibakod = 2;
            break;

        case 4:
            ujrec_adat_bekeres(adatok);
            break;

        case 5:
            adatok->fomenu = true;
            break;

        default:
            hibakod = 1;
            break;

        }

    }
}


void rec_menu_listazas()
{
    econio_textcolor(14);
    printf("Receptek menuje\n\n");
    econio_textcolor(11);

    printf("[1] Receptek ABC sorrendben\n");
    printf("[2] Receptek listazasa kezdobetu alapjan\n");
    printf("[3] Receptek listazasa szo/szoreszlet alapjan\n");
    printf("[4] Uj Receptek hozzaadasa\n");
    printf("[5] Visszalepes a fomenube\n");
    printf("\n");
}


int rec_menu_hibauzenet(int hibakod)
{
    switch(hibakod)
    {
    case 1:
        hiba_uzenet("Ervenytelen erteket adott meg!");
        break;
    case 2:
        hiba_uzenet("Nincs megjelenitheto recept!");
        break;
    case 3:
        hiba_uzenet("Nincs kovetkezo oldal!");
        break;
    case 4:
        hiba_uzenet("Nincs visszafele tovabbi oldal!");
        break;
    }
    return 0;
}


void rec_lista_nezet(Listak* adatok)
{
    int oldalszam = 1;
    bool vissza = false;
    char valaszto;
    bool vege = false;
    int hiba_kodja = 0;
    int sorszam = 0;

    while (!vissza && !(adatok->fomenu) && adatok->elso_rec != NULL)
    {
        econio_clrscr();

        if (adatok->kezdobetu[0] == '0' && adatok->kezdobetu[1] == '\0')
            vege = rec_abc_listazas(oldalszam, adatok);

        else if (adatok->kezdobetu[1] == '\0' && ((adatok->kezdobetu[0] >= 'a' && adatok->kezdobetu[0] <= 'z') || (adatok->kezdobetu[0] >= 'A' && adatok->kezdobetu[0] <= 'Z')))
            vege = rec_kezdobetu_listazas(oldalszam, adatok);

        else if (strlen(adatok->kezdobetu) >= 2)
            vege = rec_szoreszlet_listazas(oldalszam, adatok);


        rec_lista_menu();

        printf("\n");

        if (hiba_kodja != 0)
            hiba_kodja = rec_menu_hibauzenet(hiba_kodja);

        valaszto = rec_lista_valaszto(&sorszam);

            switch (valaszto)
            {
            case 'e':
                if (!vege)
                    oldalszam += 1;
                else
                    hiba_kodja = 3;
                break;

            case 'h':
                if (oldalszam > 1)
                    oldalszam -= 1;
                else
                    hiba_kodja = 4;
                break;

            case 'u':
                ujrec_adat_bekeres(adatok);
                break;

            case 'v':
                vissza = true;
                break;

            case 'f':
                adatok->fomenu = true;
                break;

            case '0':
                if (sorszam > (oldalszam-1)*10 && sorszam <= oldalszam*10 && sorszam <= adatok->rec_max)
                    recept_adatok(adatok, sorszam);
                else
                    hiba_kodja = 1;
                break;

            default:
                hiba_kodja = 1;
                break;
            }
    }
}


bool rec_abc_listazas(int oldalszam, Listak* adatok)
{
    int szamlalo = 1;
    ReceptLista * rec_lepteto = adatok->elso_rec;

    econio_textcolor(14);
    printf("Receptek %d. oldal\n\n", oldalszam);
    econio_textcolor(11);

    while (rec_lepteto != NULL && szamlalo <= oldalszam*10)
    {
        if (szamlalo > (oldalszam-1)*10)
        {
            printf("[%d] %s\n", szamlalo, rec_lepteto->rec_nev);
        }

        rec_lepteto = rec_lepteto->kov_rec;
        szamlalo += 1;
    }
    printf("\n");

    adatok->rec_max = szamlalo-1;

    // A NULL ertek jelzi hogy eljutottunk az utolso receptig
    if (rec_lepteto == NULL)
        return true;

    return false;
}


bool rec_kezdobetu_listazas(int oldalszam, Listak* adatok)
{
    int szamlalo = 1;
    ReceptLista * rec_lepteto = adatok->elso_rec;

    econio_textcolor(14);
    printf("A %c-val/vel kezdodo receptek %d. oldal\n\n", (adatok->kezdobetu)[0], oldalszam);
    econio_textcolor(11);

    while (rec_lepteto != NULL && szamlalo <= (oldalszam*10)+1)
    {

        if (tolower((rec_lepteto->rec_nev)[0]) == tolower(adatok->kezdobetu[0]))
        {
            if (szamlalo > (oldalszam-1)*10 && szamlalo <= oldalszam*10)
            {
                printf("[%d] %s\n", szamlalo, rec_lepteto->rec_nev);

            }
            szamlalo++;
        }

        rec_lepteto = rec_lepteto->kov_rec;
    }
    if (szamlalo == 1)
    {
        hiba_uzenet("Nincs az adott kezdobetuvel kezdodo recept.");
    }

    adatok->rec_max = szamlalo-1;

    printf("\n");

    // A NULL ertek jelzi hogy eljutottunk az utolso receptig
    if (rec_lepteto == NULL)
        return true;

    return false;
}


bool rec_szoreszlet_listazas(int oldalszam, Listak* adatok)
{
    int szamlalo = 1;
    ReceptLista * rec_lepteto = adatok->elso_rec;

    econio_textcolor(14);
    printf("A \"%s\"-t tartalmazo receptek %d. oldal\n\n", adatok->kezdobetu, oldalszam);
    econio_textcolor(11);

    while (rec_lepteto != NULL && szamlalo <= (oldalszam*10)+1)
    {

        if (strstr(rec_lepteto->rec_nev, adatok->kezdobetu) != NULL)
        {
            if (szamlalo > (oldalszam-1)*10 && szamlalo <= oldalszam*10)
            {
                printf("[%d] %s\n", szamlalo, rec_lepteto->rec_nev);

            }
            szamlalo++;
        }

        rec_lepteto = rec_lepteto->kov_rec;
    }
    if (szamlalo == 1)
    {
        hiba_uzenet("Nem talalhato megadott szot/reszletet tartalmazo recept.");
    }

    adatok->rec_max = szamlalo-1;

    printf("\n");

    // A NULL ertek jelzi hogy eljutottunk az utolso receptig
    if (rec_lepteto == NULL)
        return true;

    return false;
}


void rec_lista_menu()
{
    printf("[e] Elore | ");
    printf("[h] Hatra | ");
    printf("[u] Uj recept | ");
    printf("[v] Vissza | ");
    printf("[f] Fomenu");
    printf("\n");
}


char rec_lista_valaszto(int* sorszam)
{
    char vezerlo_kar = 'a';
    printf("Kerem valasszon a menu opciok kozul: ");

    if (scanf("%d", sorszam) == 1)
    {
        while(getchar() != '\n');
        return '0';
    }

    scanf(" %c", &vezerlo_kar);

    while (getchar() != '\n');

    return vezerlo_kar;
}


void recept_adatok(Listak* adatok, int sorszam)
{
    ReceptLista* keresett = NULL;

    if ((adatok->kezdobetu)[0] == '0' && adatok->kezdobetu[1] == '\0')
        keresett = rec_lista_sorszam_keres(adatok->elso_rec, sorszam);

    else if(adatok->kezdobetu[1] == '\0' && ((adatok->kezdobetu[0] >= 'a' && adatok->kezdobetu[0] <= 'z') || (adatok->kezdobetu[0] >= 'A' && adatok->kezdobetu[0] <= 'Z')))
        keresett = rec_lista_kezdobetu_sorszam(adatok->elso_rec, sorszam, adatok->kezdobetu);

    else if (adatok->rec_max != 0 && strlen(adatok->kezdobetu) >= 2 )
        keresett = rec_lista_szoreszlet_sorszam(adatok->elso_rec, sorszam, adatok->kezdobetu);

    else if (strlen(adatok->kezdobetu) >= 2)
        keresett = rec_osszetevo_keres(adatok->elso_rec, sorszam, adatok->kezdobetu);



    bool vissza = false;
    char valaszto;
    int hiba_kodja = 0;

    while (!vissza && !(adatok->fomenu) && keresett != NULL)
    {
        econio_clrscr();

        recept_adatok_megjelenit(keresett);

        printf("\n\n");

        recept_adatok_menu();

        printf("\n");

        if (hiba_kodja != 0)
            hiba_kodja = rec_menu_hibauzenet(hiba_kodja);

        printf("Kerem valasszon az opciok kozul: ");
        scanf(" %c", & valaszto);

        while(getchar()!= '\n');

        switch (valaszto)
        {
        case 'v':
            vissza = true;
            break;

        case 'f':
            adatok->fomenu = true;
            break;

        case 'm':
            rec_modositas_menu(adatok, keresett);
            break;

        case 't':
            adatok->elso_rec = rec_torles(adatok->elso_rec, &keresett);
            break;

        default:
            hiba_kodja = 1;
            break;
        }
    }
}


void recept_adatok_megjelenit(ReceptLista * keresett)
{

    barna_sarga("Recept");
    econio_textcolor(14);
    printf("%s\n", keresett->rec_nev);

    printf("\n");

    barna_sarga("Hozzavalok:");
    rec_osszetevo_bejaras(keresett->kov_ossz);
    printf("\n");

    barna_sarga("Leiras");

    if (keresett->leiras != NULL)
        printf("%s", keresett->leiras);
    else
        hiba_uzenet("Nincs megjelenitheto leiras!");

    econio_textcolor(11);
}


void rec_osszetevo_bejaras(OsszetevoLista * elso)
{
    if (elso == NULL)
    {
        hiba_uzenet("Nincs megjelenitheto osszetevo!");
        return;
    }

    OsszetevoLista * lepteto = elso;

    while(lepteto != NULL)
    {
        printf("%s (%d %s)\n", lepteto->ossz_nev, lepteto->mennyiseg, lepteto->mertekegyseg);
        lepteto = lepteto->kov_ossz;
    }
}


void recept_adatok_menu()
{
    printf("[m] Modositas | ");
    printf("[t] Torles | ");
    printf("[v] Vissza | ");
    printf("[f] Fomenu\n");
}


void rec_modositas_menu(Listak* adatok, ReceptLista* keresett)
{
    bool vissza = false;
    int valasztott;
    int hiba_kodja = 0;

    while (!vissza)
    {
        valasztott = 0;

        econio_clrscr();

        recept_adatok_megjelenit(keresett);

        printf("\n\n");

        printf("Melyik adaton szeretnel modositani?\n");

        printf("\n");

        printf("[1] Recept neve\n");
        printf("[2] Hozzavalok\n");
        printf("[3] Recept leirasa\n");
        printf("[4] vissza");

        printf("\n\n");

        if (hiba_kodja != 0)
            rec_menu_hibauzenet(hiba_kodja);
        printf("Kerem valasszon az opciok kozul: ");
        scanf("%d", &valasztott);

        while(getchar() != '\n');

        switch (valasztott)
        {
        case 1:
            recnev_modosit(adatok, keresett);
            break;

        case 2:
            rec_ossz_modosit_menu(adatok, keresett);
            break;

        case 3:
            rec_leiras_modosit(adatok, keresett);
            break;

        case 4:
            vissza = true;
            break;

        default:
            hiba_kodja = 1;
        }
    }
}


