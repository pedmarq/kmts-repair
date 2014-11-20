#ifndef ARENA_H
#define	ARENA_H

#include "Configuracao.h"
#include "Componente.h"
#include "Formula.h"
#include "Estado.h"
#include <vector>
#include <map>
#include <sstream>

class Arena {
private:

  Formula *formula;

  list<Estado> modelo;
  vector<Configuracao*> cabecasTabuleiro; //apontadores para as raizes de cada Si X Psi
  // ou seja, raiz de cada arvore de estado do tabuleiro
  void constroiArena(vector<Configuracao*> configuracoes, Formula *form);
  vector<Configuracao*>* controiConfigs(Formula *form);
  map<string, vector<Configuracao*> > tabelaVariaveis;
  map<string, Conectivo> tipoVariavel;
  map<string, int> nameEstToPosVector;
  map<int, Estado*> posVectotToEst;

  list<Configuracao*> matrizConfiguracoes;

  int index;

  list<Componente*> componentes;

  void marcarComponentesConexos();
  void componenteConexas(Configuracao *configuracao, list<Configuracao*> *pilha);

  int minBetwConfigs(int c1, int c2);


public:
  Arena(list<Estado> modelo, Formula *form);
  Arena(list<Configuracao*> configs);
  void printArena();
  list<Componente*> getComponentes();

  Estado* estadoNaPos(int posEstate);

  Conectivo getTipoVariavel(string variavel);

  void printColorsResult();

  void printArenaSimples();
  Formula *getForm();

  list<Estado>* getModelo();
  int namesToNumEstates(string nameState);

  list<Configuracao*> getMatrizConfiguracao();
  vector<Configuracao*> getCabecasTabuleiro();

  set<Configuracao*, bool(*)(Configuracao*,Configuracao*)> getIndefinedConfigs();

  static bool compConfigs(Configuracao *config1, Configuracao *confg2){
    return config1->getNumNome() < confg2->getNumNome();
  }




};

#endif /* arena.h */
