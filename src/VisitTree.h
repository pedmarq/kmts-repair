/*
 * File:   VisitTree.h
 * Author: jandson
 *
 * Created on 28 de Outubro de 2013, 18:05
 */

#ifndef VISITTREE_H
#define	VISITTREE_H

#include <string>
#include <iostream>

#include "Formula.h"
#include "Configuracao.h"

using namespace std;

class Path{

private:

    list<string> caminhos;

public:

    void add(string str){
        caminhos.push_back(str);
    }

    void print(){

        for(list<string>::iterator it = caminhos.begin() ; it != caminhos.end() ;it++){
            cout << *it;
        }
        cout << endl;
    }




};

class VisitTree {

public:
    VisitTree(Formula *form, int numtab);
    void visit();

private:

    Formula *form;
    int numTab ;
};

class VisitConfiguracao{
public:

    VisitConfiguracao (Configuracao *config, int numtab);
    void visit(Path *path);
    Path getPath();


private:
    Configuracao *configuracao;
    int numTab;
    bool visitado;




};


#endif	/* VISITTREE_H */
