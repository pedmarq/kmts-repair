#ifndef COMPONENTE_H
#define	COMPONENTE_H

#include "Configuracao.h"
#include "Formula.h"
#include "Estado.h"
#include <vector>
#include <map>
#include <sstream>

class Componente {
private:
  list<Configuracao*> configuracoes;
  int numConfiguracoesColoridas;
  int numComponente;
  bool visitado;
  vector< vector<bool> > *relacaoEntreComponenetes;
  Conectivo testemunhaCiclo;

public:

  Componente(int numeroComponente);
  void incrementNumConfiguracoesColoridos();
  void decrementNumConfiguracoesColoridos();
  list<Configuracao*> getConfiguracoes();
  void addConfiguracao(Configuracao *configuracao);
  string toStr();
  vector< vector<bool> >* getRelacaoOrdem();
  void setRelacao(vector <vector<bool> > *relacaoEntreComponentes);
  bool foiVisitado();
  void setVisitado(bool value);
  int getNumComponente();
  void setNumComponente(int num);
  int getNumConfiguracoesColoridas();

};

#endif
