
#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H


typedef struct {
  unsigned int width, height;
  float *r, *g, *b;
} imagem;

imagem abrir_imagem(char *nome_do_arquivo);
void salvar_imagem(char *nome_do_arquivo, imagem *I,float mult);
void liberar_imagem(imagem *i);
imagem abrir_imagem_retornapixel(char *nome_do_arquivo); //funcao que adicionei

#endif
