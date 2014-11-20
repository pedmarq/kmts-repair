#include "Formula.h"
#include <iostream>
#include <typeinfo>

FormulaLiteral::FormulaLiteral(string literal, bool valorLogico) {
    this->literal = *(new literalNegativo);
    this->literal.literal = literal;
    this->literal.valorLogico = valorLogico;
    
}

literalNegativo FormulaLiteral::getLiteral() {
    return this->literal;
}

string FormulaLiteral::toStr() {
    return  (this->literal.valorLogico ? " " : "NOT ") + this->getLiteral().literal  ;
}

list<Formula*> FormulaLiteral::getFilhos(){
    return *(new list<Formula*>) ;
}

FormulaVariavel::FormulaVariavel(string value){
    this->value = value;
}

string FormulaVariavel::getValue(){
    return this->value;
}

string FormulaVariavel::toStr(){
    return this->getValue();
}

list<Formula*> FormulaVariavel::getFilhos(){
    return *(new list<Formula*>) ;
}

FormulaPrefixa::FormulaPrefixa(Conectivo conectivo, Formula *subFormula) {
    this->conectivo = conectivo;
    this->subFormula = subFormula;
}

string FormulaPrefixa::toStr(){
    string ret = "";
    switch  (this->getConectivo()){
        case C_AX: 
                ret = "AX";
                break;
        case C_EX :
               ret = "EX";
               break;
        case C_NOT :
            ret = "NOT";
            break;
        case C_MAXPT:
            ret = "MAXPT";
            break;
        case C_MINPT:
            ret = "MINPT";
            break;            
                
    }
    return ret;
}

Conectivo FormulaPrefixa::getConectivo(){
    return this->conectivo;
}

Formula* FormulaPrefixa::getSubformula(){
    return this->subFormula;
}

list<Formula*> FormulaPrefixa::getFilhos(){
    list<Formula*> *forms = new list<Formula*>;
    forms->push_front((this->subFormula));
    return *forms;
}

FormulaBinaria::FormulaBinaria(Conectivo conectivo, Formula *subFormulaDir, Formula *subFormulaEsq){
    this->conectivo = conectivo;
    this->subFormulaDir = subFormulaDir;
    this->subFormulaEsq = subFormulaEsq;
}
string FormulaBinaria::toStr(){
    string ret = "";
    switch  (this->getConectivo()){
        case C_AND: 
                ret = "AND";
                break;
        case C_OR :
               ret = "OR";
               break;
    }
    return ret;
}

Conectivo FormulaBinaria::getConectivo(){
    return this->conectivo;
}

Formula* FormulaBinaria::getSubformulaDir(){
    return this->subFormulaDir;
}

Formula* FormulaBinaria::getSubformulaEsq(){
    return this->subFormulaEsq;
}

list<Formula*> FormulaBinaria::getFilhos(){
    list<Formula*> *forms = new list<Formula*>;
    forms->push_front((this->subFormulaEsq));
    forms->push_front((this->subFormulaDir));
    return *forms;
}

FormulaPontoFixo::FormulaPontoFixo(Conectivo conectivo, Formula *subFormula, string variavel)
                                : FormulaPrefixa(conectivo,  subFormula) {
    this->variavel = variavel;
}

string FormulaPontoFixo::getVariavel(){
    return this->variavel;
}

string FormulaPontoFixo::toStr(){
    string ret = "";
    switch  (this->getConectivo()){
        case C_MINPT: 
                ret = "MINPT";
                break;
        case C_MAXPT:
               ret = "MAXPT";
               break;   
                
    }
    return ret + " " + this->getVariavel();
}
