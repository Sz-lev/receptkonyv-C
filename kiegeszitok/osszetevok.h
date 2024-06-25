#ifndef OSSZETEVOK_H_INCLUDED
#define OSSZETEVOK_H_INCLUDED

#include <stdbool.h>
#include "fajl_beolvasas.h"

// Az osszetevok fomenujet tartalmazza
void osszetevo_menu(Listak* adatok);

// Az osszetevokhoz tartozo hibauzeneteket tartalmazza
// 1: ervenytelene adat, 2: nincs recept, 3/4 nincs tovabbi oldal
int ossz_hibakod(int hiba_kodja);


// Osszetevok almenu opcioinak listazasa
void ossz_menu_listazas();


// Bejarja es kiirja a kulon osszetevok listajat.
void osszetevo_lista_nezet(Listak* adatok);


// Mivel abc sorrendben vannak beszurva az adatok igy csak listazni kell
// Az oldalszamnak megfelelo max 10 hozzavalot listazza. Ha a lista vegere er (tehat nincs tobb osszetevo) akkor true ertekkel ter vissza.
bool ossz_abc_listazas(int oldalszam, Listak* adatok);


// kezdobetu szerint listazza a hozzavalokat
// Csak 1 betut lehet neki megadni
bool ossz_kezdobetu_listazas(int oldalszam, Listak* adatok);

// Szoreszlet alapjan listazza a hozzavalokat
// A kis es nagy betuk kulonbseget okoznak!
// min 2 karaktert kell neki megadni
bool ossz_szoreszlet_listazas(int oldalszam, Listak* adatok);


// Az osszetevo lista nezetenek vezerlesi kodjait listazza
void ossz_lista_menu();


// Hozzavalo adatainak a menujet vezerli.
void ossz_adatok(int sorszam, Listak*adatok);


// hozzavalo adatainak listazasaert felel.
void ossz_adatok_megjelenit(OsszOsszetevo * keresett, ReceptLista* elso_rec);


// Azokat a recepteket listazza amikben az adott hozzavalo megtalalhato
void ossz_recept_listazas(ReceptLista* elso_rec, OsszOsszetevo* keresett);


// Egy megadott szot alakit osszetevo lancoltlista elemme.
OsszOsszetevo * ossz_listaeleme_alakit(char *szo);


void uj_ossz_bekeres(Listak* adatok);


#endif // OSSZETEVOK_H_INCLUDED
