#ifndef FORMULA_H
#define FORMULA_H

#include <string>
#include <list>

#include "Estado.h"

using namespace std;

namespace ConstantConectivo{
enum Conectivo {
    C_AX,
    C_EX,
    C_OR,
    C_AND,
    C_NOT,
    C_MAXPT,
    C_MINPT,
    C_NONE
};
}

using namespace ConstantConectivo;


class Formula {
public:
    virtual string toStr(){
        return "afv";
    }
    
    virtual list<Formula*> getFilhos () {
        return *(new list<Formula*>);
        
    }

};

class FormulaLiteral : public Formula {
public:
    FormulaLiteral(string literal, bool valorLogico);
    literalNegativo getLiteral();
    string toStr();
    list<Formula*> getFilhos ();

private:

    literalNegativo literal;

};

class FormulaVariavel : public Formula {
public:
    FormulaVariavel(string value);
    string getValue();
    string toStr();
    list<Formula*> getFilhos ();

private:

    string value;

};

class FormulaPrefixa : public Formula {
private:
    Conectivo conectivo;
    Formula *subFormula;

public:
    FormulaPrefixa(Conectivo conectivo, Formula *subFormula);
    Conectivo getConectivo();
    Formula* getSubformula();
    virtual string toStr();
    list<Formula*> getFilhos ();

};

class FormulaBinaria : public Formula {
private:
    Conectivo conectivo;
    Formula *subFormulaDir;
    Formula *subFormulaEsq;

public:
    FormulaBinaria(Conectivo conectivo, Formula *subFormulaDir, Formula *subFormulaEsq);
    Conectivo getConectivo();
    Formula *getSubformulaDir();
    Formula *getSubformulaEsq();
    string toStr();
    list<Formula*> getFilhos ();

};

class FormulaPontoFixo : public FormulaPrefixa {
private:
    string variavel;

public:
    FormulaPontoFixo(Conectivo conectivo, Formula *subFormula, string variavel);
    
    string getVariavel();
    string toStr();
    

};


#endif