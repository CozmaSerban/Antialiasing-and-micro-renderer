Titlu : Antialiasing si micro renderer
Author : Daniel - Serban Cozma
Grupa : 334AA

homework.c

blackwhite** alocareMatrice(int h, int w)
- functie e alocare pentru matricea de alb/negru (aloc matricea linie cu linie)

unsigned char* alocareMatriceColor(int h, int w)
- functie de alocare pentru vectorul in care salvez imaginea color

void readInput(const char * fileName, image *img) {
-citesc date de header si in functie de ce tip de poza e fac alocarea spatiului de incarcare a pozei
 si citesc datele din fisier(intr-o matrice linie cu linie pentru matricea alb/negru si o singura data toata imaginea pentru imaginea color)

void writeData(const char * fileName, image *img) 
-functia de scriere a datelor;
-deschid fisier si in functie de tipul pozei scriu din matrice linie cu linie (imaginea alb/negru) sau toata imaginea de o data (imaginea color)


int min(int a, int b)
-functie de minim pentru calcul formle threaduri

void* BW(void * var)
-functie in care impart matricea in matrice mai mici si le procesez intr-un pixel in functie de resize factor
-paralelizez pe inaltime
-cu k si l parcurg matricele mai mici

void* COLOR(void * var)
-functie in care interpretez vectorul ca pe o matrice si il parcurg ca si cum as imparti matricea in matrice mai mici si procesez pixeli rezultand
 cate un pixel din matricea finala.


void resize(image *in, image * out) 
-functia in care determin dimensiunile imaginii de out;
-incep paralelizarea;

Observatii (Discrepanta dintre modul de rezolvare a imaginii Alb/Negru si cea Color) :
- module de abordare al problemei este diferit fiindca azi, duminica (28 octombrie), dupa-amiaza am urcat solutia si am observat ca imi pica si earth_color dupa urcari repetate (in decursul mai multor zile);
- pana in momentul respectiv pica numai city_color ( lucru pe care l-am mentionat si pe forum )
- am vazut ca imi zicea faptul ca programul dura mai mult de 8 secunde la un test de earth_color si apoi mai dadea fail la cateva teste de la earth_color.
- parea sa nu fie de la I/O ca nu pica city_bw si mai erau si testele de la earth_color care nu aveau 8 secunde si tot picau
- M-am gandit sa incerc sa incarc imaginea intr-un vector si totusi sa o tratez ca o matrice.
- Am incercat asta stergand structura de data color si facand cateva modificari in alocare si apoi in functia de thread COLOR.


homework1.c


blackwhite** alocareMatrice(int w)
- functie de alocare a matricei pentru randare
- in functie fac si toti pixeli albi

void initialize(image *im) 
- setez rezolutia imaginii in header
- apelez functia de alocare

int min(int a, int b)
- functie de minim pentru calcule threaduri

void* THREAD(void * var)
- functie in care parcurg matricea si in momentul in care respecta conditia (distanta de la o dreapta la un punct) fac pixelul negru
- parcurg de la stanga jos si paralelizez pe coloane


void render(image *im) 
- incep paralelizarea randarii


void writeData(const char * fileName, image *img) 
-functia de scriere a datelor;
-deschid fisier si in functie de tipul pozei scriu din matrice linie cu linie.
