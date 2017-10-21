%{
#include <stdio.h>
#include "imageprocessing.h"
#include <FreeImage.h>

void yyerror(char *c);
int yylex(void);

%}
%union {
  char    strval[50];
  int     ival;
  float fval;
}
%token <strval> STRING
%token <ival> VAR IGUAL EOL ASPA 
%token <fval> FATOR
%left SOMA SINAL DIVISAO ABRECOL FECHACOL FIM

%%

PROGRAMA:
        PROGRAMA ABRECOL STRING FECHACOL EOL{      //retorna os maiores valores de RGB - ex: [teste.jpg] 
        imagem I = abrir_imagem_retornapixel($3); //fiz a funcao abrir_imagem_retornapixel em lib_imageprocessing.c
        salvar_imagem($3,&I,1);
        }
        |
        PROGRAMA STRING IGUAL STRING DIVISAO FATOR EOL{ //divide cada pixel do RGB por uma constante - ex: teste.jpg = demo.jpg / 0.5
        printf("Copiando %s para %s\n", $4, $2);
        imagem I = abrir_imagem($4);
        printf("Li imagem %d por %d\n", I.width, I.height);
        mult_pixel(&I,1/$6);
        salvar_imagem($2, &I,1); //o ultimo argumento dessa funçao eh o fator que se multiplica cada pixel
        }
        |
        PROGRAMA STRING IGUAL STRING SINAL FATOR EOL{ //multiplica cada pixel por uma constante - ex: teste.jpg = demo.jpg * 0.5
        printf("Copiando %s para %s\n", $4, $2);
        imagem I = abrir_imagem($4);
        printf("Li imagem %d por %d\n", I.width, I.height);
        mult_pixel(&I, $6);
        salvar_imagem($2, &I,1);
        }
        |
        PROGRAMA STRING IGUAL STRING EOL{ //apenas copia um arquivo para outro - ex: teste.jpg = demo.jpg
        printf("Copiando %s para %s\n", $4, $2);
        imagem I = abrir_imagem($4);
        printf("Li imagem %d por %d\n", I.width, I.height);
        salvar_imagem($2, &I,1);
        }
        |
        PROGRAMA FIM EOL{ //finaliza o programa quando ; é digitado
        printf("Programa finalizado.\n");
        return 0;
        }
        |
        ;



%%

void yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
}

int main() {
  FreeImage_Initialise(0);
  yyparse();
  return 0;

}
