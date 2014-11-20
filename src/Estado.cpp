#include <list>

#include "Estado.h"

Estado::Estado(string nome, list<literalNegativo> literais) {
    this->nome = nome;
    this->literais = *(new set<literalNegativo, classcomp>);
    for (list<literalNegativo>::iterator it = literais.begin(); it != literais.end(); it++) {
        this->literais.insert(*it);
    }
    this->transicoes = *(new list<Estado::Transicao>);
}

string Estado::toStr() {
    string str = this->nome + "\n : ";

    for (set<literalNegativo>::iterator it = this->literais.begin(); it != this->literais.end(); it++) {
        str += it->literal + "-" + (it->valorLogico ? "T : " : "F : ");
    }

    for (list<Transicao>::iterator it = transicoes.begin(); it != transicoes.end(); it++) {
        str += it->filho->getNome() + " " + ((it->tipo == MUST) ? "MUST" : "MAY") + "\n";
    }

    return str;
}

void Estado::addTransicao(Transicao &t) {
    //Transicao *t2 = new Transicao;
    this->transicoes.push_back(t);
    //transicoes = (new list<Estado::Transicao>);


}

void Estado::removeTransicao(Transicao t){
    for(list<Transicao>::iterator it = this->transicoes.begin() ; 
            it != this->transicoes.end() ; it++){
        if (it->filho->getNome() == t.filho->getNome() && it->tipo == t.tipo){
            this->transicoes.erase(it);
            break;
        } 
    }
}

LogicalValues Estado::valueOfLiteral(literalNegativo ln) {
    
    set<literalNegativo>::iterator it = this->literais.find(ln);
    
    if (it == this->literais.end()) {        
        return L_INDEFINED;
    } else {
        return ( (it->valorLogico) ? L_TRUE : L_FALSE);
    }

}

void Estado::defineLiteral(literalNegativo ln){
    this->literais.insert(ln);
}

void Estado::indefineLiteral(literalNegativo ln){
    this->literais.erase(ln);
}

string Estado::getNome() {
    return this->nome;
}

list<Estado::Transicao> Estado::getTransicoes() {
    return this->transicoes;
}

list<Estado::Transicao>* Estado::getOwnTransicoes() {
    return &(this->transicoes);
}

//
//void Estado::addLiteralNegativo(literalNegativo literal){
//    this->literais.insert(literal);
//}
