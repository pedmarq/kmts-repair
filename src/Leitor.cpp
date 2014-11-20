#include "Leitor.h"
#include <cstdlib>
#include <iostream>
// #include "lex.yy.c"
#include "Formula.h"
#include "VisitTree.h"
#include "scanner.c"
#include "Estado.h"
#include <map>

using namespace std;

extern int yyparse();
extern Formula *principal;
extern list<Estado> estadosLidos;
extern list<TransicaoTemp> transicoes;

Leitor::Leitor(string caminho){
    yyin = fopen( caminho.c_str(), "r" );
    //cout << "INICIO" << endl;
    do {
		yyparse();
	} while (!feof(yyin));
        
        
        
//        for(list<TransicaoTemp>::iterator it1 = transicoes.begin() ; it1 != transicoes.end() ; it1++){
//            cout << it1->est1 + it1->est2 + 
//                        ( it1->tipo == MUST ? "MUST" : "MAY") << endl;
//        }
        
        //Tratar Estados
        
        map<string,Estado*> estadosMapeados;
        
        for(list<Estado>::iterator it = estadosLidos.begin() ; it !=estadosLidos.end() ; it++){
            estadosMapeados[it->getNome()] = &(*it);
        }
        
        for(list<TransicaoTemp>::iterator it = transicoes.begin() ; it != transicoes.end() ; it++){
            Estado::Transicao t1 = *(new Estado::Transicao);
            t1.filho = estadosMapeados[it->est2];
            t1.tipo = it->tipo;
            estadosMapeados[it->est1]->addTransicao(t1);            
        }
        
        
}

Formula* Leitor::lerFormulas(){    
        
  return principal;
}

list<Estado> Leitor::lerEstados(){
    return estadosLidos;
}


