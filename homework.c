#include "homework.h"
#include "stdlib.h"
#include "stdio.h"
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <math.h>
int num_threads;
int resize_factor;


blackwhite** alocareMatrice(int h, int w){ //functie de alocare matrice pentru imagine rezultata Alb-Negru

    blackwhite** test;
    int i;

    test = (blackwhite **) malloc (sizeof (blackwhite *) * h); //Aloc imginea ca o matrice avad height si witdh cele noi (impartite la resize_factor)

	for (i = 0; i < h; i++) {

		test[i] = (blackwhite *) malloc (sizeof (blackwhite) * w);

	}

    return test;

}

unsigned char* alocareMatriceColor(int h, int w){ // functie de alocare matrice pentru imagine Cclor
        unsigned char * test;


    test = (unsigned char *) malloc (3 * w *h); //aloc totul ca un sir de bytes inmultesc cu 3 ca sunt (r, g, b)

    return test;


}

void readInput(const char * fileName, image *img) {
        FILE *read;
        int i;

        char first;
        read = fopen(fileName, "rb");   //Deschid fisierul in mod binar

    fscanf(read, "%c",&first);
    fscanf(read, "%c",&first);          
    (*img).type = (int)(first - '0');   // citesc tipul de imagine si fac valoarea '6' sau '5' un int facand scadere cu '0'
    
    fscanf(read, "%d", &(*img).width);
    fscanf(read, "%d", &(*img).height);
    fscanf(read, "%d", &(*img).max);

    // daca e valoarea 5 la tip inseamna ca poza e alb negru 
    if((*img).type == 5){
    (*img).imagbl = alocareMatrice((*img).height, (*img).width); // apelez functie de alocare
   
    fscanf(read, "%c",&first); // este un caracter in plus intre datele citite si pixeli din imagine si scap de el

    for(i = 0; i < (*img).height; i++){
        fread((*img).imagbl[i], sizeof(blackwhite), (*img).width, read); // citesc rand cu rand pixeli din imaginea Alb/Negru
    }

    }
    // daca e valorea 6 la tip inseamna ca poze e color
    if((*img).type == 6){
            (*img).imag = alocareMatriceColor((*img).height, (*img).width); //apelez functia de alocare

            fscanf(read, "%c",&first); //citesc caracterul in plus dintre info despre imagine si pixeli

            fread((*img).imag,  3* (*img).height * (*img).width,1, read); // citesc toti pixeli din imagine dintr-o data
    
    }
    
    fclose(read); // inchid fisierul


}

void writeData(const char * fileName, image *img) {
      
      
    FILE *write;

    
    int i ;
    if((*img).type == 5){ // daca e de tipul 5 deschid fisier binar 
    write = fopen(fileName,"wb");
    fprintf(write,"P%d\n%d %d\n%d\n",(*img).type,(*img).width,(*img).height,(*img).max); // scriu informatii despre imagine
    
    for(i = 0; i < (*img).height; i++){
        fwrite((*img).imagbl[i],sizeof(blackwhite), (*img).width, write); // scriu rand cu rand pixeli din poza
    }
     fclose(write); // inchid fisier
    }
    
    if((*img).type == 6){ // daca e tip color deschid fisier binar
    write = fopen(fileName,"wb");
    fprintf(write,"P6\n%d %d\n%d\n",(*img).width,(*img).height,(*img).max); // scriu info despre poza
    
   
    
            fwrite((*img).imag, 1, 3* (*img).width * img->height, write); // scriu toti pixeli din imagina dintr-o data
    
    fclose(write); // inchid fisier
    }

    

}
image *in2,*out2; // pentru usurinta creez niste referinte globale la image in si image out ca sa le accesez mai usor in BW si COLOR 

int min(int a, int b){ // functie de min pentru calculul lui first si second din functiile BW si COLOR

	return (a < b)? a : b;
} 

void* BW(void * var)
{ 
	int thread_id = *(int*)var;
	int first = thread_id * ceil(((double)out2->height/num_threads));
	int second = min((thread_id + 1) * ceil(((double)out2->height/num_threads)),out2->height);



    int i,k,j,l;
    int aux_pixel = 0;
    int kg[3][3] =  {
					{1, 2, 1}, 
					{2, 4, 2}, // declar matricea Kernel Gaussian
					{1, 2, 1}
					};

            for(i = first; i < second; i++){ // paralelizez pe inaltime
                for(j = 0; j < out2->width; j++){
                    
                    //impart matricea mare in matrice mai mici de resize_factor x resize_factor si fac operatiile 
                    // pentru a creea noul pixel in functie de resize_factor (daca e 3 sau nu)
                    for(k = i * resize_factor; k < (i+1)*resize_factor; k++){ 
                        for(l = j * resize_factor; l < (j+1)*resize_factor; l++){
                            if(resize_factor == 3){
                            aux_pixel += (*in2).imagbl[k][l].a * (kg[k%3][l%3]);
                            }
                            else{
                            aux_pixel += (int)((*in2).imagbl[k][l].a);
                            }
                        }
                    }

                    if(resize_factor == 3){
                        (*out2).imagbl[i][j].a = (unsigned char)(aux_pixel/16) & 255;
                    }
                    else{
                        (*out2).imagbl[i][j].a = (unsigned char)(aux_pixel/(resize_factor * resize_factor))& 255;
                    }
                    aux_pixel = 0;
                }

            }

    return NULL;
}

void*  COLOR(void * var){
    int green=0,blue=0,red=0;

    int thread_id = *(int*)var;
   
    int first = thread_id * ceil(((double)out2->height/num_threads));
    int second = min((thread_id + 1) * ceil(((double)out2->height/num_threads)),out2->height);
    
    int i,k,j,l;

    int kg[3][3] =  {
					{1, 2, 1}, 
					{2, 4, 2},// declar matricea Kernel Gaussian
					{1, 2, 1}
					};

            for(i = first; i < second ; i++){ // paralelizez pe inaltime 
                for( j = 0; j < out2->width; j++){

                    //impart vectorul pe care il interpretez ca o matrice mare in matrice mai mici de resize_factor x resize_factor si fac operatiile 
                    // pentru a creea noul pixel in functie de resize_factor (daca e 3 sau nu)
                    for(k = 0; k < resize_factor; k++){
                        for(l = 0; l < resize_factor; l++){
                        
                            if(resize_factor != 3)
                            {
                            red +=  in2->imag[3*(resize_factor*j+k) + 3*(resize_factor*i+l)*in2->width];
                            green +=  in2->imag[3*(resize_factor * j+k)+1 + 3*(resize_factor*i+l)*in2->width];
                            blue +=  in2->imag[3*(resize_factor * j+k)+2 + 3*(resize_factor*i+l)*in2->width];
                            }
                            else
                            {
                            red +=  in2->imag[3*(resize_factor*j+k) + 3*(resize_factor*i+l)*in2->width]*kg[k][l];
                            green +=  in2->imag[3*(resize_factor * j+k)+1 + 3*(resize_factor*i+l)*in2->width]*kg[k][l];
                            blue +=  in2->imag[3*(resize_factor * j+k)+2 + 3*(resize_factor*i+l)*in2->width]*kg[k][l];
                            }
                        }
                    }

                    if(resize_factor != 3)
                    {
                    out2->imag[3*j + 3*i*out2->width] = (unsigned char)(red/(resize_factor*resize_factor));
                    out2->imag[3*j+1 + 3*i*out2->width] = (unsigned char)(green/(resize_factor*resize_factor));
                    out2->imag[3*j+2 + 3*i*out2->width] = (unsigned char)(blue/(resize_factor*resize_factor));           
                    }
                    else
                    {
                    out2->imag[3*j + 3*i*out2->width] = (unsigned char)(red/16);
                    out2->imag[3*j+1 + 3*i*out2->width] = (unsigned char)(green/16);
                    out2->imag[3*j+2 + 3*i*out2->width] = (unsigned char)(blue/16);    
                    }
                    red = 0;
                    green = 0;
                    blue = 0;
                }
            }

    return NULL;
}

void resize(image *in, image * out) { 

    
    int i ;
            out->type = in->type;
            out->width = in->width / resize_factor;
            out->height = in->height / resize_factor; // determin height si width al imaginii noi
            out->max = in->max;
            
            in2 = in; //
            out2 = out;
   
            pthread_t tid[num_threads];
            int thread_id[num_threads];
            for(i = 0;i < num_threads; i++)
                thread_id[i] = i;
            
    // in functie de tipul pozei am paralelicat folosind functiile specifice
    if((*out).type == 5){
         int i;
         
         (*out).imagbl = alocareMatrice((*out).height, (*out).width);
      
            for(i = 0; i < num_threads; i++) {
                pthread_create(&(tid[i]), NULL, BW, &(thread_id[i]));
            }

    }

    if((*out).type == 6){
         int i;
         
        (*out).imag = alocareMatriceColor((*out).height, (*out).width);
        

            for(i = 0; i < num_threads; i++) {
                pthread_create(&(tid[i]), NULL, COLOR, &(thread_id[i]));
            }
    }
     for(i = 0; i < num_threads; i++) {
                pthread_join(tid[i], NULL);
            }

}