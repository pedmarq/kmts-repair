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

    int getIndex();
    void setIndex(int index);

};

#endif	/* COFIGURACAO_H */
