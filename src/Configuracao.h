#ifndef CONFIGURACAO_H
#define	CONFIGURACAO_H

#include "Formula.h"
#include "Estado.h"
#include <vector>
#include <map>
#include <sstream>

typedef enum {
    PARATODO,
    EXISTE
} Jogador;

typedef enum {
    C_UNCOLORED, C_TRUE, C_FALSE, C_INDEF
} Cor;

class Configuracao {
public:

    typedef struct {
        Configuracao *destino;
        bool isMust;

    } TransicaoConfig;

private:
    Jogador jogador;
    Cor cor;
    Conectivo conectivo;
    literalNegativo literal;
    int numEstado;
    list<TransicaoConfig> filhos;
    int numeroNome;
    int numeroComponente;
    int lowIndex;
    list<Configuracao*> parents;
    int numConfigsColoridas;
    bool intoStack;
    bool visited;
    
    int index;

public:

    Jogador getJogador();
    void setJogador(Jogador jog);
    Cor getCor();
    Conectivo getConectivo();
    Configuracao(Conectivo conectivo, int numEstado, int numeroNome);
    Configuracao(literalNegativo literal, int numEstado, int numeroNome);
    void setCor(Cor cor);
    string toStr();
    int getNumNome();
    void addFilho(Configuracao *filho);
    void addFilho(Configuracao *filho, bool transicaoIsMust);
    list<TransicaoConfig> getFilhos();

    int getNumComponente();
    void setNumeroComponente(int numComponente);

    int getLowIndex();
    void setLowIndex(int numLowIndex);

    void addParent(Configuracao *parent);

    list<Configuracao*> getParents();

    int getNumEstado();

    literalNegativo getLiteralNegativo();
    
    bool isLiteral();
    bool isVariavel();
    bool isMaxFixPoint();
    bool isMinFixPoint();
    
    bool isIntoStack();    
    void setIntoStack(bool valor);
    
    bool wasVisited();
    void setVisited(bool value);
    
    int getIntex();
    void setIndex(int index);

};

class Componente {
private:
    list<Configuracao*> configuracoes;
    int numConfiguracoesColoridas;
    int numComponente;
    bool visitado;
    vector< vector<bool> > *relacaoEntreComponenetes;
    Conectivo testemunhaCiclo;

public:

    Componente(int numeroComponente) {
        this->configuracoes = *(new list<Configuracao*>);
        this->numComponente = numeroComponente;
        this->visitado = false;
        this->numConfiguracoesColoridas = 0;
    }
    
    void incrementNumConfiguracoesColoridos(){
        this->numConfiguracoesColoridas++;
    }
    
    void decrementNumConfiguracoesColoridos(){
        this->numConfiguracoesColoridas--;
    }

    list<Configuracao*> getConfiguracoes() {
        return this->configuracoes;
    }

    void addConfiguracao(Configuracao *configuracao) {        
        this->configuracoes.push_back(configuracao);
    }

    string toStr() {
        stringstream ss;
        ss << "### Numero Componente :: " << this->numComponente << endl;
        for (list<Configuracao*>::iterator it = this->configuracoes.begin();
                it != this->configuracoes.end(); it++) {
            ss << "\t" << (*it)->toStr() << endl;

            list<Configuracao::TransicaoConfig> filhos = (*it)->getFilhos();


            for (list<Configuracao::TransicaoConfig>::iterator itFil = filhos.begin();
                    itFil != filhos.end(); itFil++) {
                ss << "\t\t" << (itFil->destino)->toStr() << endl;
            }

        }

        return ss.str();
    }

    vector< vector<bool> > * getRelacaoOrdem() {
        return this->relacaoEntreComponenetes;
    }

    void setRelacao(vector< vector<bool> > *relacaoEntreComponenetes) {
        this->relacaoEntreComponenetes = relacaoEntreComponenetes;
    }

    bool foiVisitado() {
        return this->visitado;
    }

    void setVisitado(bool value) {
        this->visitado = value;
    }

    int getNumComponente() {
        return this->numComponente;
    }

    void setNumComponente(int num) {
        this->numComponente = num;
        for(list<Configuracao*>::iterator it = this->configuracoes.begin() ;
                it != this->configuracoes.end() ; it++){
            (*it)->setNumeroComponente(num);
        }
    }
    
    int getNumConfiguracoesColoridas(){
        
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


};

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
    void printArean();
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


#endif	/* COFIGURACAO_H */