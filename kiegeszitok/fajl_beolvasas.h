#ifndef FAJL_BEOLVASAS_H_INCLUDED
#define FAJL_BEOLVASAS_H_INCLUDED

#include <stdbool.h>


typedef struct OsszetevoLista {
    char * ossz_nev;
    int mennyiseg;
    char * mertekegyseg;
    struct OsszetevoLista * kov_ossz;
} OsszetevoLista;


typedef struct ReceptLista {
    char * rec_nev;
    OsszetevoLista * kov_ossz;
    char * leiras;
    struct ReceptLista * kov_rec;
} ReceptLista;


typedef struct OsszOsszetevo {
    char * osszetevo;
    struct OsszOsszetevo * kov;
} OsszOsszetevo ;

typedef struct Listak {
    ReceptLista* elso_rec;
    OsszOsszetevo* elso_ossz;
    bool fomenu;
    char* kezdobetu;
    int rec_max;
    int ossz_max;
} Listak;


// A sor eleji #/SPACE/TAB karakterek es ha a vegen van enter akkor anelkul menti el recept nevkent.
void rec_nev_letrehoz(char *sor, ReceptLista * uj);


// Ha az elso receptnev mentesnel nem ertunk volna a sor vegere akkor itt a tovabbi reszt hozzafuzzuk.
void rec_nev_hozzafuz(char * sor, ReceptLista * recept);


// Egy teljes sor hozzavalo adatait szabdalja fel nev, mennyiseg, mertekegyseg adatokra.
void ossz_beolvas(char * sor, OsszetevoLista * osszetevo);


// A sor eleji # es SPACE karakterek es ha a vegen van enter akkor anelkul menti el recept leiraskent.
void leiras_letrehoz(char * sor, ReceptLista * recept);


// Ha nem ertunk a leiras vegere akkor a tovabbi reszleteket hozzafuzi a meglevohoz.
void leiras_hozzafuz(char * sor, ReceptLista * recept);


// Megnyitja a receptek.txt-t es beolvassa a sorokat a megfelelo taroloba menti es az egeszet lancol listaba fuzi.
ReceptLista * rec_fajl_beolvas();


// Megnyitja az osszetevok.txt-t es beolvassa a hozzavalokat es lancolt listaba fuzi oket.
OsszOsszetevo * ossz_fajl_beolvas();


// A receptek.txt-be torteno fajlok kiirasaert felel
void rec_fajlba_kiir(Listak* adatok);


// Az osszetevok kulon menteseert felel.
void ossz_fajlba_kiir(Listak* adatok);

#endif // FAJL_BEOLVASAS_H_INCLUDED
