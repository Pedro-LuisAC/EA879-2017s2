
#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H


typedef struct {
  unsigned int width, height;
  float *r, *g, *b;
} imagem;

 struct Conjunto{
  imagem *K;
  int inicio;
  int fim;
  float valor;
 };

 #define n_threads 4	

struct Conjunto conjunto[n_threads];

imagem abrir_imagem(char *nome_do_arquivo);
void salvar_imagem(char *nome_do_arquivo, imagem *I,float mult);
void liberar_imagem(imagem *i);
imagem abrir_imagem_retornapixel(char *nome_do_arquivo); //funcao que adicionei
void mult_pixel(imagem *I, float valor);
void mult_thread(struct Conjunto *conjunto);
void varias_thread(imagem *I, float valor);
void mult_pixelapixel(float *rr,float *gg,float *bb, int valor, int max,int min);
void varios_processo(imagem *I, float valor);

#endif
