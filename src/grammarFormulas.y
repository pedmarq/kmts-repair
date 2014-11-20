%{
   	#include <iostream>
   	#include <stdio.h>
   	#include <stdlib.h>
	#include "Formula.h"
	#include <string>
        #include "Estado.h"
        #include <set>

	using namespace std;

	void yyerror (const char *error);
  	extern int yylex();

	Formula *principal;
        list<Estado> estadosLidos = *(new list<Estado>);
        list<TransicaoTemp> transicoes = *(new list<TransicaoTemp>);


 %}

 %union{
 	string *String;
        int token;
        Formula *form;
        Estado *est;
        literalNegativo *litneg;
        list<literalNegativo> *listLit;
 }


%token <String> LITERAL VARIAVEL ESTADO
%token <token> AND OR EX AX NOT MINPT MAXPT POINT LPAREN RPAREN VIRG PLUS MINUS DBPOINT

%type <form> formula
%type <est> estado
%type <litneg> litNeg
%type <listLit> listaLiteral
%left OR AND 


%% 

inicio : formula estados transicoes{}

formula : 	LITERAL { principal = new FormulaLiteral(*$1, true); 
                             //cout << principal->toStr() + "TTTTT" << endl;
                           $$ = principal; }                           
                |VARIAVEL { principal = new FormulaVariavel(*$1); 
                             //cout << principal->toStr() + "TTTTT" << endl;
                           $$ = principal; }
                | formula AND formula {
                            principal = new FormulaBinaria(C_AND, $3,$1); 
                             //cout << principal->toStr() + "TTTTT" << endl;
                           $$ = principal;
                                
                            }
                |formula OR formula {
                            principal = new FormulaBinaria(C_OR, $3,$1); 
                             //cout << principal->toStr() + "TTTTT" << endl;
                           $$ = principal;
                                
                            }
                | LPAREN formula RPAREN {
                            principal = $2;
                            $$ = principal;
                                
                            }
                | MAXPT VARIAVEL POINT LPAREN formula RPAREN {
                            principal = new FormulaPontoFixo(C_MAXPT, $5, *$2); 
                             //cout << principal->toStr() + "TTTTT" << endl;
                           $$ = principal;
                                
                            }
                | MINPT VARIAVEL POINT LPAREN formula RPAREN {
                            principal = new FormulaPontoFixo(C_MINPT, $5, *$2); 
                             //cout << principal->toStr() + "TTTTT" << endl;
                           $$ = principal;
                                
                            }
                | EX formula  {
                            principal = new FormulaPrefixa(C_EX, $2); 
                             //cout << principal->toStr() + "TTTTT" << endl;
                           $$ = principal;
                                
                            }
                | AX formula  {
                            principal = new FormulaPrefixa(C_AX, $2); 
                             //cout << principal->toStr() + "TTTTT" << endl;
                           $$ = principal;
                                
                            }
                 | NOT LITERAL  {
                            principal = new FormulaLiteral(*$2, false); 
                             //cout << principal->toStr() + "TTTTT" << endl;
                           $$ = principal;
                                
                            }


estados : estado estados {}
           | estado {}
estado: ESTADO LPAREN listaLiteral RPAREN {Estado *e = new Estado(*$1,*$3); 
                                        estadosLidos.push_back(*e);
                                        }

listaLiteral: litNeg listaLiteral { $$ = $2 ; $2->push_back(*$1); }
            | {$$ = new list<literalNegativo>;}

litNeg: LITERAL {literalNegativo *l = new literalNegativo;
                    l->literal = *$1;
                    l->valorLogico = true;
                    $$ = l;
                 }
        | NOT LITERAL  {literalNegativo *l = new literalNegativo;
                    l->literal = *$2;
                    l->valorLogico = false;
                    $$ = l;;

                 }

transicoes :  transicao transicoes {}
               | {}
transicao : LPAREN ESTADO VIRG ESTADO RPAREN DBPOINT PLUS      {   TransicaoTemp *t = new TransicaoTemp;
                                                        t->est1 = *$2;
                                                        t->est2 = *$4;
                                                        t->tipo = MUST;
                                                        transicoes.push_back(*t);
                                                    }
            | LPAREN ESTADO VIRG ESTADO RPAREN DBPOINT MINUS      {   TransicaoTemp *t = new TransicaoTemp;
                                                        t->est1 = *$2;
                                                        t->est2 = *$4;
                                                        t->tipo = MAY;
                                                        transicoes.push_back(*t);
                                                    }                                                    
%%

void yyerror(const char *s) {
	cout << "EEK, parse error!  Message: " << s << endl;
	// might as well halt now:
	exit(-1);
}

