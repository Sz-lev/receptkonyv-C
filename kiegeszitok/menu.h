#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED


#include "fajl_beolvasas.h"
#include <stdbool.h>

// Megjeleniti a fomenut
void menu_megjelenit();


// Bekeri a felhasznalotol a vezerleshez szukseges szamot.
int menu_valaszto();

// Fomenuben a lehetseges hibakat jeleniti meg a terminalon.
// 1: Ervenytelen ertek, 2: nincs megjelenitheto recept.
int fomenu_hibakod(int hiba_kodja);

// A program futasanak fuggvenye.
// Itt allithato be a terminal neve.
// Itt olvassa be a program a receptek.txt tartalmát és menti el lancolt listaba.
// Amig az open valtozo erteke true addig a program fut. Ha false erteket kap, akkor felszabaditjuk a memoriat es kilepunk a programbol.
void futtatas();


// Veletlenszeruen kivalaszt egy receptet es megjeleniti az adatait
void nincs_otletem(Listak* adatok);


// Tobb osszetevot ki lehet valasztani es annak megfelelo recepteket megjeleniti.
void el_kell_hasznalni(Listak* adatok);


// A kivalasztott sorszamu hozzavalot hozzaadja egy osszetevo listahoz.
OsszOsszetevo* hutohoz_ad(OsszOsszetevo* huto_elso, Listak* adatok, int sorszam);


// Az el kell hasznalni funkciohoz tartozik es a receptek listazott nezetenek menujet valositja meg.
void huto_rec_lista_nezet(Listak* adatok, OsszOsszetevo* huto);


// Tobb osszetevonek megfelelo recepteket listazza oldalszamnak megfeleloen
// rec_max-ot beallitja
bool huto_rec_abc_listazas(int oldalszam, Listak* adatok, OsszOsszetevo* huto);


// Megkeresi a sorszamnak megfelelo receptet, ami tartalmazza az el kell hasznalni funkcio osszetevoit
void huto_rec_adatok(Listak* adatok, int sorszam, OsszOsszetevo* huto);


#endif // MENU_H_INCLUDED
