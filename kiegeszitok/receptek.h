#ifndef RECEPTEK_H_INCLUDED
#define RECEPTEK_H_INCLUDED

#include <stdbool.h>
#include "fajl_beolvasas.h"

void recept_menu(Listak* adatok);


void rec_menu_listazas();


int rec_menu_hibauzenet(int hibakod);


// Recept listazott nezetnek a mukodtetese itt megy.
void rec_lista_nezet(Listak* adatok);


// Oldalszamnak megfelelo 10 recept nevet irja ki
// Visszateresi ertekevel jelzi ha elertuk a lista veget
bool rec_abc_listazas(int oldalszam, Listak* adatok);


bool rec_kezdobetu_listazas(int oldalszam, Listak* adatok);


bool rec_szoreszlet_listazas(int oldalszam, Listak* adatok);


// listazott nezet vezerlesi kodjait irja ki
void rec_lista_menu();


// listazott nezet vezerlesi kodjat keri be
char rec_lista_valaszto(int* sorszam);


// Recept adat nezet menujet valositja meg
void recept_adatok(Listak* adatok, int sorszam);


// Megjeleniti egy recept adatait
void recept_adatok_megjelenit(ReceptLista * keresett);


// bejarja egy recepthez tartozo osszetevok listajat es megjeleniti: "hozz. (menny. mert.)" formaban
void rec_osszetevo_bejaras(OsszetevoLista * elso);


// recept adat nezet menujet irja ki
void recept_adatok_menu();


void rec_modositas_menu(Listak* adatok, ReceptLista* keresett);












#endif // RECEPTEK_H_INCLUDED
