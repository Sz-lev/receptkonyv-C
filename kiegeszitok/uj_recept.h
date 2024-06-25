#ifndef UJ_RECEPT_H_INCLUDED
#define UJ_RECEPT_H_INCLUDED

#include "fajl_beolvasas.h"


// Egy recepthez tartozo adatokat keri be segedfuggvenyek segitsegevel.
// Ha minden adat megfelel akkor rendezve beszurja a tobbi recept koze.
void ujrec_adat_bekeres(Listak* adatok);


// Beker egy uj recept nevet a felhasznalotol. Ha egy "x" karaktert kap akkor kilep.
// Ha letezik a recept neve akkor uj nevet ker be. Vegen rakerdez, hogy szeretnenk-e menteni.
// Mentes eseten visszater a szoval.
char* ujrec_nev_beker(ReceptLista * elso_rec);


// Egy uj recepthez tartozo hozzavalok bekereset vezerli.
OsszetevoLista * ujrec_osszetevok_lista(char * recept, Listak* adatok);


int ujrec_hibakod(int hiba_kodja);


// uj recepthez hozzavalok bekeresenek vezerlesi kodjait tartalmazza
void ujrec_ossz_lista_menu();


// egy recepthez uj hozzavalo adatait keri be es menti bele.
OsszetevoLista* ujrec_ossz_hozzaadas(OsszetevoLista* elso_rec_ossz, OsszOsszetevo* elso_ossz, int sorszam);


// Egy tetszolegesen hosszu sort olvas be leiraskent.
char* ujrec_leiras_beker(ReceptLista* elem);


// Egy recept nevenek modositasaert felel
void recnev_modosit(Listak* adatok, ReceptLista* keresett);


// Egy recept osszetveoinek modositas menujet vezerli
void rec_ossz_modosit_menu(Listak* adatok, ReceptLista* keresett);


// Egy recept hozzavaloit a funkcionak (felulir, modosit, torol) megfeleloen modosit
void rec_ossz_modositas(Listak* adatok, ReceptLista* keresett, char funkcio);


// Egy recept leirasat modositja
void rec_leiras_modosit(Listak* adatok, ReceptLista* keresett);


#endif // UJ_RECEPT_H_INCLUDED
