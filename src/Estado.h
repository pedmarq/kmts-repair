/* 
 * File:   Estado.h
 * Author: jandson
 *
 * Created on 28 de Outubro de 2013, 19:23
 */

#ifndef ESTADO_H
#define	ESTADO_H

#include <string>
#include <iostream>
#include <set>
#include "Estado.h"
#include <list>

using namespace std;

typedef struct{
    string literal;
    bool valorLogico;
}literalNegativo;

enum TipoTransicao{
    MUST, MAY
};

enum LogicalValues{
    L_TRUE, L_FALSE, L_INDEFINED
};

typedef struct{
    string est1, est2;
    TipoTransicao tipo;
}TransicaoTemp;




class Estado{
    
public:
    typedef struct{
        Estado *filho;
        TipoTransicao tipo;
    }Transicao;
    
struct classcomp {
  bool operator() (const literalNegativo& lhs, const literalNegativo& rhs) const
  {return (lhs.literal.compare(rhs.literal)) < 0;}
};
    
private:
    
    string nome;
   
    set<literalNegativo,classcomp> literais ;
    list<Transicao> transicoes;
    
    
    
public:
    
    Estado(string nome, list<literalNegativo> literais);
    string toStr();
    
    string getNome();
    list<Transicao> getTransicoes();
    list<Transicao>* getOwnTransicoes();
   void  addTransicao(Transicao &t);
   void removeTransicao(Transicao t);   
    //void addLiteralNegativo(literalNegativo literal);
   //void addTransicao(Transicao transicao);
   LogicalValues valueOfLiteral(literalNegativo ln);
   void defineLiteral(literalNegativo ln);
   void indefineLiteral(literalNegativo ln);
   
//   list<literalNegativo> getLiterais(){
//       list<literalNegativo> lit = *(new list<literalNegativo>);
////       
////       for(set<literalNegativo,classcomp>::iterator it = this->literais.begin() ; 
////               it != this->literais.end() ; it++){
////           lit.push_back(*it);
////       }
//       
//       return lit;
//   }
   
   string imprimirEstado(){
       
       string str = "";
       
       for(set<literalNegativo,classcomp>::iterator it = this->literais.begin() ; 
               it != this->literais.end() ; it++){
           
           if(!it->valorLogico){
               str = str.append("not ");
           }
           
           str = str.append(it->literal);
           str = str.append(" ");
           
       }
       
       return str;
       
   }
   
   string imprimirTransicoes(){
       
       string str = "";
       
       for(list<Transicao>::iterator it = this->transicoes.begin() ; 
               it != this->transicoes.end() ; it++){
           
           str = str.append("(");
           str = str.append(this->getNome());
           str = str.append(",");
           str = str.append(it->filho->getNome());
           str = str.append(") : ");
           
           if(it->tipo == MAY){
               str = str.append("-");
           }else{
               str = str.append("+");
           }
           
           str = str.append("\n");
       }
       
       return str;
   }
    
};


#endif	/* ESTADO_H */

