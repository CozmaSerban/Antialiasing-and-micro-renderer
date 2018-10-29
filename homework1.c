#include "homework1.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
int num_threads;
int resolution;


image *out2; // referinta ca sa accesez usor imaginea in functia de threaduri



blackwhite** alocareMatrice(int w){ // functia de alocare a matricei pentru randare si fac fiecare pixel alb

    blackwhite** test;
    int i,j;

    test = (blackwhite **) malloc (sizeof (blackwhite *) * w); //aloc matricea

	for (i = 0; i < w; i++) { 

	 	test[i] = (blackwhite *) malloc (sizeof (blackwhite) * w);
     
	}

    for(i = 0; i < w; i++){
        for(j = 0; j < w; j++){
            test[i][j].a = 255; // fac pixelul alb
        }
    }

    return test;

}

void initialize(image *im) {
    im->width = resolution;
    (*im).img = alocareMatrice((*im).width); // apelez functia de alocare
    
}


int min(int a, int b){ // functia de minim pentru determinarea first si second la threaduri

    return (a < b)? a : b;
}


void* THREAD(void * var)
{ 
    int i,j;
    int thread_id = *(int*)var;
   
    int first = thread_id * ceil(((double)out2->width/num_threads)); // calcule pentru first si second
    int second = min((thread_id + 1) * ceil(((double)out2->width/num_threads)),out2->width);

 

// parcurg matricea si verific cu formula si daca se respecta cerintele din enunt fac pixelul negru
for(i = out2->width -1; i >=0; i--){
        for(j = first; j < second; j++){
            float test = sqrt(5);

            if(1.0 * abs((-1) * (j+0.5) * 100 / out2 -> width  + 2*(out2->width-i-1 + 0.5)*100.0/out2->width + 0) / test <= 3){
            
                  out2->img[i][j].a = 0;  
            }
        
        }
    }

    return NULL;
}


void render(image *im) {
    int i;
    out2 = im;
    
    
        // paralelizez
        pthread_t tid[num_threads];
            int thread_id[num_threads];
            for(i = 0;i < num_threads; i++)
                thread_id[i] = i;

            for(i = 0; i < num_threads; i++) {
                pthread_create(&(tid[i]), NULL, THREAD, &(thread_id[i]));
            }

            for(i = 0; i < num_threads; i++) {
                pthread_join(tid[i], NULL);
            }
    

}

void writeData(const char * fileName, image *img) {


    FILE *write;

    int i ;
    write = fopen(fileName,"wb"); //deschid fisier
    fprintf(write,"P5\n%d %d\n255\n",(*img).width,(*img).width); // scriu header
    
    for(i = 0; i < (*img).width; i++){ // scriu pixeli linie cu linie din imagine
        fwrite((*img).img[i],sizeof(blackwhite), (*img).width, write);
    }
    fclose(write);
    
}

