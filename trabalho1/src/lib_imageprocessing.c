#include <stdlib.h>
#include <stdio.h>

#include "imageprocessing.h"

#include <FreeImage.h>
#include <pthread.h>
#include <time.h>

#include <sys/types.h>

#include <sys/time.h>
#include <unistd.h>

#include <sys/mman.h>

 #define n_threads 4
#define n_processos 4



struct timeval rt0, rt1, drt;

imagem abrir_imagem(char *nome_do_arquivo) {
  FIBITMAP *bitmapIn;
  int x, y;
  RGBQUAD color;
  imagem I;

  bitmapIn = FreeImage_Load(FIF_JPEG, nome_do_arquivo, 0);

  if (bitmapIn == 0) {
    printf("Erro! Nao achei arquivo - %s\n", nome_do_arquivo);
  } else {
    printf("Arquivo lido corretamente!\n");
   }

  x = FreeImage_GetWidth(bitmapIn);
  y = FreeImage_GetHeight(bitmapIn);

  I.width = x;
  I.height = y;
  I.r = malloc(sizeof(float) * x * y);
  I.g = malloc(sizeof(float) * x * y);
  I.b = malloc(sizeof(float) * x * y);

   for (int i=0; i<x; i++) {
     for (int j=0; j <y; j++) {
      int idx;
      FreeImage_GetPixelColor(bitmapIn, i, j, &color);

      idx = i + (j*x);

      I.r[idx] = color.rgbRed;
      I.g[idx] = color.rgbGreen;
      I.b[idx] = color.rgbBlue;
    }
   }
  return I;

}

imagem abrir_imagem_retornapixel(char *nome_do_arquivo) { //fiz essa funcao para retornar os maiores valores do RGB
  FIBITMAP *bitmapIn;
  int x, y;
  float red=0.0,green=0.0,blue=0.0;
  RGBQUAD color;
  imagem I;

  bitmapIn = FreeImage_Load(FIF_JPEG, nome_do_arquivo, 0);

  if (bitmapIn == 0) {
    printf("Erro! Nao achei arquivo - %s\n", nome_do_arquivo);
  } else {
    printf("Arquivo lido corretamente!\n");
   }

  x = FreeImage_GetWidth(bitmapIn);
  y = FreeImage_GetHeight(bitmapIn);

  I.width = x;
  I.height = y;
  I.r = malloc(sizeof(float) * x * y);
  I.g = malloc(sizeof(float) * x * y);
  I.b = malloc(sizeof(float) * x * y);

   for (int i=0; i<x; i++) { 
     for (int j=0; j <y; j++) {
      int idx;
      FreeImage_GetPixelColor(bitmapIn, i, j, &color);

      idx = i + (j*x);

      I.r[idx] = color.rgbRed;
      I.g[idx] = color.rgbGreen;
      I.b[idx] = color.rgbBlue;

      if(I.r[idx]>=red){// eu que modifiquei essa parte da funcao para encontrar o menor valor
        red = I.r[idx]; //acha o menor valor de cada RGB
      }
      if (I.g[idx]>=green){
        green = I.g[idx];
      }
      if(I.b[idx] >= blue){
        blue = I.b[idx];
      }
    }
   }

printf("R: %f\n",red);
printf("G: %f\n",green);
printf("B: %f\n",blue);

  return I;

}

void salvar_imagem(char *nome_do_arquivo, imagem *I,float mult) { //adicionei o parametro mult
  FIBITMAP *bitmapOut;
  RGBQUAD color;


  printf("Salvando imagem %d por %d...\n", I->width, I->height);
  bitmapOut = FreeImage_Allocate(I->width, I->height, 24, 0, 0, 0);


   for (int i=0; i<I->width; i++) {
     for (int j=0; j<I->height; j++) {
      int idx;

      idx = i + (j*I->width);
      color.rgbRed = (I->r[idx]);  //adicionei o termo mult para multiplicar cada RGB
      color.rgbGreen = (I->g[idx]);
      color.rgbBlue = (I->b[idx]);

      FreeImage_SetPixelColor(bitmapOut, i, j, &color);
    }
  }


  

  FreeImage_Save(FIF_JPEG, bitmapOut, nome_do_arquivo, JPEG_DEFAULT);
}

void mult_pixel(imagem *I, float valor) {

    gettimeofday(&rt0, NULL);

    for (int i=0; i<I->width; i++) {
     for (int j=0; j<I->height; j++) {
      int idx;

      idx = i + (j*I->width);

      I->r[idx] = I->r[idx]*valor;
      if(I->r[idx] > 255.0){
        I->r[idx] = 255.0;
      }
      I->g[idx] = I->g[idx]*valor;
      if(I->g[idx] > 255.0){
        I->g[idx] = 255.0;
      }
      I->b[idx] = I->b[idx]*valor;
      if(I->b[idx] > 255.0){
        I->b[idx] = 255.0;
      }
    }
  }

  gettimeofday(&rt1, NULL);
  timersub(&rt1, &rt0, &drt);

  printf("Tempo real: %ld.%06ld segundos\n", drt.tv_sec, drt.tv_usec);
    
}

 void varias_thread(imagem *I, float valor){
   
   pthread_t threads[n_threads];
   gettimeofday(&rt0, NULL);
   conjunto[0].inicio = 0;
   conjunto[0].fim = ((I->width)*(I->height))/n_threads;
 
        for(int k=0;(k<n_threads);k++){
          
          (conjunto[k]).K = I;
          
          conjunto[k].valor = valor;
          if (k) {
            conjunto[k].inicio = conjunto[k-1].fim +1 ;
            conjunto[k].fim = conjunto[k-1].fim + conjunto[0].fim + 1;
          }
          
        
          pthread_create(&(threads[k]),NULL,mult_thread, (void*)(&conjunto[k]));
          
        }
        for(int k=0;(k<n_threads);k++){
          pthread_join(threads[k],NULL);
        }

        
      

  gettimeofday(&rt1, NULL);
  timersub(&rt1, &rt0, &drt);

  printf("Tempo real: %ld.%06ld segundos\n", drt.tv_sec, drt.tv_usec);

     
  }

 void mult_thread(struct Conjunto *conjunto) {
      int idx;

      for(int g = conjunto->inicio; g < (conjunto->fim); g++) {

        
        conjunto->K->r[g] = (conjunto->K->r[g])*(conjunto->valor);
          if(conjunto->K->r[g] > 255.0){
            conjunto->K->r[g] = 255.0;
          }
          conjunto->K->g[g] = conjunto->K->g[g]*(conjunto->valor);
          if(conjunto->K->g[g] > 255.0){
            conjunto->K->g[g] = 255.0;
          }
          conjunto->K->b[g] = conjunto->K->b[g]*(conjunto->valor);
          if(conjunto->K->b[g] > 255.0){
            conjunto->K->b[g] = 255.0;
          }


       //return NULL;
       //pthread_exit(0);
    }
 }
void varios_processo(imagem *I, float valor){
  pid_t filho[n_processos];
  int contador = 0;
  int min[n_processos], max[n_processos];
  gettimeofday(&rt0, NULL);

  min[0] = 0;
  max[0] = ((I->width)*(I->height))/n_processos;

  for(int i = 1; i<n_processos; i++){
    min[i] = max[i-1] + 1;
    max[i] = max[i-1] + max[0] + 1;
  }
  int tam = (I->height)*(I->width);
  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANON;
  float *rr,*bb,*gg;
  rr = (float*) mmap(NULL, sizeof(float)*tam, protection, visibility, 0, 0);
  bb = (float*) mmap(NULL, sizeof(float)*tam, protection, visibility, 0, 0);
  gg = (float*) mmap(NULL, sizeof(float)*tam, protection, visibility, 0, 0);

  for(int demonio = 0; demonio<tam;demonio++){
    *(rr+demonio) = I->r[demonio];
    *(gg+demonio) = I->g[demonio];
    *(bb+demonio) = I->b[demonio];
  }
  //if ((imagem)b==-1) printf("Erro de alocacao!\n");
  

  for(int i =0; i<n_processos ; i++){
  
    filho[i] = fork();

    if(filho[i] == 0){

      mult_pixelapixel(rr,gg,bb,valor,max[i],min[i]);
      exit(EXIT_SUCCESS);

    }
    

  }


  for(int i=0; i < n_processos; i++){
    int status;
    waitpid(filho[i], &status, 0);


  }
  I->r = rr;
  I->g = gg;
  I->b = bb;

  gettimeofday(&rt1, NULL);
  timersub(&rt1, &rt0, &drt);

  printf("Tempo real: %ld.%06ld segundos\n", drt.tv_sec, drt.tv_usec);


}

void mult_pixelapixel(float *rr,float *gg,float *bb, int valor, int max,int min){

      for (int j=min;j<(max);j++){
        *(rr+j) = *(rr+j)*valor;
        if(*(rr+j) > 255.0){
          *(rr+j) = 255.0;
        }
        *(gg+j) = *(gg+j)*valor;
        if(*(gg+j) > 255.0){
          *(gg+j) = 255.0;
        }
        *(bb+j) = *(bb+j)*valor;
        if(*(bb+j) > 255.0){
          *(bb+j) = 255.0;
        }
      }
}


void col(imagem *I, float valor) {

      gettimeofday(&rt0, NULL);

    for (int i=0; i<(I->width)*(I->height); i++) {
      int idx;

      idx = i;

      I->r[idx] = I->r[idx]*valor;
      if(I->r[idx] > 255.0){
        I->r[idx] = 255.0;
      }
      I->g[idx] = I->g[idx]*valor;
      if(I->g[idx] > 255.0){
        I->g[idx] = 255.0;
      }
      I->b[idx] = I->b[idx]*valor;
      if(I->b[idx] > 255.0){
        I->b[idx] = 255.0;
      }
    
  }
    gettimeofday(&rt1, NULL);
  timersub(&rt1, &rt0, &drt);

  printf("Tempo real: %ld.%06ld segundos\n", drt.tv_sec, drt.tv_usec);
}