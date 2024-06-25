#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "adat_kezeles.h"
#include "osszetevok.h"
#include "receptek.h"
#include "uj_recept.h"
#include "debugmalloc.h"
#include "kinezet.h"
#include "econio.h"
#include "menu.h"


void kivag(char ** sor)
{
    bool eleje = false;
    while (!eleje && **sor != '\0')
    {
        if (**sor != '#' && **sor != ' ' && **sor != '\t')
        {
            eleje = true;
        }
        else
            *sor = *sor + 1;
    }
}


int kar_to_szam(char * sor)
{
    int kar_hossz = strlen(sor)-1;
    int osszeg = 0;
    int helyiert = 1;
    int ertek;

    while (kar_hossz >= 0)
    {
        ertek = sor[kar_hossz]-'0';
        if (ertek >= 0 && ertek < 10)
        {
            osszeg += ertek*helyiert;
            helyiert = helyiert * 10;
        }

        kar_hossz-= 1;
    }
    return osszeg;
}


char* hosszu_sor_beolvas()
{
    char* tarolo = NULL;
    char betu;
    bool leall = false;

    while(!leall && scanf("%c", &betu) != EOF)
    {
        if (betu == '\n' && tarolo != NULL)
        {
            leall = true;
        }

        else if (betu != '\n' && betu != '#')
        {
            if (tarolo == NULL)
            {
                tarolo = (char*) malloc (sizeof(char)*2);

                if (tarolo == NULL)
                    return NULL;

                tarolo[0] = betu;
                tarolo[1] = '\0';
            }
            else
            {
                int szohossz = 1 + strlen(tarolo);
                char * uj_nev = (char*) malloc (sizeof(char)*(szohossz+1));

                if (uj_nev == NULL)
                {
                    free(tarolo);
                    return NULL;
                }

                strcpy(uj_nev, tarolo);
                uj_nev[szohossz-1] = betu;
                uj_nev[szohossz] = '\0';

                free(tarolo);

                tarolo = uj_nev;
            }
        }
    }
    return tarolo;
}


char* kezdobetu_beker()
{

    char* kezdobetu = (char*) malloc (2*sizeof(char));

    if (kezdobetu == NULL)
        return NULL;

    kezdobetu[1] = '\0';

    bool hiba = false;
    do
    {
        econio_clrscr();

        if (hiba)
            ujrec_hibakod(3);

        printf("Kerem adja meg a kezdobetut: ");
        scanf(" %c", kezdobetu);

        while (getchar() != '\n');

        if(!hiba)
            hiba = true;
    } while (!isalpha(kezdobetu[0]));

    return kezdobetu;
}


char* keresett_szo_beker()
{
    char* szo = NULL;
    bool hiba = false;

    do
    {
        econio_clrscr();
        if (szo != NULL)
        {
            free(szo);
            szo = NULL;
        }

        if (hiba)
            ujrec_hibakod(3);

        printf("Kerem adja meg a keresett adat szavat/reszletet (min. 2 karakter): ");

        szo = hosszu_sor_beolvas();

        if (!hiba)
            hiba = true;

    } while (strlen(szo) < 2);

    return szo;
}


int szo_osszehasonlit(char * elso, char * masodik)
{
    int first_idx = 0;
    int sec_idx = 0;
    while(elso[first_idx] != '\0' && masodik[sec_idx] != '\0')
    {
        char szo1_betui = elso[first_idx];
        char szo2_betui = masodik[sec_idx];

        if (isalpha(szo1_betui))
            szo1_betui = tolower(szo1_betui);

        if (isalpha(szo2_betui))
            szo2_betui = tolower(szo2_betui);

        if (szo1_betui < szo2_betui)
            return -1;

        else if (szo1_betui > szo2_betui)
            return 1;

        first_idx ++;
        sec_idx ++;
    }

    if (elso[first_idx] != '\0' && masodik[sec_idx] == '\0')
        return 1;
    else if (elso[first_idx] == '\0' && masodik[sec_idx] != '\0')
        return -1;
    else
        return 0;
}


char mentes()
{
    char bekert;

    printf("Szeretne menteni az adatot?\n");
    printf("[i] igen\n");
    printf("[n] nem\n");

    printf("\n");

    printf("Kerem valasszon az opciok kozul: ");

    scanf(" %c", &bekert);

    while(getchar() != '\n');

    return bekert;
}


bool kilepes()
{
    char bekert;
    bool hibas = false;

    do
    {
        econio_clrscr();
        econio_textcolor(4);
        puts("Ha most kilep, akkor nem kerulnek mentesre az adatok!");
        econio_textcolor(14);
        puts("Biztosan ki szeretne lepni?\n");
        econio_textcolor(11);

        puts("[i] igen");
        puts("[n] nem\n");


        if (hibas)
            hiba_uzenet("Ervenytelen erteket adott meg!");
        printf("Kerem valasszon az opciok kozul: ");

        scanf(" %c", &bekert);

        while (getchar() != '\n');

        if (!hibas)
            hibas = true;

    } while (bekert != 'i' && bekert != 'n');

    if (bekert == 'i')
        return true;

    return false;

}



// ---------------------Rendezo es kereso fuggvenyek---------------------------


ReceptLista* rec_rendezve_szur(ReceptLista * elso_rec, ReceptLista * uj_rec)
{
    if (elso_rec == NULL || szo_osszehasonlit(elso_rec->rec_nev, uj_rec->rec_nev)>0)
    {
        uj_rec->kov_rec = elso_rec;
        elso_rec = uj_rec;
        return elso_rec;
    }

    ReceptLista * aktualis = elso_rec;
    ReceptLista * lemarado = NULL;

    while(aktualis != NULL && szo_osszehasonlit(aktualis->rec_nev, uj_rec->rec_nev)<=0)
    {
        lemarado = aktualis;
        aktualis = aktualis->kov_rec;
    }
    uj_rec->kov_rec = aktualis;
    lemarado->kov_rec = uj_rec;

    return elso_rec;
}


OsszetevoLista* rec_osszetevo_rendezve_szur(OsszetevoLista* elso_ossz, OsszetevoLista* uj_ossz)
{

    if (elso_ossz == NULL || szo_osszehasonlit(elso_ossz->ossz_nev, uj_ossz->ossz_nev) > 0)
    {

        uj_ossz->kov_ossz = elso_ossz;
        elso_ossz = uj_ossz;
        return elso_ossz;
    }

    OsszetevoLista* aktualis = elso_ossz;
    OsszetevoLista* lemarado = NULL;

    while(aktualis != NULL && szo_osszehasonlit(aktualis->ossz_nev, uj_ossz->ossz_nev)<=0)
    {
        lemarado = aktualis;
        aktualis = aktualis->kov_ossz;
    }
    uj_ossz->kov_ossz = aktualis;
    lemarado->kov_ossz = uj_ossz;

    return elso_ossz;
}


OsszOsszetevo* receptbol_beszur(Listak* adatok)
{
    ReceptLista * rec_lepteto = adatok->elso_rec;

    OsszOsszetevo* elso_ossz = adatok->elso_ossz;

    // Receptek bejarasa
    while (rec_lepteto != NULL)
    {
        OsszetevoLista * rec_ossz_lepteto = rec_lepteto->kov_ossz;

        // Egy recept osszetevoinek bejarasa
        while(rec_ossz_lepteto != NULL)
        {
            OsszOsszetevo* talalat = osszetevo_nev_keres(elso_ossz, rec_ossz_lepteto->ossz_nev);

            // Ha NULL akkor nem talalt meg ilyen hozzavalot a kulon osszetevo listaban
            if  (talalat == NULL)
            {
                // A receptben talalhato osszetevobol csinalunk egy kulon osszetevo listahoz tartozo elemet.
                OsszOsszetevo* uj_ossz = ossz_listaeleme_alakit(rec_ossz_lepteto->ossz_nev);

                // Ha sikeresen megcsinaltuk az uj elemet akkor rendezve beszurjuk azt.
                if (uj_ossz != NULL)
                {
                    elso_ossz = ossz_rendezve_beszur(elso_ossz, uj_ossz);
                }
            }

            rec_ossz_lepteto = rec_ossz_lepteto->kov_ossz;
        }

        rec_lepteto = rec_lepteto->kov_rec;
    }
    return elso_ossz;
}


OsszOsszetevo* ossz_rendezve_beszur(OsszOsszetevo * elso_ossz, OsszOsszetevo * uj_ossz)
{

    // Ha az elso osszetevo meg nem letezi kakkor az ujat az elsonek menti el.
    if (elso_ossz == NULL)
    {
        elso_ossz = uj_ossz;
        return elso_ossz;
    }

    int kisebb_szo;

    OsszOsszetevo * lepteto = elso_ossz;
    OsszOsszetevo * lemarado = NULL;

    while (lepteto != NULL)
    {

        kisebb_szo = szo_osszehasonlit(lepteto->osszetevo, uj_ossz->osszetevo);

        // Ha megtalaltuk azt az elemet ami nagyobb akkor ele beszurjuk az ujat.
        if (lemarado == NULL && kisebb_szo > 0 )
        {
            uj_ossz->kov = lepteto;
            elso_ossz = uj_ossz;
            return elso_ossz;
        }

        else if (kisebb_szo > 0)
        {
            uj_ossz->kov = lepteto;
            lemarado->kov = uj_ossz;
            return elso_ossz;
        }

        lemarado = lepteto;
        lepteto = lepteto->kov;
    }

    // Ha eljutunk a NULL-ig akkor ez a leganygobb elem igy a vegere tesszuk
    uj_ossz->kov = lepteto;
    lemarado->kov = uj_ossz;

    return elso_ossz;
}


ReceptLista* rec_lista_sorszam_keres(ReceptLista * elso_rec, int sorszam)
{
    ReceptLista * lepteto = elso_rec;
    int szamlalo = 1;
    while (lepteto != NULL && szamlalo != sorszam)
    {
        lepteto = lepteto->kov_rec;
        szamlalo++;
    }
    return lepteto;
}


ReceptLista* rec_lista_kezdobetu_sorszam(ReceptLista* elso_rec, int sorszam, char* kezdobetu)
{
    ReceptLista* lepteto = elso_rec;
    int szamlalo = 1;

    while (lepteto != NULL)
    {

        if (tolower(lepteto->rec_nev[0]) == tolower(kezdobetu[0]))
        {
            if (szamlalo == sorszam)
                return lepteto;
            else
                szamlalo++;
        }

        lepteto = lepteto->kov_rec;

    }
   return lepteto;
}


ReceptLista* rec_lista_szoreszlet_sorszam(ReceptLista* elso_rec, int sorszam, char* kezdobetu)
{
    ReceptLista* lepteto = elso_rec;
    int szamlalo = 1;

    while (lepteto != NULL)
    {
        if (strstr(lepteto->rec_nev, kezdobetu) != NULL)
        {
            if (szamlalo == sorszam)
                return lepteto;
            else
                szamlalo++;
        }


        lepteto = lepteto->kov_rec;
    }

   return lepteto;
}


ReceptLista* rec_osszetevo_keres(ReceptLista* elso_rec, int sorszam, char* kezdobetu)
{
    ReceptLista* lepteto = elso_rec;
    int szamlalo = 1;

    while (lepteto != NULL)
    {
        OsszetevoLista* rec_ossz_lepteto = lepteto->kov_ossz;

        while(rec_ossz_lepteto != NULL)
        {
            if (szo_osszehasonlit(rec_ossz_lepteto->ossz_nev, kezdobetu) == 0)
            {
                if (szamlalo == sorszam)
                    return lepteto;
                else
                {
                    szamlalo++;
                }

            }
            rec_ossz_lepteto = rec_ossz_lepteto->kov_ossz;
        }

        lepteto = lepteto->kov_rec;

    }

   return lepteto;
}


ReceptLista* huto_rec_osszetevo_keres(Listak* adatok, int sorszam, OsszOsszetevo* huto)
{
    int szamlalo = 1;
    bool talalat = false;
    bool tovabb = false;
// valami 1
// so, liszt, bors
// valami hozzavalo: tej, viz, so, bors, NULL
    ReceptLista* rec_lepteto = adatok->elso_rec;

    while(rec_lepteto != NULL)
    {
        tovabb = false;

        OsszOsszetevo* huto_lepteto = huto;

        while(huto_lepteto != NULL && !tovabb)
        {
            talalat = false;

            OsszetevoLista* rec_ossz_lepteto = rec_lepteto->kov_ossz;

            while(rec_ossz_lepteto != NULL && !talalat)
            {
                if (szo_osszehasonlit(rec_ossz_lepteto->ossz_nev, huto_lepteto->osszetevo) == 0)
                    talalat = true;

                rec_ossz_lepteto = rec_ossz_lepteto->kov_ossz;
            }

            if(rec_ossz_lepteto == NULL && !talalat)
                tovabb = true;

            huto_lepteto = huto_lepteto->kov;
        }

        if(huto_lepteto == NULL && !tovabb)
        {
            if (szamlalo == sorszam)
                return rec_lepteto;

            szamlalo++;
        }
        rec_lepteto = rec_lepteto->kov_rec;
    }

   return rec_lepteto;
}


ReceptLista* rec_nev_keres(ReceptLista * elso_rec, char * keresett_szo)
{
    ReceptLista * lepteto = elso_rec;

    while (lepteto != NULL)
    {
        if (szo_osszehasonlit(lepteto->rec_nev, keresett_szo) == 0)
        {
            return lepteto;
        }
        lepteto = lepteto->kov_rec;
    }

    return NULL;
}


OsszOsszetevo* ossz_lista_sorszam_keres(OsszOsszetevo * elso, int sorszam)
{
    OsszOsszetevo * lepteto = elso;
    int szamlalo = 1;
    while (szamlalo != sorszam)
    {
        lepteto = lepteto->kov;
        szamlalo++;
    }
    return lepteto;
}


OsszOsszetevo* ossz_lista_kezdobetu_sorszam(OsszOsszetevo* elso_ossz, int sorszam, char* kezdobetu)
{
    OsszOsszetevo* lepteto = elso_ossz;
    int szamlalo = 1;

    while (lepteto != NULL)
    {
        if (tolower(lepteto->osszetevo[0]) == tolower(kezdobetu[0]))
        {
            if (sorszam == szamlalo)
                return lepteto;
            else
                szamlalo++;
        }

        lepteto = lepteto->kov;

    }
   return lepteto;
}


OsszOsszetevo* ossz_lista_szoreszlet_sorszam(OsszOsszetevo* elso_ossz, int sorszam, char* kezdobetu)
{
    OsszOsszetevo* lepteto = elso_ossz;
    int szamlalo = 1;

    while (lepteto != NULL)
    {
        if (strstr(lepteto->osszetevo, kezdobetu) != NULL)
        {
            if (szamlalo == sorszam)
                return lepteto;
            else
                szamlalo++;
        }

        lepteto = lepteto->kov;
    }
   return lepteto;
}


OsszOsszetevo* osszetevo_nev_keres(OsszOsszetevo* elso_ossz, char * szo)
{
    OsszOsszetevo * lepteto = elso_ossz;

    while (lepteto != NULL)
    {
        if (szo_osszehasonlit(lepteto->osszetevo, szo) == 0)
            return lepteto;

        lepteto = lepteto->kov;
    }

    return NULL;
}



// ---------------------Torlo es felszabadito fuggvenyek---------------------------


ReceptLista * rec_torles(ReceptLista * elso_rec, ReceptLista ** torlendo)
{

    if (!rec_del_kerdes(*torlendo))
        return elso_rec;


    if (*torlendo == elso_rec)
    {
        elso_rec = (*torlendo)->kov_rec;
        egy_rec_felszabadit(*torlendo);

        *torlendo = NULL;
        return elso_rec;
    }

    ReceptLista * lepteto = elso_rec;
    ReceptLista * lemarado = NULL;

    while (lepteto != NULL && lepteto != *torlendo)
    {
        lemarado = lepteto;
        lepteto = lepteto->kov_rec;
    }

    lemarado->kov_rec = (*torlendo)->kov_rec;

    egy_rec_felszabadit(*torlendo);

    *torlendo = NULL;

    return elso_rec;
}


ReceptLista* rec_felszabadit(ReceptLista * elso_rec)
{
    ReceptLista * rec_lepteto = elso_rec;
    while(rec_lepteto != NULL)
    {
        ReceptLista * temp = rec_lepteto->kov_rec;

        rec_lepteto = egy_rec_felszabadit(rec_lepteto);

        free(rec_lepteto);
        rec_lepteto = temp;

    }
    return NULL;
}


ReceptLista* egy_rec_felszabadit(ReceptLista * felszabaditando)
{
    if (felszabaditando != NULL)
    {

        if (felszabaditando->rec_nev != NULL)
            free(felszabaditando->rec_nev);

        if (felszabaditando->leiras != NULL)
            free(felszabaditando->leiras);

        felszabaditando->kov_ossz = rec_ossz_felszabadit(felszabaditando->kov_ossz);

        free(felszabaditando);
    }


    return NULL;
}


bool rec_del_kerdes(ReceptLista* torlendo)
{
    bool hibas = false;
    char bekert;

    do
    {
        econio_clrscr();

        if (hibas)
            hiba_uzenet("Hibas erteket adtal meg!");

        econio_textcolor(14);
        printf("Biztosan vegleg torolni szeretne a(z) %s receptet?\n", torlendo->rec_nev);
        econio_textcolor(11);

        printf("\n");

        printf("[i] igen\n");
        printf("[n] nem\n");

        printf("\n");

        printf("Kerem valasszon az opciok kozul: ");

        scanf(" %c", &bekert);

        while(getchar() != '\n');

        if (!hibas)
            hibas = true;

    } while (bekert != 'i' && bekert != 'n');

    if (bekert == 'i')
        return true;

    else return false;
}


OsszetevoLista* rec_ossz_felszabadit(OsszetevoLista* elso_rec_ossz)
{

    OsszetevoLista* lepteto = elso_rec_ossz;

    while(lepteto != NULL)
    {
        OsszetevoLista* temp = lepteto->kov_ossz;

        lepteto = egy_ossz_felszabadit(lepteto);

        lepteto = temp;
    }

    return NULL;
}


OsszetevoLista* egy_ossz_felszabadit(OsszetevoLista* elem)
{
    if (elem != NULL)
    {
        if (elem->ossz_nev != NULL)
            free(elem->ossz_nev);
        if (elem->mertekegyseg != NULL)
            free(elem->mertekegyseg);

        free(elem);
    }

    return NULL;
}


void osszetevo_torles(Listak* adatok, OsszOsszetevo* keresett)
{
    ReceptLista* rec_lepteto = adatok->elso_rec;

    while(rec_lepteto != NULL)
    {

        if (rec_lepteto->kov_ossz != NULL)
        {
            if (szo_osszehasonlit(rec_lepteto->kov_ossz->ossz_nev, keresett->osszetevo) == 0)
            {
                OsszetevoLista* temp = rec_lepteto->kov_ossz->kov_ossz;
                egy_ossz_felszabadit(rec_lepteto->kov_ossz);
                rec_lepteto->kov_ossz = temp;
            }

            else
            {
                OsszetevoLista* rec_ossz_lepteto = rec_lepteto->kov_ossz;
                OsszetevoLista* rec_ossz_lemarado = NULL;

                while (rec_ossz_lepteto != NULL)
                {

                    if (szo_osszehasonlit(rec_ossz_lepteto->ossz_nev, keresett->osszetevo) == 0)
                    {
                        rec_ossz_lemarado->kov_ossz = rec_ossz_lepteto->kov_ossz;
                        egy_ossz_felszabadit(rec_ossz_lepteto);
                        rec_ossz_lepteto = rec_ossz_lemarado->kov_ossz;
                    }
                    else
                    {
                        rec_ossz_lemarado = rec_ossz_lepteto;
                        rec_ossz_lepteto = rec_ossz_lepteto->kov_ossz;
                    }

                }
            }
        }
        printf("\n");
        rec_lepteto = rec_lepteto->kov_rec;
    }

    if (adatok->elso_ossz == keresett)
    {
        adatok->elso_ossz = keresett->kov;
        free(keresett->osszetevo);
        free(keresett);
        keresett = NULL;
    }

    else
    {
        OsszOsszetevo* ossz_aktualis = adatok->elso_ossz;
        OsszOsszetevo* ossz_lemarado = NULL;

        while (ossz_aktualis != keresett)
        {
            ossz_lemarado = ossz_aktualis;
            ossz_aktualis = ossz_aktualis->kov;
        }

        ossz_lemarado->kov = ossz_aktualis->kov;
        free(ossz_aktualis->osszetevo);
        free(ossz_aktualis);
        ossz_aktualis = NULL;
    }
}


void osszetevok_felszabadit(OsszOsszetevo * elso_ossz)
{
    while (elso_ossz != NULL)
    {
        OsszOsszetevo * temp = elso_ossz->kov;
        free(elso_ossz->osszetevo);
        free(elso_ossz);
        elso_ossz = temp;
    }
}


bool osszetevo_del_kerdes(OsszOsszetevo* keresett)
{
    char bekert;
    bool hibas = false;

    do
    {
        econio_clrscr();

        econio_textcolor(4);
        puts("Ha torli a hozzavalot, akkor az osszes receptbol amiben szerepel torlodni fog!");

        econio_textcolor(14);
        printf("Biztosan vegleg torolni szeretne a(z) %s hozzavalot?\n", keresett->osszetevo);
        econio_textcolor(11);

        printf("\n");

        puts("[i] igen");
        puts("[n] nem");

        printf("\n");

        if (hibas)
            hiba_uzenet("Ervenytelen erteket adott meg!");

        printf("Kerem valasszon az opciok kozul: ");
        scanf(" %c", &bekert);

        while (getchar()!= '\n');

        if (!hibas)
            hibas = true;

    } while (bekert != 'i' && bekert != 'n');

    if (bekert == 'i')
        return true;

    return false;
}




