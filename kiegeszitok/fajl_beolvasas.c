#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fajl_beolvasas.h"
#include "adat_kezeles.h"
#include "osszetevok.h"
#include "debugmalloc.h"
#include "econio.h"
#include "menu.h"





void rec_nev_letrehoz(char *sor, ReceptLista * uj)
{
    kivag(&sor);

    int szohossz = strlen(sor);
    char * nev = NULL;

    if(*sor == '\n')
        return;

    else if (sor[szohossz-1] == '\n')
    {
        nev = (char*) malloc((szohossz) * sizeof(char));
        strncpy(nev, sor, szohossz-1);
        nev[szohossz-1] = '\0';
    }
    else
    {
        nev = (char*) malloc((szohossz+1) * sizeof(char));
        strcpy(nev, sor);
    }

    uj->rec_nev = nev;
}


void rec_nev_hozzafuz(char * sor, ReceptLista * recept)
{
    int sorhossz = strlen(sor);
    char * uj;

    //megszabadulunk a sorvegi entertol
    if (sor[sorhossz-1] == '\n')
    {
        int szohossz = strlen(recept->rec_nev) + sorhossz;
        uj = (char*) malloc ((szohossz) * sizeof (char));
        strcpy(uj, recept->rec_nev);
        strncat(uj, sor, sorhossz-1);
        uj[szohossz-1] = '\0';
    }
    else
    {
        int szohossz = strlen(recept->rec_nev) + sorhossz;
        uj = (char*) malloc ((szohossz+1) * sizeof (char));
        strcpy(uj, recept->rec_nev);
        strcat(uj, sor);
    }

    free(recept->rec_nev);
    recept->rec_nev = uj;
}


void ossz_beolvas(char * sor, OsszetevoLista * osszetevo)
{
    kivag(&sor);
    int sorhossz = strlen(sor);

    if (sor[sorhossz-1] == '\n')
    {
        sor[sorhossz-1] = '\0';
        sorhossz--;
    }

    char* uj_ossz = NULL;
    char* uj_menny = NULL;
    char * uj_mert = NULL;

    int ossz_nev_vege = strcspn(sor, "/");

    if (ossz_nev_vege < sorhossz)
    {
        char * menny_eleje = sor + ossz_nev_vege + 1;
        uj_ossz = (char*) malloc ((ossz_nev_vege+1) * sizeof(char));

        strncpy(uj_ossz, sor, ossz_nev_vege);
        uj_ossz[ossz_nev_vege] = '\0';

        int menny_vege = strcspn(menny_eleje, "/");

        if (menny_vege < sorhossz-ossz_nev_vege-1)
        {
            uj_menny = (char*) malloc ((menny_vege+1) * sizeof(char));

            char * mert_eleje = menny_eleje + menny_vege + 1;

            strncpy(uj_menny, menny_eleje, menny_vege);
            uj_menny[menny_vege] = '\0';

            if(*mert_eleje != '\0')
            {
                uj_mert = (char*) malloc ((strlen(mert_eleje)+1) * sizeof(char));
                strcpy(uj_mert, mert_eleje);
            }

        }

    }

    osszetevo->ossz_nev = uj_ossz;

    if (uj_menny != NULL)
    {
        osszetevo->mennyiseg = kar_to_szam(uj_menny);
        free(uj_menny);
    }

    osszetevo->mertekegyseg = uj_mert;
}


void leiras_letrehoz(char * sor, ReceptLista * recept)
{
    kivag(&sor);

    int szohossz = strlen(sor);
    char * uj_leiras = NULL;

    // Ha kivag fv. utan csak egy enter marad akkor nem mentunk erteket a leirasba.
    if (*sor == '\n')
        return;

    else if (sor[szohossz-1] == '\n')
    {
        uj_leiras = (char*) malloc((szohossz) * sizeof(char));
        strncpy(uj_leiras, sor, szohossz-1);
        uj_leiras[szohossz-1] = '\0';
    }
    else
    {
        uj_leiras = (char*) malloc((szohossz+1) * sizeof(char));
        strcpy(uj_leiras, sor);
    }


    recept->leiras = uj_leiras;
}


void leiras_hozzafuz(char * sor, ReceptLista * recept)
{
    int sorhossz = strlen(sor);
    char * uj_leiras;

    if (sor[sorhossz-1] == '\n')
    {
        int szohossz = strlen(recept->leiras) + sorhossz;
        uj_leiras = (char*) malloc ((szohossz) * sizeof (char));
        strcpy(uj_leiras, recept->leiras);
        strncat(uj_leiras, sor, sorhossz-1);
        uj_leiras[szohossz-1] = '\0';
    }

    else
    {
        int szohossz = strlen(recept->leiras) + sorhossz;
        uj_leiras = (char*) malloc ((szohossz+1) * sizeof (char));
        strcpy(uj_leiras, recept->leiras);
        strcat(uj_leiras, sor);
    }

    free(recept->leiras);
    recept->leiras = uj_leiras;
}


ReceptLista * rec_fajl_beolvas()
{
    // Adatokat tartalmazó txt megnyitása
    FILE * fptr;
    fptr = fopen("./TXT/receptek.txt", "r");
    if (fptr == NULL)
    {
        return NULL;
    }

    ReceptLista * elso_recept = NULL;
    ReceptLista * uj_rec = NULL;

    // Txt-ben található sorok beolvasása
    char sor[51];
    while (fgets(sor, 51, fptr))
    {


        // A recept nevét a sorkezdõ 1 db # jelzi
        if (sor[0] == '#' && sor[1] != '#')
        {
            int sorhossz = strlen(sor);

            uj_rec = (ReceptLista*) malloc (sizeof(ReceptLista));

            if (uj_rec == NULL)
                return NULL;

            uj_rec->kov_ossz = NULL;
            uj_rec->kov_rec = NULL;
            uj_rec->leiras = NULL;
            uj_rec->rec_nev = NULL;

            rec_nev_letrehoz(sor, uj_rec);

            // Ha nem értünk a a sor végére akkor hozzáfûzzük a további elemeket.
            while (sor[sorhossz-1] != '\n' && fgets(sor, 51, fptr))
            {
                rec_nev_hozzafuz(sor, uj_rec);
                sorhossz = strlen(sor);
            }

            // Az uj_recept listaba futese de csak akkor ha a recept neve nem NULL
            if (uj_rec->rec_nev != NULL)
            {
                if (elso_recept == NULL)
                    elso_recept = uj_rec;

                else
                elso_recept = rec_rendezve_szur(elso_recept, uj_rec);
            }
            else
            {
                free(uj_rec);
                uj_rec = NULL;
            }


        }

        // osszetevo sor beolvasasa
        else if (sor[1] == '#' && sor[2] != '#' && uj_rec != NULL)
        {
            int szohossz = strlen(sor);

            char * teljes_sor = (char*) malloc ((szohossz+1)*sizeof(char));

            if (teljes_sor == NULL)
                return NULL;

            strcpy(teljes_sor, sor);

            // Ha hosszabb mint 50 karakter a sor akkor hozzafuzi az enterig a tobbi reszletet.
            while (sor[szohossz-1] != '\n' && fgets(sor, 51, fptr))
            {
                int teljes_sor_hossz = strlen(teljes_sor);
                szohossz = strlen(sor);

                char * temp = (char*) malloc ((teljes_sor_hossz+szohossz+1)*sizeof(char));

                // Hibas memoria foglalas eseten felszabaditja az eddig lefoglalt memoriat es NULL ertekkel ter vissza.
                if (temp == NULL)
                {
                    rec_felszabadit(elso_recept);
                    free(teljes_sor);
                    return NULL;
                }

                // Uj sor osszefuzese a regivel es a pointer atallitasa.
                strcpy(temp, teljes_sor);
                strcat(temp, sor);

                free(teljes_sor);

                teljes_sor = temp;
            }


            OsszetevoLista * uj_ossz = (OsszetevoLista*) malloc(sizeof(OsszetevoLista));

            uj_ossz->kov_ossz = NULL;
            uj_ossz->ossz_nev = NULL;
            uj_ossz->mennyiseg = -1;
            uj_ossz->mertekegyseg = NULL;

            ossz_beolvas(teljes_sor, uj_ossz);

            free(teljes_sor);

            if (uj_ossz->ossz_nev != NULL && uj_ossz->mennyiseg != -1 && uj_ossz->mertekegyseg != NULL)
                uj_rec->kov_ossz = rec_osszetevo_rendezve_szur(uj_rec->kov_ossz, uj_ossz);
            else
                egy_ossz_felszabadit(uj_ossz);

        }

        // Leiras soranak beolvasasa
        else if (sor[2] == '#' && uj_rec != NULL)
        {
            if (uj_rec->leiras == NULL)
            {
                leiras_letrehoz(sor, uj_rec);
                int sorhossz = strlen(sor);
                while (sor[sorhossz-1] != '\n' && fgets(sor, 51, fptr))
                {
                    leiras_hozzafuz(sor, uj_rec);
                    sorhossz = strlen(sor);
                }
            }
        }
    }

    fclose(fptr);
    return elso_recept;
}


OsszOsszetevo * ossz_fajl_beolvas()
{
    FILE * fptr;
    fptr = fopen("./TXT/hozzavalok.txt", "r");

    if (fptr == NULL)
        return NULL;

    OsszOsszetevo * elso_ossz = NULL;

    char sor[11];
    char* reszlet;


    while (fgets(sor, 11, fptr))
    {
        bool sorvege = false;
        if (sor[0] == '#' && sor[1] == '#')
        {
            // Sor elejen talalhato #/SPACE/TAB karakterek torlese
            int i;
            for (i = 0; (sor[i] == '#' || sor[i] == ' ' || sor[i] == '\t') && sor[i] != '\n'; i++);
            reszlet = sor+i;

            if (*reszlet != '\n')
            {
                OsszOsszetevo * uj = (OsszOsszetevo*) malloc (sizeof(OsszOsszetevo));
                if (uj == NULL)
                    return NULL;

                uj->kov = NULL;

                int szohossz = strlen(reszlet);

                if (reszlet[szohossz-1] == '\n')
                {
                    reszlet[szohossz-1] = '\0';
                    szohossz--;
                    sorvege = true;
                }

                char * uj_ossz_nev = (char*) malloc ((szohossz+1)*sizeof(char));
                strcpy(uj_ossz_nev, reszlet);

                uj->osszetevo = uj_ossz_nev;

                while(!sorvege && fgets(sor,11, fptr))
                {
                    int ossz_nev_hossz = strlen(uj_ossz_nev);
                    szohossz = strlen(sor);

                    if (sor[szohossz-1] == '\n')
                    {
                        sor[szohossz-1] = '\0';
                        szohossz--;
                        sorvege = true;
                    }



                    char * uj_ossz_nev = (char*) malloc ((ossz_nev_hossz+szohossz+1)*sizeof(char));

                    strcpy(uj_ossz_nev, uj->osszetevo);
                    strcat(uj_ossz_nev, sor);

                    free(uj->osszetevo);

                    uj->osszetevo = uj_ossz_nev;
                }
                if (elso_ossz != NULL && osszetevo_nev_keres(elso_ossz, uj_ossz_nev) != NULL)
                {
                    free(uj->osszetevo);
                    free(uj);
                }
                else
                    elso_ossz = ossz_rendezve_beszur(elso_ossz, uj);
            }
        }
    }

    fclose(fptr);
    return elso_ossz;
}


void rec_fajlba_kiir(Listak* adatok)
{
    FILE* fptr;
    fptr = fopen("./TXT/receptek.txt", "w");

    if (fptr == NULL)
        return;

    ReceptLista* rec_lepteto = adatok->elso_rec;

    while (rec_lepteto != NULL)
    {
        OsszetevoLista* ossz_lepteto = rec_lepteto->kov_ossz;

        fprintf(fptr, "# %s\n", rec_lepteto->rec_nev);
        while(ossz_lepteto != NULL)
        {
            fprintf(fptr, "## %s/%d/%s\n", ossz_lepteto->ossz_nev, ossz_lepteto->mennyiseg, ossz_lepteto->mertekegyseg);

            ossz_lepteto = ossz_lepteto->kov_ossz;
        }

        fprintf(fptr, "### %s\n", rec_lepteto->leiras);
        fprintf(fptr, "***\n");

        rec_lepteto = rec_lepteto->kov_rec;
    }
    fclose(fptr);
}


void ossz_fajlba_kiir(Listak* adatok)
{
    FILE* fptr;

    fptr = fopen("./TXT/hozzavalok.txt", "w");

    if (fptr == NULL)
        return;


    OsszOsszetevo* ossz_lepteto = adatok->elso_ossz;

    while (ossz_lepteto != NULL)
    {
        fprintf(fptr, "## %s\n", ossz_lepteto->osszetevo);
        ossz_lepteto = ossz_lepteto->kov;
    }

    fclose(fptr);
}

