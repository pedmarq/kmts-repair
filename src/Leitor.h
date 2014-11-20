/* 
 * File:   LeitorFormulas.h
 * Author: jandson
 *
 * Created on 28 de Outubro de 2013, 20:34
 */

#ifndef LEITORFORMULAS_H
#define	LEITORFORMULAS_H

#include <string>
#include <iostream>

#include "Formula.h"
#include "Estado.h"

using namespace std;

class Leitor{
    
public :
    Leitor(string caminho);
    Formula* lerFormulas();
    list<Estado> lerEstados();
    
    
};


#endif	/* LEITORFORMULAS_H */

