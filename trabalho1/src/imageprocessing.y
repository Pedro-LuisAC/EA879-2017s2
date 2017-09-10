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
        PROGRAMA ABRECOL STRING FECHACOL EOL{
        imagem I = abrir_imagem_retornapixel($3);
        salvar_imagem($3,&I,1);
        }
        |
        PROGRAMA STRING IGUAL STRING DIVISAO FATOR EOL{
        printf("Copiando %s para %s\n", $4, $2);
        imagem I = abrir_imagem($4);
        printf("Li imagem %d por %d\n", I.width, I.height);
        salvar_imagem($2, &I,1/$6); //o ultimo argumento dessa funçao eh o fator que se multiplica cada pixel
        }
        |
        PROGRAMA STRING IGUAL STRING SINAL FATOR EOL{
        printf("Copiando %s para %s\n", $4, $2);
        imagem I = abrir_imagem($4);
        printf("Li imagem %d por %d\n", I.width, I.height);
        salvar_imagem($2, &I,$6);
        }
        |
        PROGRAMA STRING IGUAL STRING EOL{
        printf("Copiando %s para %s\n", $4, $2);
        imagem I = abrir_imagem($4);
        printf("Li imagem %d por %d\n", I.width, I.height);
        salvar_imagem($2, &I,1);
        }
        |
        PROGRAMA FIM EOL{
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
