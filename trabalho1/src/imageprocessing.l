
%{
#include <stdlib.h>
#include <string.h>
void yyerror(char *);

#include "y.tab.h"
%}



%%
  
[0-9]+\.*[0-9]*  { //acha float
        for(int i = 0; i<yyleng;i++){
          yylval.fval = atof(yytext);
        }
        return FATOR;
}


[a-z]  {
        yylval.ival = yytext[0] - 'a';
        return VAR;
       }


\" { return ASPA; }

[a-zA-Z0-9\.]+  { //Acha nome do arquivo
        for (int i = 0; i < yyleng; i++ )
          yylval.strval[i] = yytext[i];

        yylval.strval[yyleng] = '\0';
        return STRING;
       }

[=]     {
          return IGUAL;
        }

[/]     {
    return DIVISAO; 
}
[*]     {
  return SINAL;
}
\[  {
  return ABRECOL;
}
\]   {
  return FECHACOL;
}


\n   {
         return EOL;
       }
\; {
  return FIM;
}

[ \t]+ ;

.     {printf("**%s**\n", yytext);
yyerror("Token nao reconhecido!\n");}

%%

int  yywrap() {
  return 1;
}
