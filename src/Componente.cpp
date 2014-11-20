#include "Configuracao.h"
#include "Arena.h"
#include "Componente.h"
#include "VisitTree.h"
#include <typeinfo>
#include <vector>
#include "Estado.h"
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <list>

Componente::Componente(int numeroComponente) {
  this->configuracoes = *(new list<Configuracao*>);
  this->numComponente = numeroComponente;
  this->visitado = false;
  this->numConfiguracoesColoridas = 0;
}

void Componente::incrementNumConfiguracoesColoridos(){
  this->numConfiguracoesColoridas++;
}

void Componente::decrementNumConfiguracoesColoridos(){
  this->numConfiguracoesColoridas--;
}

list<Configuracao*> Componente::getConfiguracoes() {
  return this->configuracoes;
}

void Componente::addConfiguracao(Configuracao *configuracao) {
  this->configuracoes.push_back(configuracao);
}

string Componente::toStr() {
  stringstream ss;
  ss << "### Numero Componente :: " << this->numComponente << endl;
  for (list<Configuracao*>::iterator it = this->configuracoes.begin(); it != this->configuracoes.end(); it++) {
    ss << "\t" << (*it)->toStr() << endl;

    list<Configuracao::TransicaoConfig> filhos = (*it)->getFilhos();


    for (list<Configuracao::TransicaoConfig>::iterator itFil = filhos.begin();
      itFil != filhos.end(); itFil++) {
        ss << "\t\t" << (itFil->destino)->toStr() << endl;
      }

    }

  return ss.str();
}

vector< vector<bool> > * Componente::getRelacaoOrdem() {
  return this->relacaoEntreComponenetes;
}

void Componente::setRelacao(vector< vector<bool> > *relacaoEntreComponenetes) {
  this->relacaoEntreComponenetes = relacaoEntreComponenetes;
}

bool Componente::foiVisitado() {
  return this->visitado;
}

void Componente::setVisitado(bool value) {
  this->visitado = value;
}

int Componente::getNumComponente() {
  return this->numComponente;
}

void Componente::setNumComponente(int num) {
  this->numComponente = num;
  for(list<Configuracao*>::iterator it = this->configuracoes.begin() ;
    it != this->configuracoes.end() ; it++){
      (*it)->setNumeroComponente(num);
  }
}

int Componente::getNumConfiguracoesColoridas(){

  int cont = 0 ;

  for(list<Configuracao*>::iterator it = this->configuracoes.begin() ;
    it != this->configuracoes.end() ; it++){
      if((*it)->getCor() == C_TRUE || (*it)->getCor() == C_FALSE ||
        (*it)->getCor() == C_INDEF){
          cont++;
        }
    }

  return cont;

  //return this->numConfiguracoesColoridas;
}
