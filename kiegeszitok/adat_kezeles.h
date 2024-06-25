#ifndef ADAT_KEZELES_H_INCLUDED
#define ADAT_KEZELES_H_INCLUDED

#include <stdbool.h>
#include "fajl_beolvasas.h"

// Ha az elso pointert eltolja annyira, hogy ne a sor eleji #/SPACE/TAB karaktereken alljon
// Nem dinamikusan foglalt memoria eseten szabad hasznalni!
void kivag(char ** sor);


// A fajlbol beolvasott hozzavalo sor mennyiseg karaktereit valtja at egesz szamma.
int kar_to_szam(char * sor);


// Beolvas EOF-ig vagy enterig egy tetszolegesen hosszu sort.
// Az enter es a # karaktereket nem olvassa be!
char* hosszu_sor_beolvas();


char* kezdobetu_beker();


char* keresett_szo_beker();


// Ket szot osszehasonlit betuik alapjan, a kis es NAGY betuket (ha azonos betuk), azonosnak tekinti.
// Ha az elso kisebb (abc elejehez kozelebbi): -1, ha a masodik kisebb: 1, egyenlok: 0
int szo_osszehasonlit(char * elso, char * masodik);


// Rakerdez a mentesre es egy karaktert kap vissza.
char mentes();




// Az adatok megadasabol valo kilepesre kerdez ra. Csak akkor enged tovabb ha i/n-t kap.
bool kilepes();


// ---------------------Rendezo es kereso fuggvenyek---------------------------


// abc sorrendnek megfeleloen illeszti be a lancolt listaba az elemet
ReceptLista* rec_rendezve_szur(ReceptLista * elso_rec, ReceptLista * uj_rec);


// Recepthez tartozo hozzavalot szurja be rendezve
OsszetevoLista* rec_osszetevo_rendezve_szur(OsszetevoLista* elso_ossz, OsszetevoLista* uj_ossz);


//receptbol beszur
OsszOsszetevo* receptbol_beszur(Listak* adatok);


// Rendezve szurja be
OsszOsszetevo* ossz_rendezve_beszur(OsszOsszetevo* elso_ossz, OsszOsszetevo * uj_ossz);


// Visszaadja a sorszamnak megfelelo receptre mutato pointert
ReceptLista* rec_lista_sorszam_keres(ReceptLista * elso_rec, int sorszam);


// Megkeresi azt a sorszamu elemet, aminek a kezdobetuje a parameterkent kapott betu.
// Olyan szituacioban erdemes meghivni amikor a parameterkent kapott *kezdobetu 1 betubol all.
ReceptLista* rec_lista_kezdobetu_sorszam(ReceptLista* elso_rec, int sorszam, char* kezdobetu);


// Bejarja a recepteket es megnezi van-e talalat a nevben ha van akkor szamolja azt.
// A parameterkent kapott sorszammal egyezo talalatnal visszaadja a talalatot.
ReceptLista* rec_lista_szoreszlet_sorszam(ReceptLista* elso_rec, int sorszam, char* kezdobetu);


// Megkeresi azokat a recepteket amikben az adott hozzavalo pontos egyezessel megtalalhato.
ReceptLista* rec_osszetevo_keres(ReceptLista* elso_rec, int sorszam, char* kezdobetu);


// Olyan receptek kozott keresi meg az adott sorszamut amikben a hozzavlok mind megtalalhatok, mint amiket megadott a fh.
ReceptLista* huto_rec_osszetevo_keres(Listak* adatok, int sorszam, OsszOsszetevo* huto);


// Megkeresi a megadott szo alapjan az egyezo receptet. Visszater egy arra mutato pointerrel.
// Mivel nem lehet ket azonos nevu recept igy el lehet donteni, hogy van-e mar.
ReceptLista* rec_nev_keres(ReceptLista * elso_rec, char * keresett_szo);


OsszOsszetevo* ossz_lista_sorszam_keres(OsszOsszetevo * elso, int sorszam);


OsszOsszetevo* ossz_lista_kezdobetu_sorszam(OsszOsszetevo* elso_ossz, int sorszam, char* kezdobetu);


OsszOsszetevo* ossz_lista_szoreszlet_sorszam(OsszOsszetevo* elso_ossz, int sorszam, char* kezdobetu);


// Megkeresi azt a hozzavalot amive pontos a szonak az egyezese. Visszater a ramutato pointerrel
OsszOsszetevo * osszetevo_nev_keres(OsszOsszetevo* elso_ossz, char * szo);


// ---------------------Torlo es felszabadito fuggvenyek---------------------------


// Rakerdez egy torlendonek jelolt recept tenyleges torlesere. Igen eseten azt az egy elemet felszabaditja.
ReceptLista * rec_torles(ReceptLista * elso_rec, ReceptLista ** torlendo);


// Felszabaditja a receptek listajat es annak minden elemet.
ReceptLista* rec_felszabadit(ReceptLista * elso_rec);


// Egy recept torleset vegzi
ReceptLista* egy_rec_felszabadit(ReceptLista * felszabadit);


// Egy recept torlesere kerdez ra. Csak akkor enged tovabb ha i/n karaktert kap.
bool rec_del_kerdes(ReceptLista* torlendo);


// Egy recept osszes osszetevojet felszabaditja.
OsszetevoLista* rec_ossz_felszabadit(OsszetevoLista* elso_rec_ossz);


// Egy osszetevo elem minden adatat felszabaditja.
OsszetevoLista* egy_ossz_felszabadit(OsszetevoLista* elem);


void osszetevo_torles(Listak* adatok, OsszOsszetevo* keresett);


// A kulon tarolt osszetevokon megy vegig es felszabaditja a memoriat.
void osszetevok_felszabadit(OsszOsszetevo * elso_ossz);


bool osszetevo_del_kerdes(OsszOsszetevo* keresett);






#endif // ADAT_KEZEL_H_INCLUDED
