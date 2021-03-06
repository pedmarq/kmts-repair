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


void Arena::marcarComponentesConexos() {

  list<Configuracao*> *pilha = (new list<Configuracao*>);

  this->index = 0;

  for (list<Configuracao*>::iterator it = this->matrizConfiguracoes.begin();
    it != this->matrizConfiguracoes.end(); it++) {
      (*it)->setVisited(false);
      (*it)->setNumeroComponente(0);
      (*it)->setIndex(0);

    }

    for (list<Configuracao*>::iterator it = this->matrizConfiguracoes.begin();
      it != this->matrizConfiguracoes.end(); it++) {
        if ( !(*it)->wasVisited()) {
          //pilha->push_back(*it);
          componenteConexas(*it, pilha);
        }
      }


}

void Arena::componenteConexas(Configuracao* configuracao, list<Configuracao*> *pilha) {
    this->index++;
    configuracao->setLowIndex(index);
    configuracao->setIndex(index);
    configuracao->setIntoStack(true);
    configuracao->setVisited(true);
    pilha->push_back(configuracao);

    list<Configuracao::TransicaoConfig> filhos = configuracao->getFilhos();

    for (list<Configuracao::TransicaoConfig>::iterator filho = filhos.begin();
      filho != filhos.end(); filho++) {
        if ( !((filho->destino)->wasVisited()) ) {
          componenteConexas(filho->destino, pilha);
          int lowIndex = minBetwConfigs(filho->destino->getLowIndex(), configuracao->getLowIndex());
          configuracao->setLowIndex(lowIndex);
        } else if (filho->destino->isIntoStack()) {
          int lowIndex = minBetwConfigs((filho->destino)->getIndex(), configuracao->getLowIndex());
          configuracao->setLowIndex(lowIndex);
        }


      }

      //fim filhos
      //Se for a raiz do componente
      if (configuracao->getLowIndex() == configuracao->getIndex()) {
        Componente *comp = new Componente(configuracao->getIndex());

        do {
          pilha->back()->setNumeroComponente(comp->getNumComponente());
          pilha->back()->setIntoStack(false);
          comp->addConfiguracao(pilha->back());
          pilha->pop_back();
        } while ((!pilha->empty()) && pilha->back()->getIndex() >= configuracao->getIndex());

        this->componentes.push_back(comp);

      }
}

      int Arena::minBetwConfigs(int c1, int c2) {

        if (c1 < c2) {
          return c1;
        } else {
          return c2;
        }

      }

      list<Componente*> Arena::getComponentes() {
        return this->componentes;
      }

      Formula* Arena::getForm() {
        return this->formula;
      }

      Arena::Arena(list<Estado> modelo, Formula *form) {
        this->formula = form;
        this->index = 0;
        this->modelo = modelo;
        this->matrizConfiguracoes = *(new list<Configuracao*>);
        this->componentes = *(new list<Componente*>);

        this->tipoVariavel = *(new map<string, Conectivo>);

        this-> nameEstToPosVector = *(new map<string, int>);
        this->posVectotToEst = *(new map<int, Estado*>);

        int itConst = 0;

        for (list<Estado>::iterator it = this->modelo.begin(); it != this->modelo.end(); it++) {
          nameEstToPosVector[it->getNome()] = itConst;
          posVectotToEst[itConst] = &(*it);
          itConst++;
        }

        int numEstados = modelo.size();

        this->cabecasTabuleiro = *(new vector<Configuracao*>(numEstados));

        if (typeid (*form) == typeid (FormulaLiteral)) {
          for (int i = 0; i < numEstados; i++) {
            FormulaLiteral *fl = dynamic_cast<FormulaLiteral*> (form);
            this->cabecasTabuleiro[i] = (new Configuracao(fl->getLiteral(), i, i));

          }
        } else {

          if (typeid (*form) == typeid (FormulaBinaria)) {
            for (int i = 0; i < numEstados; i++) {
              FormulaBinaria *fb = dynamic_cast<FormulaBinaria*> (form);
              this->cabecasTabuleiro[i] = new Configuracao(fb->getConectivo(), i, i);
            }

          } else if (typeid (*form) == typeid (FormulaPrefixa)) {
            FormulaPrefixa *fb = dynamic_cast<FormulaPrefixa*> (form);
            for (int i = 0; i < numEstados; i++) {
              this->cabecasTabuleiro[i] = new Configuracao(fb->getConectivo(), i, i);
            }

          } else if (typeid (*form) == typeid (FormulaPontoFixo)) {
            FormulaPontoFixo *fb = dynamic_cast<FormulaPontoFixo*> (form);
            for (int i = 0; i < numEstados; i++) {
              this->cabecasTabuleiro[i] = new Configuracao(fb->getConectivo(), i, i);
            }
          }



        }

        constroiArena(this->cabecasTabuleiro, form);
        this->marcarComponentesConexos();

      }

      vector<Configuracao*>* Arena::controiConfigs(Formula *form) {

        // if formula for do tipo variavel retornar o vetor da tabelade variaveis corespondente

        vector<Configuracao*> *configs = new vector<Configuracao*>(this->modelo.size());

        if (typeid (*form) == typeid (FormulaLiteral)) {
          for (int i = 0; i < this->modelo.size(); i++) {
            FormulaLiteral *fl = dynamic_cast<FormulaLiteral*> (form);
            (*configs)[i] = (new Configuracao(fl->getLiteral(), i, this->matrizConfiguracoes.size() + i));

          }
        } else if (typeid (*form) == typeid (FormulaPontoFixo)) {
          FormulaPontoFixo *fp = dynamic_cast<FormulaPontoFixo*> (form);

          for (int i = 0; i < this->modelo.size(); i++) {
            (*configs)[i] = new Configuracao(fp->getConectivo(), i, this->matrizConfiguracoes.size() + i);
          }

          // this->tabelaVariaveis[fp->getVariavel()] = *configs;
        } else if (typeid (*form) == typeid (FormulaBinaria)) {
          for (int i = 0; i < this->modelo.size(); i++) {
            FormulaBinaria *fl = dynamic_cast<FormulaBinaria*> (form);
            (*configs)[i] = (new Configuracao(fl->getConectivo(), i, this->matrizConfiguracoes.size() + i));
          }
        } else if (typeid (*form) == typeid (FormulaPrefixa)) {
          FormulaPrefixa *fl = dynamic_cast<FormulaPrefixa*> (form);
          for (int i = 0; i < this->modelo.size(); i++) {
            (*configs)[i] = (new Configuracao(fl->getConectivo(), i, this->matrizConfiguracoes.size() + i));
          }
        }

        return configs;
      }

      void Arena::constroiArena(vector<Configuracao*> configuracoes, Formula *form) {

        for (int i = 0; i < configuracoes.size(); i++) {
          this->matrizConfiguracoes.push_back(configuracoes[i]);
        }

        //Nó terminal
        if (configuracoes[0]->getConectivo() == C_NONE) {
          return;
        }

        //no de formula binaria AND ou OR
        if (configuracoes[0]->getConectivo() == C_AND || configuracoes[0]->getConectivo() == C_OR) {
          list<Formula*> formulasAux = form->getFilhos();

          for (list<Formula*>::iterator it = formulasAux.begin()
            ; it != formulasAux.end(); it++) {

              vector<Configuracao*> *confg = controiConfigs((*it));

              for (int j = 0; j < this->modelo.size(); j++) {
                (*confg)[j]->addParent(configuracoes[j]);
                configuracoes[j]->addFilho((*confg)[j]);
              }

              constroiArena(*confg, (*it));

            }
          } else if (configuracoes[0]->getConectivo() == C_MAXPT || configuracoes[0]->getConectivo() == C_MINPT) {

            vector<Configuracao*> configs = *(new vector<Configuracao*>(this->modelo.size()));
            vector<Configuracao*> *proxConfigs;
            FormulaPontoFixo *fp = dynamic_cast<FormulaPontoFixo*> (form);
            //criar variaveis

            for (int i = 0; i < this->modelo.size(); i++) {
              literalNegativo ltn = *(new literalNegativo);
              ltn.literal = fp->getVariavel();
              ltn.valorLogico = true;
              configs[i] = new Configuracao(ltn, i, this->matrizConfiguracoes.size() + i);
              configs[i]->addParent(configuracoes[i]);
              configuracoes[i]->addFilho(configs[i]);
            }

            for (int i = 0; i < configuracoes.size(); i++) {
              this->matrizConfiguracoes.push_back(configs[i]);
            }


            this->tabelaVariaveis[fp->getVariavel()] = configs;
            this->tipoVariavel[fp->getVariavel()] = configuracoes[0]->getConectivo();

            proxConfigs = controiConfigs(form->getFilhos().front());

            for (int i = 0; i < this->modelo.size(); i++) {
              (*proxConfigs)[i]->addParent(configs[i]);
              configs[i]->addFilho((*proxConfigs)[i]);
            }

            constroiArena(*proxConfigs, form->getFilhos().front());

          } else if (configuracoes[0]->getConectivo() == C_AX || configuracoes[0]->getConectivo() == C_EX) {

            vector<Configuracao*> *configs;
            if (typeid (*(form->getFilhos().front())) == typeid (FormulaVariavel)) {

              //olhar para modelo e fazer ligacoes às variaveis

              FormulaVariavel *fv = dynamic_cast<FormulaVariavel*> (form->getFilhos().front());

              vector<Configuracao*> confVars = this->tabelaVariaveis[fv->getValue()];

              int posEstado = 0;
              int posConfigs = 0;
              for (list<Estado>::iterator it = this->modelo.begin(); it != this->modelo.end(); it++) {
                //posEstado = this->nameEstToPosVector[it->getNome()];
                list<Estado::Transicao> transTemp = it->getTransicoes();

                for (list<Estado::Transicao>::iterator it2 = transTemp.begin(); it2 != transTemp.end(); it2++) {
                  posEstado = nameEstToPosVector[it2->filho->getNome()];
                  confVars[posEstado]->addParent(configuracoes[posConfigs]);
                  configuracoes[posConfigs]->addFilho(confVars[posEstado], (it2->tipo == MUST) ? true : false);
                }

                posConfigs++;

              }

            } else { // olhar para modelo e ligar para as respectivas subformulas

              //primeiro cria-se as subformulas

              configs = controiConfigs(form->getFilhos().front());

              //olhar para modelo e fazer ligações
              int posEstado = 0;
              int posConfigs = 0;
              for (list<Estado>::iterator it = this->modelo.begin(); it != this->modelo.end(); it++, posConfigs++) {
                //posEstado = this->nameEstToPosVector[it->getNome()];
                list<Estado::Transicao> transTemp = it->getTransicoes();

                for (list<Estado::Transicao>::iterator it2 = transTemp.begin(); it2 != transTemp.end(); it2++) {
                  posEstado = nameEstToPosVector[it2->filho->getNome()];
                  configs->at(posEstado)->addParent(configuracoes[posConfigs]);
                  configuracoes[posConfigs]->addFilho(configs->at(posEstado), (it2->tipo == MUST) ? true : false);
                }

              }

              constroiArena(*configs, form->getFilhos().front());

            }


          }

        }

        void Arena::printArena() {

          string str = "\\ node (Raiz) [white] {} [->] \n";
          VisitConfiguracao *vc;
          Path *path = (new Path);
          for (int i = 0; i < 1; i++) {
            Configuracao *conf = this->cabecasTabuleiro[i];
            //vc->visit();
            str += "\t child [black]{";
            str += conf->toStr();

            cout << str << endl;

            list<Configuracao::TransicaoConfig> filhos = conf->getFilhos();
            for (list<Configuracao::TransicaoConfig>::iterator it = filhos.begin(); it != filhos.end(); it++) {
              cout << "\t child [black]{" << endl;

              vc = new VisitConfiguracao(it->destino, 1);
              vc->visit(path);

              cout << "}" << endl;
            }

            cout << "}";

          }

          cout << ";" << endl;

          path->print();

          for (list<Componente*>::iterator it = this->componentes.begin(); it != this->componentes.end(); it++) {
            cout << (*it)->toStr() << endl;
          }


        }

        Estado* Arena::estadoNaPos(int posEstate) {
          return this->posVectotToEst[posEstate];
        }

        Conectivo Arena::getTipoVariavel(string variavel) {
          return this->tipoVariavel[variavel];
        }

        void Arena::printColorsResult() {

          for (int i = 0; i< this->cabecasTabuleiro.size(); i++) {
            cout << "Raiz ";
            cout << i;
            cout << " ";

            switch (this->cabecasTabuleiro[i]->getCor()) {
              case C_TRUE:
              cout << " TRUE ";
              break;
              case C_FALSE:
              cout << " FALSE ";
              break;

              case C_UNCOLORED:
              cout << " UNCOLOR ";
              break;
              case C_INDEF:
              cout << " INDEF ";
              break;
            }

            cout << endl;
          }

        }

        set<Configuracao*, bool(*)(Configuracao*, Configuracao*) > Arena::getIndefinedConfigs() {

          bool(*fn_pt)(Configuracao*, Configuracao*) = Arena::compConfigs;
          set < Configuracao*, bool(*)(Configuracao*, Configuracao*) > configs =
          *(new set < Configuracao*, bool(*)(Configuracao*, Configuracao*)> (fn_pt));

          for (list<Configuracao*>::iterator it = this->matrizConfiguracoes.begin();
            it != this->matrizConfiguracoes.end(); it++) {

              if ((*it)->getCor() == C_INDEF) {
                configs.insert(*it);
              }
            }

            return configs;


          }

          Arena::Arena(list<Configuracao*> configs) {
            this->index = 0;
            this->matrizConfiguracoes = configs;
            this->marcarComponentesConexos();
          }

          int Arena::namesToNumEstates(string nameState) {
            return this->nameEstToPosVector[nameState];
          }

          list<Estado>* Arena::getModelo() {
            return &(this->modelo);
          }

          list<Configuracao*> Arena::getMatrizConfiguracao() {
            return this->matrizConfiguracoes;
          }

          vector<Configuracao*> Arena::getCabecasTabuleiro() {
            return this->cabecasTabuleiro;
          }

          void Arena::printArenaSimples() {

            string fim = "\033[0m\n";
            string red = "\033[1;31m";
            string green = "\033[1;32m";
            string black = "\033[1;30m";
            for (list<Configuracao*>::iterator it = this->matrizConfiguracoes.begin();
              it != this->matrizConfiguracoes.end(); it++) {
                //        if ((*it)->getCor() == C_TRUE) {
                //            cout << green;
                //        } else if ((*it)->getCor() == C_FALSE) {
                //            cout << red;
                //        } else if ((*it)->getCor() == C_INDEF) {
                //            cout << black;
                //        }
                cout << (*it)->toStr();

                //        if ((*it)->getCor() == C_TRUE || (*it)->getCor() == C_FALSE
                //                || (*it)->getCor() == C_INDEF) {
                //            cout << fim;
                //        }

                cout << endl;

                list<Configuracao::TransicaoConfig> filhos = (*it)->getFilhos();
                for (list<Configuracao::TransicaoConfig>::iterator it2 = filhos.begin();
                  it2 != filhos.end(); it2++) {

                    //            if (it2->destino->getCor() == C_TRUE) {
                    //                cout << green;
                    //            } else if (it2->destino->getCor() == C_FALSE) {
                    //                cout << red;
                    //            } else if (it2->destino->getCor() == C_INDEF) {
                    //                cout << black;
                    //            }
                    cout << "\t" << it2->destino->toStr();
                    if(it2->isMust){
                      cout << "(!)";
                    }else{
                      cout << "(?)";
                    }

                    //            if (it2->destino->getCor() == C_TRUE
                    //                    || it2->destino->getCor() == C_FALSE
                    //                    || it2->destino->getCor() == C_INDEF) {
                    //                cout << fim;
                    //            }

                    cout << endl;
                  }

                }

              }
