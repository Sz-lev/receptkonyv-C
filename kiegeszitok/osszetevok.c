#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "adat_kezeles.h"
#include "receptek.h"
#include "osszetevok.h"
#include "menu.h"
#include "kinezet.h"
#include "econio.h"
#include "debugmalloc.h"


void osszetevo_menu(Listak* adatok)
{
    int valasztott = 0;
    int hiba_kodja = 0;

    while (!adatok->fomenu)
    {
        econio_clrscr();

        // Menu opciok listazasa
        ossz_menu_listazas();

        if (hiba_kodja != 0)
            hiba_kodja = ossz_hibakod(hiba_kodja);

        // Vezerles bekerese
        valasztott = menu_valaszto();

        switch(valasztott)
        {

        case 1: // abc listazas
            if (adatok->elso_ossz != NULL)
            {
                adatok->kezdobetu = "0";
                osszetevo_lista_nezet(adatok);
            }
            else
                hiba_kodja = 2;
            printf("\n");
            break;

        case 2: // kezdobetu szerinti kereses
            if (adatok->elso_ossz != NULL)
            {
                adatok->kezdobetu = kezdobetu_beker(); // kezdobetu bekeres
                osszetevo_lista_nezet(adatok);
                free(adatok->kezdobetu); // kezdobetu felszabadit
                adatok->kezdobetu = NULL;
            }
            else
                hiba_kodja = 2;
            break;

        case 3: // szoreszlet szerinti kereses
            if (adatok->elso_ossz != NULL)
            {
                adatok->kezdobetu = keresett_szo_beker(); // szoreszlet bekerese
                osszetevo_lista_nezet(adatok);
                free(adatok->kezdobetu); // szoreszlet felszabaditas
                adatok->kezdobetu = NULL;
            }
            else
                hiba_kodja = 2;
            break;

        case 4: // uj osszetevo
            uj_ossz_bekeres(adatok);
            break;

        case 5: // visszalepes
            adatok->fomenu = true;
            break;

        default:
            hiba_kodja = 1;
        }

    }
}


int ossz_hibakod(int hiba_kodja)
{
    switch (hiba_kodja)
    {
    case 1:
        hiba_uzenet("Ervenytelen erteket adott meg!");
        break;
    case 2:
        hiba_uzenet("Nincs megjelenitheto osszetevo!");
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


void ossz_menu_listazas()
{
    econio_textcolor(14);
    printf("Hozzavalok menuje\n\n");
    econio_textcolor(11);

    printf("[1] Hozzavalok ABC sorrendben\n");
    printf("[2] Hozzavalok listazasa kezdobetu alapjan\n");
    printf("[3] Hozzavalok listazasa szo/szoreszlet alapjan\n");
    printf("[4] Uj hozzavalo hozzaadasa\n");
    printf("[5] Visszalepes a fomenube\n");
    printf("\n");
}


void osszetevo_lista_nezet(Listak* adatok)
{
    int oldalszam = 1; // Egyszerre max 10 hozzavalo megjelenitesehez szukseges oldal informaciot tarolja
    bool vissza = false;
    char valaszto;
    bool vege = false;
    int hiba_kodja = 0;
    int sorszam = 0;

    while (!vissza && !(adatok->fomenu) && adatok->elso_ossz != NULL)
    {
        econio_clrscr();

        // Annak megfeleloen, hogy az adatok->kezdobetu milyen informaciot tarol, mashogy listazzuk a hozzavalokat.
        if (adatok->kezdobetu[0] == '0' && adatok->kezdobetu[1] == '\0')
            vege = ossz_abc_listazas(oldalszam, adatok);

        else if (adatok->kezdobetu[1] == '\0' && ((adatok->kezdobetu[0] >= 'a' && adatok->kezdobetu[0] <= 'z') || (adatok->kezdobetu[0] >= 'A' && adatok->kezdobetu[0] <= 'Z')))
            vege = ossz_kezdobetu_listazas(oldalszam, adatok);

        else if (strlen(adatok->kezdobetu) >= 2)
            vege = ossz_szoreszlet_listazas(oldalszam, adatok);

        ossz_lista_menu();

        if (hiba_kodja != 0)
            hiba_kodja = ossz_hibakod(hiba_kodja);

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
                uj_ossz_bekeres(adatok);
                break;

            case 'v':
                vissza = true;
                break;

            case 'f':
                adatok->fomenu = true;
                break;

            case '0':
                if(sorszam> (oldalszam-1)*10 && sorszam <= oldalszam*10 && sorszam <= adatok->ossz_max)
                   ossz_adatok(sorszam, adatok);
                else
                    hiba_kodja = 1;
                break;

            default:
                hiba_kodja = 1;
                break;
            }
    }
}


bool ossz_abc_listazas(int oldalszam, Listak* adatok)
{
    int szamlalo = 1;
    OsszOsszetevo* lepteto = adatok->elso_ossz;

    econio_textcolor(14);
    printf("Hozzavalok %d. oldal\n\n", oldalszam);
    econio_textcolor(11);

    while (lepteto != NULL && szamlalo <= oldalszam*10)
    {
        if (szamlalo > (oldalszam-1)*10)
        {
            printf("[%d] %s\n", szamlalo, lepteto->osszetevo);
        }

        lepteto = lepteto->kov;
        szamlalo += 1;
    }
    printf("\n");

    adatok->ossz_max = szamlalo-1;

    // A NULL ertek jelzi hogy eljutottunk az utolso receptig
    if (lepteto == NULL)
        return true;

    return false;
}


bool ossz_kezdobetu_listazas(int oldalszam, Listak* adatok)
{
    int szamlalo = 1;
    OsszOsszetevo* ossz_lepteto = adatok->elso_ossz;

    econio_textcolor(14);
    printf("A \"%c\"-val/vel kezdodo hozzavalo(k) %d. oldal\n\n", (adatok->kezdobetu)[0], oldalszam);
    econio_textcolor(11);

    // A while ciklus feltetele azert mas mert megnezi, hogy vane-e meg a feltetelnek megfelelo adat a 10. utan.
    // Ha nincs akkor a vegere ertunk, tehat TRUE a visszateres
    while (ossz_lepteto != NULL && szamlalo <= (oldalszam*10)+1)
    {

        if (tolower(ossz_lepteto->osszetevo[0]) == tolower(adatok->kezdobetu[0]))
        {
            if (szamlalo > (oldalszam-1)*10 && szamlalo <= oldalszam*10)
            {
                printf("[%d] %s\n", szamlalo, ossz_lepteto->osszetevo);

            }
            szamlalo++;
        }

        // Ha talaltunk 10+1-ik elemet akkor nem leptetunk tovabb mert lehet a kovetkezo elem NULL lenne
        // es tevesen azt felteteleznenk hogy a vegere ertunk.
        if(szamlalo <= (oldalszam*10)+1)
            ossz_lepteto = ossz_lepteto->kov;
    }

    if (szamlalo == 1)
    {
        hiba_uzenet("Nincs az adott kezdobetuvel kezdodo hozzavalo.");
    }

    adatok->ossz_max = szamlalo-1;

    printf("\n");

    // A NULL ertek jelzi hogy eljutottunk az utolso receptig
    if (ossz_lepteto == NULL)
        return true;

    return false;
}


bool ossz_szoreszlet_listazas(int oldalszam, Listak* adatok)
{
    int szamlalo = 1;
    OsszOsszetevo* ossz_lepteto = adatok->elso_ossz;

    econio_textcolor(14);
    printf("A \"%s\"-t tartalmazo  hozzavalo(k) %d. oldal\n\n", adatok->kezdobetu, oldalszam);
    econio_textcolor(11);

    while (ossz_lepteto != NULL && szamlalo <= (oldalszam*10)+1)
    {

        if (strstr(ossz_lepteto->osszetevo, adatok->kezdobetu) != NULL)
        {
            if (szamlalo > (oldalszam-1)*10 && szamlalo <= oldalszam*10)
            {
                printf("[%d] %s\n", szamlalo, ossz_lepteto->osszetevo);

            }
            szamlalo++;
        }

        if(szamlalo <= (oldalszam*10)+1)
            ossz_lepteto = ossz_lepteto->kov;
    }
    if (szamlalo == 1)
    {
        hiba_uzenet("Nem talalhato megadott szot/reszletet tartalmazo hozzavalo.");
    }

    adatok->ossz_max = szamlalo-1;

    printf("\n");

    // A NULL ertek jelzi hogy eljutottunk az utolso receptig
    if (ossz_lepteto == NULL)
        return true;

    return false;
}


void ossz_lista_menu()
{
    printf("[e] Elore | ");
    printf("[h] Hatra | ");
    printf("[u] Uj hozzavalo | ");
    printf("[v] Vissza | ");
    printf("[f] Fomenu\n");
    printf("\n");
}


void ossz_adatok(int sorszam, Listak* adatok)
{
    OsszOsszetevo * keresett = NULL;

    // Az adatok->kezdobetuben rejlo informacio alapjan kikeressuk a keresett sorszamu hozzavalot.
    if ((adatok->kezdobetu)[0] == '0' && adatok->kezdobetu[1] == '\0')
        keresett = ossz_lista_sorszam_keres(adatok->elso_ossz, sorszam);

    else if(adatok->kezdobetu[1] == '\0' && ((adatok->kezdobetu[0] >= 'a' && adatok->kezdobetu[0] <= 'z') || (adatok->kezdobetu[0] >= 'A' && adatok->kezdobetu[0] <= 'Z')))
        keresett = ossz_lista_kezdobetu_sorszam(adatok->elso_ossz, sorszam, adatok->kezdobetu);

    else if (strlen(adatok->kezdobetu) >= 2)
        keresett = ossz_lista_szoreszlet_sorszam(adatok->elso_ossz, sorszam, adatok->kezdobetu);


    bool vissza = false;
    char valaszto;
    int hiba_kodja = 0;
    int rec_szama = 0;
    char* temp;


    while (!vissza && !(adatok->fomenu))
    {

        econio_clrscr();

        ossz_adatok_megjelenit(keresett, adatok->elso_rec);

        printf("\n");

        printf("[t] Torles | ");
        printf("[v] Vissza | ");
        printf("[f] Fomenu\n");

        printf("\n");

        if (hiba_kodja != 0)
            hiba_kodja = ossz_hibakod(hiba_kodja);

        valaszto = rec_lista_valaszto(&rec_szama);

        switch (valaszto)
        {
        case 'v':
            vissza = true;
            break;

        case 'f':
            adatok->fomenu = true;
            break;

        case 't':
            if (osszetevo_del_kerdes(keresett))
            {
                osszetevo_torles(adatok, keresett);
                vissza = true;
            }
            break;

        case '0':
            if (sorszam > 0 && sorszam <= adatok->ossz_max)
            {
                temp = adatok->kezdobetu; // A hozzavalo keresesehez szukseges karaktert atmenetileg eltarolja.

                adatok->kezdobetu = keresett->osszetevo; // A keresett hozzavalo nevet bementjuk az adatokba

                adatok->rec_max = 0; // A receptek adatok nezetnel ez jelzi majd a fuggvenynek, hogy hozzavalo alapjan keresunk

                recept_adatok(adatok, rec_szama);

                adatok->kezdobetu = temp; // Az adatok kezdobetu taroloja visszakapja az eredetileg tarolt informaciojat
            }
            break;

        default:
            hiba_kodja = 1;
            break;
        }
    }
}


void ossz_adatok_megjelenit(OsszOsszetevo * keresett, ReceptLista* elso_rec)
{

    barna_sarga("Osszetevo");
    printf("%s\n", keresett->osszetevo);

    printf("\n");

    barna_sarga("Recept(ek) ami(k)ben megtalalhato: ");

    // receptek bejarasa es ha van bene ilyen osszetevo akkor kiirja
    ossz_recept_listazas(elso_rec, keresett);

    econio_textcolor(11);
}


void ossz_recept_listazas(ReceptLista* elso_rec, OsszOsszetevo* keresett)
{
    int szamlalo = 0;
    bool talalat;
    ReceptLista* rec_lepteto = elso_rec;

    while (rec_lepteto != NULL)
    {
        talalat = false;

        OsszetevoLista* rec_ossz_lepteto = rec_lepteto->kov_ossz;

        while (rec_ossz_lepteto != NULL && !talalat)
        {
            if (szo_osszehasonlit(rec_ossz_lepteto->ossz_nev, keresett->osszetevo) == 0 )
                {

                    szamlalo++;
                    printf("[%d] %s\n", szamlalo, rec_lepteto->rec_nev);
                    talalat = true;
                }

            rec_ossz_lepteto = rec_ossz_lepteto->kov_ossz;

        }

        rec_lepteto = rec_lepteto->kov_rec;

    }
    if (szamlalo == 0)
        hiba_uzenet("Nem talalhato meg meg egy receptben sem.\n");
}


OsszOsszetevo * ossz_listaeleme_alakit(char *szo)
{
    OsszOsszetevo * uj = (OsszOsszetevo*) malloc (sizeof(OsszOsszetevo));
    if (uj == NULL)
        return NULL;

    uj->kov = NULL;

    char * uj_ossz_nev = (char*) malloc ((strlen(szo)+1)*sizeof(char));
    if (uj_ossz_nev == NULL)
    {
        return NULL;
    }

    strcpy(uj_ossz_nev, szo);
    uj->osszetevo = uj_ossz_nev;

    return uj;
}


void uj_ossz_bekeres(Listak* adatok)
{
    char* uj_ossz = NULL;
    bool mar_szerepel = false;

    do
    {
        econio_clrscr();

        econio_textcolor(6);
        printf("Egy \"x\" megadasaval kilephet az adatok megadasabol.\n");
        econio_textcolor(11);

        printf("\n");


        if (mar_szerepel)
        {
            hiba_uzenet("Ez a hozzavalo mar szerepel a listaban!");
            free(uj_ossz);
        }

        printf("Kerem adja meg az uj hozzavalo nevet: ");

        uj_ossz = hosszu_sor_beolvas();

        if (uj_ossz[0] == 'x' && uj_ossz[1] == '\0')
        {
            free(uj_ossz);
            return;
        }

        mar_szerepel = osszetevo_nev_keres(adatok->elso_ossz, uj_ossz);

    } while (mar_szerepel);

    char mentes_kar;

    do
    {
        econio_clrscr();

        econio_textcolor(6);
        printf("Az uj hozzavalo neve: ");
        econio_textcolor(14);
        puts(uj_ossz);
        econio_textcolor(11);

        printf("\n");

        mentes_kar = mentes();

    } while (mentes_kar != 'i' && mentes_kar != 'n');


    if (mentes_kar == 'i')
    {
        OsszOsszetevo* uj = (OsszOsszetevo*) malloc (sizeof(OsszOsszetevo));
        uj->kov = NULL;

        if (uj == NULL)
        {
            free(uj_ossz);
            return;
        }

        uj->osszetevo = uj_ossz;
        adatok->elso_ossz = ossz_rendezve_beszur(adatok->elso_ossz, uj);
    }

    else
        free(uj_ossz);
}



