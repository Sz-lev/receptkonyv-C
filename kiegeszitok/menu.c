#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "osszetevok.h"
#include "econio.h"
#include "adat_kezeles.h"
#include "receptek.h"
#include "menu.h"
#include "kinezet.h"
#include "debugmalloc.h"


void menu_megjelenit()
{
    sarga_kek("Fomenu");

    printf("\n");

    printf("[1] Receptek\n");
    printf("[2] Hozzavalok\n");
    printf("[3] Nincs otletem\n");
    printf("[4] El kell hasznalni\n");
    printf("[5] Kilepes\n");
    printf("\n");
}


int menu_valaszto()
{
    int bekert = 0;
    printf("Kerem valasszon az opciok kozul: ");

    scanf("%d", &bekert);

    while(getchar() != '\n');

    return bekert;
}


int fomenu_hibakod(int hiba_kodja)
{
    switch(hiba_kodja)
    {
    case 1:
        hiba_uzenet("Ervenytelen erteket adott meg!");
        break;
    case 2:
        hiba_uzenet("Nem talalhato megjelenitheto recept!");
        break;
    }

    return 0;
}


void futtatas()
{
    econio_set_title("Receptes konyv"); // Terminál címének beállítása

    // letrehozzuk az adatokat tarolo strukturat.
    Listak adatok;
    adatok.elso_rec = NULL;
    adatok.elso_ossz = NULL;
    adatok.fomenu = false;
    adatok.kezdobetu = "0";
    adatok.ossz_max = 0;
    adatok.rec_max = 0;

    // Receptek.txt beolvasása és láncolt listába szervezése. Láncolt listára mutató pointer létrehozása.
    adatok.elso_rec = rec_fajl_beolvas();

    // Osszetevok beolvasa es lancolt listaba szervezese
    adatok.elso_ossz = ossz_fajl_beolvas();

    // Ha lenne olyan osszetevo a receptekben ami nem lenne a hozzavalok listajaban akkor azt is atmenti.
    adatok.elso_ossz = receptbol_beszur(&adatok);

    srand(time(NULL));

    bool fut = true;    // A program futásáért felelõs logikai változó
    int valasztott;
    int hiba_kodja = 0;

    //futtatásért felelõs ciklus
    while(fut)
    {
        adatok.fomenu = false;  // Ha visszakerultunk a fomenube, akkor visszaallitjuk false-ra

        econio_clrscr();

        econio_textcolor(6);
        fejlec();
        econio_textcolor(11);

        menu_megjelenit();

        if (hiba_kodja != 0)
            hiba_kodja = fomenu_hibakod(hiba_kodja);

        valasztott = menu_valaszto();   // Egy szamot ker be

        switch (valasztott)
        {

        case 1: // Receptek fomenu
            recept_menu(&adatok);
            break;

        case 2: // osszetevok fomenu
            osszetevo_menu(&adatok);
            break;

        case 3: // nincs otletem
            if (adatok.elso_rec != NULL)
            {
                nincs_otletem(&adatok);
            }
            else
                hiba_kodja = 2;
            break;

        case 4: // el kell hasznalni
            if (adatok.elso_rec != NULL)
            {
                el_kell_hasznalni(&adatok);
            }
            else
                hiba_kodja = 2;
            break;

        case 5: // kilepes
            fut = false;
            break;

        default:    // Ha egyik lehetosegnek sem felel meg a kapott ertek, akkor hibakod keletkezik.
            hiba_kodja = 1;
            break;
        }
    }

    // A meglevo recept adatok kiirasa egy txt fajlba
    rec_fajlba_kiir(&adatok);

    // A meglevo osszetevok kiirasa egy txt-be
    ossz_fajlba_kiir(&adatok);

    // Ha van mit felszabaditani akkor felszabaditjuk a dinamikusan foglalt memoriat.
    if (adatok.elso_ossz != NULL)
        osszetevok_felszabadit(adatok.elso_ossz);

    adatok.elso_ossz = NULL;

    if (adatok.elso_rec != NULL)
        rec_felszabadit(adatok.elso_rec);

    adatok.elso_rec = NULL;
}


void nincs_otletem(Listak* adatok)
{
    int szamlalo = 0;
    int sorszam;

    ReceptLista* lepteto = adatok->elso_rec;

    // A receptek maximalis szamat adja meg
    while (lepteto != NULL)
    {
        szamlalo++;
        lepteto = lepteto->kov_rec;
    }

    sorszam = (rand() % szamlalo) +1;

    // A generalt random szamnak megfelelo sorszamu receptet jeleniti meg.
    adatok->kezdobetu = "0";
    recept_adatok(adatok, sorszam);
}


void el_kell_hasznalni(Listak* adatok)
{
    int oldalszam = 1;
    bool vissza = false;
    char valaszto;
    bool vege = false;
    int hiba_kodja = 0;

    int sorszam = 0;

    OsszOsszetevo* huto = NULL;

    while (!vissza && !adatok->fomenu)
    {
        econio_clrscr();

        vege = ossz_abc_listazas(oldalszam, adatok);

        econio_textcolor(6);
        printf("Eddig megadott hozzavalok:");

        if(huto != NULL)
        {
            econio_textcolor(14);

            OsszOsszetevo* lepteto = huto;

            while (lepteto->kov != NULL)
            {
                printf(" %s,", lepteto->osszetevo);
                lepteto = lepteto->kov;
            }
            printf(" %s", lepteto->osszetevo);
        }

        econio_textcolor(11);

        printf("\n\n");

        printf("[e] Elore | ");
        printf("[h] Hatra | ");
        printf("[t] Torles | ");
        printf("[v] Vissza | ");
        printf("[k] Kereses\n");

        printf("\n");

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

            case 't':
                if (huto != NULL)
                {
                    osszetevok_felszabadit(huto);
                    huto = NULL;
                }
                break;

            case 'v':
                vissza = true;
                if (huto != NULL)
                    osszetevok_felszabadit(huto);
                break;

            case '0':
                if(sorszam> (oldalszam-1)*10 && sorszam <= oldalszam*10 && sorszam <= adatok->ossz_max)
                {
                    huto = hutohoz_ad(huto, adatok, sorszam);
                }

                else
                    hiba_kodja = 1;
                break;

            case 'k':
                huto_rec_lista_nezet(adatok, huto);
                if (adatok->fomenu)
                    osszetevok_felszabadit(huto);
                break;

            default:
                hiba_kodja = 1;
                break;
            }
    }
}


OsszOsszetevo* hutohoz_ad(OsszOsszetevo* huto_elso, Listak* adatok, int sorszam)
{
    // sorszamnak megfelelo adat kikerese az adatok listabol
    OsszOsszetevo* keresett = ossz_lista_sorszam_keres(adatok->elso_ossz, sorszam);

    if (osszetevo_nev_keres(huto_elso, keresett->osszetevo) == NULL)
    {
        OsszOsszetevo* uj = ossz_listaeleme_alakit(keresett->osszetevo);
        if (uj == NULL)
            return huto_elso;

        huto_elso = ossz_rendezve_beszur(huto_elso, uj);
    }

    return huto_elso;
}


void huto_rec_lista_nezet(Listak* adatok, OsszOsszetevo* huto)
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

        vege = huto_rec_abc_listazas(oldalszam, adatok, huto);

        printf("[e] Elore | ");
        printf("[h] Hatra | ");
        printf("[v] Vissza | ");
        printf("[f] Fomenu");
        printf("\n\n");

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

            case 'v':
                vissza = true;
                break;

            case 'f':
                adatok->fomenu = true;
                break;

            case '0':
                if (sorszam > (oldalszam-1)*10 && sorszam <= oldalszam*10 && sorszam <= adatok->rec_max)
                    huto_rec_adatok(adatok, sorszam, huto);
                else
                    hiba_kodja = 1;
                break;

            default:
                hiba_kodja = 1;
                break;
            }


    }
}


bool huto_rec_abc_listazas(int oldalszam, Listak* adatok, OsszOsszetevo* huto)
{
    int szamlalo = 1;
    bool talalat = false;
    bool tovabb = false;

    ReceptLista* rec_lepteto = adatok->elso_rec;

    // max az oldalszamnak megfelelo receptig leptetjuk
    while(rec_lepteto != NULL && szamlalo <= oldalszam*10)
    {
        tovabb = false;

        OsszOsszetevo* huto_lepteto = huto;

        while(huto_lepteto != NULL && !tovabb)
        {
            talalat = false;

            OsszetevoLista* rec_ossz_lepteto = rec_lepteto->kov_ossz;

            while(rec_ossz_lepteto != NULL && !talalat)
            {
                // Ha megtalaltuk az adott hozzavalot, akkor a tobbit nem kell nezni. Lephetunk a kovetkezore.
                if (szo_osszehasonlit(rec_ossz_lepteto->ossz_nev, huto_lepteto->osszetevo) == 0)
                    talalat = true;

                rec_ossz_lepteto = rec_ossz_lepteto->kov_ossz;
            }

            // Ha eljutunk a recept osszetevoinek vegere es nem votl egyezes akkor ez a recept nem jo. Lepunk tovabb.
            if(rec_ossz_lepteto == NULL && !talalat)
                tovabb = true;

            huto_lepteto = huto_lepteto->kov;
        }

        // Ha a huto tartalmat bejartuk es minden egyezett akkor kiirjuk es a szamlalot noveljuk
        if(huto_lepteto == NULL && !tovabb)
        {
            if (szamlalo > (oldalszam-1)*10)
                printf("[%d] %s\n", szamlalo, rec_lepteto->rec_nev);
            szamlalo++;
        }
        rec_lepteto = rec_lepteto->kov_rec;
    }


    if(szamlalo == 1)
    {
        hiba_uzenet("Nincs megjelenitheto recept.");
    }

    printf("\n");

    // Ebben a modban talalt receptek max szama. Szukseges lesz a sorszam valasztasnal
    adatok->rec_max = szamlalo-1;

    if (rec_lepteto == NULL)
        return true;

    return false;
}


void huto_rec_adatok(Listak* adatok, int sorszam, OsszOsszetevo* huto)
{
    // Megkeressuk a feltetelnek es sorszamnak megfelelo receptet.
    ReceptLista* keresett = huto_rec_osszetevo_keres(adatok, sorszam, huto);

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

