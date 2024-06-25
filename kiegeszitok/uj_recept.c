#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "uj_recept.h"
#include "debugmalloc.h"
#include "receptek.h"
#include "osszetevok.h"
#include "adat_kezeles.h"
#include "kinezet.h"
#include "econio.h"


void ujrec_adat_bekeres(Listak* adatok)
{
    ReceptLista * uj = (ReceptLista*) malloc(sizeof(ReceptLista));

    if (uj == NULL)
        return;

    uj->rec_nev = NULL;
    uj->kov_ossz = NULL;
    uj->leiras = NULL;
    uj->kov_rec = NULL;

    // bekeri az uj recept nevet
    do
    {
        uj->rec_nev = ujrec_nev_beker(adatok->elso_rec);
    } while (uj->rec_nev == NULL && !kilepes());


    //Ha sikeres volt az uj recept nevenek a bekerese akkor bekeri a tovabbi adatot
    if (uj->rec_nev != NULL)
    {

        // Bekeri az uj recepthez az osszetevoket
        uj->kov_ossz = ujrec_osszetevok_lista(uj->rec_nev, adatok);

        // Ha sikeres volt az uj rec nev beolvasas es osszetevok beolvasasa akkor leirast is beker.
        if (uj->kov_ossz != NULL)
        {
            do
            {
                uj->leiras = ujrec_leiras_beker(uj);
            } while (uj->leiras == NULL && !kilepes());
        }
    }

    // Ha valamelyik adat bekerese sikertelen volt akkor felszabaditja az itt foglalt memoriat.
    if (uj->rec_nev == NULL || uj->kov_ossz == NULL || uj->leiras == NULL)
    {
        uj = egy_rec_felszabadit(uj);
        return;
    }

    char mentes_kar;
    bool hibas = false;

    // Az uj adat menteset kerdezi meg a felhasznalotol
    do
    {
        econio_clrscr();
        recept_adatok_megjelenit(uj);

        printf("\n\n");

        if (hibas)
            hiba_uzenet("Ervenytelen erteket adott meg!");

        mentes_kar = mentes();

        if (!hibas)
            hibas = true;

    } while (mentes_kar != 'i' && mentes_kar != 'n');

    if (mentes_kar == 'i')
    {
        // Sikeres uj recept letrehozasnal rendezve hozzaadja a receptek listahoz es ha van uj hozzavalo azt is.
        adatok->elso_rec = rec_rendezve_szur(adatok->elso_rec, uj);
        adatok->elso_ossz = receptbol_beszur(adatok);
    }

    else
    {
        egy_rec_felszabadit(uj);
    }
}


char* ujrec_nev_beker(ReceptLista * elso_rec)
{
    char mentes_kar;
    char * rec_nev = NULL;
    bool mar_szerepel = false;

    do
    {
        econio_clrscr();

        if (mar_szerepel)
        {
            hiba_uzenet("Ez a recept nev mar szerepel a listaban!");
            mar_szerepel = false;
        }

        econio_textcolor(6);
        puts("Egy \"x\" megadasaval kilephet az adatok megadasabol.");
        econio_textcolor(11);

        printf("\n");

        printf("Az uj recept neve: ");

        rec_nev = hosszu_sor_beolvas();

        if (rec_nev[0] == 'x' && rec_nev[1] == '\0')
        {
            free(rec_nev);
            return NULL;
        }

        bool hibas = false;
        if (rec_nev_keres(elso_rec, rec_nev) == NULL)
            do
            {
                econio_clrscr();
                printf("Az on altal megadott recept neve: ");
                sarga_kek(rec_nev);

                printf("\n");

                if (hibas)
                    hiba_uzenet("Ervenytelen erteket adott meg!");

                mentes_kar = mentes();

                if (!hibas)
                    hibas = true;

            } while(mentes_kar != 'i' && mentes_kar != 'n');

        else
            mar_szerepel = true;

        if (mentes_kar != 'i')
        {
            free(rec_nev);
            rec_nev = NULL;
        }
    } while (mentes_kar != 'i');

    return rec_nev;
}


OsszetevoLista * ujrec_osszetevok_lista(char * recept, Listak* adatok)
{

    int oldalszam = 1;
    bool vissza = false;
    char valaszto;
    bool vege = false;

    int hiba_kodja = 0;

    int sorszam;

    OsszetevoLista* elso_rec_ossz = NULL;

    while (!vissza)
    {
        sorszam = 0;

        econio_clrscr();

        printf("Az uj recept neve: ");
        barna_kek(recept);

        printf("\n");

        puts("Mar hozzaadott osszetevok:");
        if (elso_rec_ossz != NULL)
        {
            econio_textcolor(6);
            rec_osszetevo_bejaras(elso_rec_ossz);
            econio_textcolor(14);
        }

        else
            hiba_uzenet("Meg nem adott meg osszetevot.");


        printf("\n");

        vege = ossz_abc_listazas(oldalszam, adatok);

        ujrec_ossz_lista_menu();

        printf("\n\n");

        hiba_kodja = ujrec_hibakod(hiba_kodja);

        printf("Egy sorszam megadasaval az adott hozzavalot tudja hozzaadni a recepthez.\n");

        valaszto = rec_lista_valaszto(&sorszam);

        switch (valaszto)
        {
        case 'e':
            if (!vege)
                oldalszam += 1;
            else
                hiba_kodja = 1;
            break;

        case 'h':
            if (oldalszam > 1)
                oldalszam -= 1;
            else
                hiba_kodja = 2;
            break;

        case 'x':
            if (kilepes())
                return rec_ossz_felszabadit(elso_rec_ossz);
            break;

        case 'u':
            elso_rec_ossz = ujrec_ossz_hozzaadas(elso_rec_ossz, adatok->elso_ossz, sorszam);
            break;

        case '0':
            elso_rec_ossz = ujrec_ossz_hozzaadas(elso_rec_ossz, adatok->elso_ossz, sorszam);
            break;

        case 't':
            if (elso_rec_ossz != NULL)
                return elso_rec_ossz;
            else
                hiba_kodja = 4;
            break;

        default:
            hiba_kodja = 3;
            break;
        }
    }
    return elso_rec_ossz;
}


int ujrec_hibakod(int hiba_kodja)
{
    switch (hiba_kodja)
    {
    case 1:
        hiba_uzenet("Nincs kovetkezo oldal!");
        break;
    case 2:
        hiba_uzenet("Nincs visszafele tovabbi oldal!");
        break;
    case 3:
        hiba_uzenet("Ervenytelen erteket adtal meg!");
        break;
    case 4:
        hiba_uzenet("A tovabblepeshez legalabb egy hozzavalot hozza kell adni!");
        break;
    }
    return 0;
}


void ujrec_ossz_lista_menu()
{
    printf("[e] Elore | ");
    printf("[h] Hatra | ");
    printf("[u] Hozzavalo begepelese | ");
    printf("[t] Tovabb a leirashoz | ");
    printf("[x] Kilepes\t");
}


OsszetevoLista* ujrec_ossz_hozzaadas(OsszetevoLista* elso_rec_ossz, OsszOsszetevo* elso_ossz, int sorszam)
{

    OsszetevoLista* uj = (OsszetevoLista*) malloc (sizeof(OsszetevoLista));

    if (uj == NULL)
        return NULL;

    uj->kov_ossz = NULL;
    uj->ossz_nev = NULL;
    uj->mennyiseg = -1;
    uj->mertekegyseg = NULL;



    // Ha a sorszam nagyobb mint 0 akkor a listabol valaszt hozzavalot.
    if (sorszam > 0)
    {
        OsszOsszetevo* keresett = ossz_lista_sorszam_keres(elso_ossz, sorszam);

        int szohossz = strlen(keresett->osszetevo);

        char* uj_ossz = (char*) malloc ((szohossz+1)*sizeof(char));

        if (uj_ossz == NULL)
        {
            free(uj);
            return NULL;
        }

        strcpy(uj_ossz, keresett->osszetevo);

        uj->ossz_nev = uj_ossz;
    }

    // Ha a sorszam 0 akkor a hozzavalo begepelese lett kivalasztva.
    else if (sorszam == 0)
    {
        econio_clrscr();
        printf("Kerem adja meg az uj hozzavalo nevet: ");
        uj->ossz_nev = hosszu_sor_beolvas();
    }

    // Hozzavalo mennyiseg bekeres
    bool hibas = false;
    do
    {
        econio_clrscr();

        printf("A hozzaadando hozzavalo neve: ");
        econio_textcolor(14);
        puts(uj->ossz_nev);
        econio_textcolor(11);

        if (hibas)
            hiba_uzenet("Ervenytelen erteket adott meg!");

        printf("Hozzavalo mennyisegenek szama: ");

        scanf("%d", &uj->mennyiseg);

        while(getchar() != '\n');

        if (!hibas)
            hibas = true;

    } while (uj->mennyiseg < 1);

    // Hozzavalo mertekegyseg bekeres
    printf("Kerem adja meg a hozzavalo mennyisegenek mertekegyseget: ");

    uj->mertekegyseg = hosszu_sor_beolvas();

    char mentes_kar;
    hibas = false;

    // Uj hozzavalo mentese kerdes
    do
    {
        econio_clrscr();

        printf("Az on altal megadott adatok:\n");

        printf("Hozzavalo neve: ");
        sarga_kek(uj->ossz_nev);

        printf("Mennyiseg: " );
        econio_textcolor(14);
        printf("%d %s", uj->mennyiseg, uj->mertekegyseg);
        econio_textcolor(11);

        printf("\n\n");

        if (hibas)
            hiba_uzenet("Ervenytelen erteket adott meg!");

        mentes_kar = mentes();

        if (!hibas)
            hibas = true;
    } while (mentes_kar != 'i' && mentes_kar != 'n');


    if (mentes_kar == 'i')
    {
        elso_rec_ossz = rec_osszetevo_rendezve_szur(elso_rec_ossz, uj);
    }
    else
    {
        egy_ossz_felszabadit(uj);
    }

    return elso_rec_ossz;
}


char* ujrec_leiras_beker(ReceptLista* elem)
{
    char mentes_kar;
    char* uj_leiras = NULL;

    do
    {
    econio_clrscr();

    econio_textcolor(6);
    puts("Egy \"x\" megadasaval kilephet az adatok megadasabol.");
    econio_textcolor(11);

    printf("\n");

    printf("Kerem adja meg a %s recept leirasat.\n", elem->rec_nev);

    printf("Az uj recept leirasa: ");

    if(uj_leiras != NULL)
        free(uj_leiras);

    uj_leiras = hosszu_sor_beolvas();

    if (uj_leiras[0] == 'x' && uj_leiras[1] == '\0')
    {
        free(uj_leiras);
        uj_leiras = NULL;
        return NULL;
    }

    econio_clrscr();

    printf("Az uj recept leirasa: ");
    econio_textcolor(14);
    puts(uj_leiras);
    econio_textcolor(11);

    printf("\n");

    mentes_kar = mentes();

    } while (mentes_kar != 'i');

    return uj_leiras;
}


void recnev_modosit(Listak* adatok, ReceptLista* keresett)
{
    char * uj_nev;

    econio_clrscr();

    uj_nev = ujrec_nev_beker(adatok->elso_rec);

    if (uj_nev != NULL)
    {
        free(keresett->rec_nev);
        keresett->rec_nev = uj_nev;

        if (keresett == adatok->elso_rec)
        {
            adatok->elso_rec = keresett->kov_rec;
        }

        else
        {
            ReceptLista* elozo = adatok->elso_rec;

            while (elozo->kov_rec != keresett)
            {
                elozo = elozo->kov_rec;
            }

            elozo->kov_rec = keresett->kov_rec;
        }

        adatok->elso_rec = rec_rendezve_szur(adatok->elso_rec, keresett);


        return;
    }

    return;
}


void rec_ossz_modosit_menu(Listak* adatok, ReceptLista* keresett)
{
    bool vissza = false;
    bool ervenytelen = false;
    char bekert;

    while (!vissza)
    {

        econio_clrscr();

        puts("Milyen modositast szeretne vegezni a hozzavalokon?");

        printf("\n");

        puts("[f] Feluliras");
        puts("[t] Torles");
        puts("[u] Uj hozzaadas");
        puts("[v] Vissza");

        printf("\n");

        if (ervenytelen)
        {
            hiba_uzenet("Ervenytelen erteket adott meg!");
            ervenytelen = false;
        }

        printf("Kerem valasszon az opciok kozul: ");
        scanf(" %c", &bekert);

        while(getchar() != '\n');

        switch (bekert)
        {
        case 'f':
            if (keresett->kov_ossz != NULL)
                rec_ossz_modositas(adatok, keresett, bekert);
            break;

        case 't':
            if (keresett->kov_ossz != NULL)
            {
                rec_ossz_modositas(adatok, keresett, bekert);
            }

            break;

        case 'u':
            rec_ossz_modositas(adatok, keresett, bekert);
            break;

        case 'v':
            vissza = true;
            adatok->elso_ossz = receptbol_beszur(adatok);
            break;

        default:
            ervenytelen = true;
        }
    }
}


void rec_ossz_modositas(Listak* adatok, ReceptLista* keresett, char funkcio)
{
    int valaszto;
    bool vissza = false;
    OsszetevoLista* uj_rec_ossz;

    while (!vissza)
    {
        uj_rec_ossz = NULL;

        if (funkcio == 'f' || funkcio == 't')
        {
            valaszto = -1;

            econio_clrscr();

            OsszetevoLista * lepteto = keresett->kov_ossz;

            adatok->ossz_max = 0;

            barna_sarga("Hozzavalok:");

            while(lepteto != NULL)
            {
                adatok->ossz_max++;
                printf("[%d] %s - %d %s\n", adatok->ossz_max, lepteto->ossz_nev, lepteto->mennyiseg, lepteto->mertekegyseg);
                lepteto = lepteto->kov_ossz;
            }

            printf("\n");

            puts("[0] vissza\n");

            econio_textcolor(11);

            if (funkcio == 'f')
                printf("Melyik hozzavalot szeretne felulirni? Sorszam: ");

            else if (funkcio == 't')
                printf("Melyik hozzavalot szeretne torolni? Sorszam: ");

            scanf("%d", &valaszto);

            while(getchar() != '\n');
        }
        else
            vissza = true;

        if (((funkcio == 'f' || funkcio == 't') && valaszto > 0 && valaszto <= adatok->ossz_max) || funkcio == 'u')
        {

            if (funkcio == 'f' || funkcio == 'u')
                uj_rec_ossz = ujrec_ossz_hozzaadas(uj_rec_ossz, adatok->elso_ossz, 0);


            if ((uj_rec_ossz != NULL && funkcio == 'f') || funkcio == 't')
            {
                int szamlalo = 1;
                OsszetevoLista* aktualis = keresett->kov_ossz;
                OsszetevoLista* elozo = NULL;

                if (valaszto == 1)
                {
                    keresett->kov_ossz = aktualis->kov_ossz;
                    egy_ossz_felszabadit(aktualis);
                    aktualis = NULL;
                }

                else
                {
                    while (szamlalo != valaszto)
                    {
                        elozo = aktualis;
                        aktualis = aktualis->kov_ossz;
                        szamlalo++;
                    }

                    elozo->kov_ossz = aktualis->kov_ossz;
                    egy_ossz_felszabadit(aktualis);
                    aktualis = NULL;
                }
            }


            if (uj_rec_ossz != NULL && (funkcio == 'f' || funkcio == 'u'))
            {
                keresett->kov_ossz = rec_osszetevo_rendezve_szur(keresett->kov_ossz, uj_rec_ossz);
            }




            if (keresett->kov_ossz == NULL)
                return;
        }

        else if(valaszto == 0)
        {
            vissza = true;
        }
    }
}


void rec_leiras_modosit(Listak* adatok, ReceptLista* keresett)
{
    char* uj;
    char mentes_kar;
    bool hiba = false;

    econio_clrscr();

    uj = ujrec_leiras_beker(keresett);

    if (uj != NULL)
    {
        do
        {
            econio_clrscr();

            barna_sarga("A regi leiras:");
            puts(keresett->leiras);

            printf("\n");

            barna_sarga("Az uj leiras:");
            puts(uj);

            printf("\n");

            if (hiba)
                hiba_uzenet("Ervenytelen erteket adott meg!");

            econio_textcolor(11);
            printf("Szeretne lecserelni az adatot?\n\n");

            puts("[i] igen");
            puts("[n] nem");

            scanf(" %c", &mentes_kar);

            while(getchar() != '\n');

            if (!hiba)
                hiba = true;

        } while (mentes_kar != 'i' && mentes_kar != 'n');

        if (mentes_kar == 'i')
        {
            free(keresett->leiras);
            keresett->leiras = uj;
        }
        else
            free(uj);
    }

}


