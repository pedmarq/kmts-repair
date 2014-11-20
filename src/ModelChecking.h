#ifndef MODEL_CHECKING
#define	MODEL_CHECKING

#include "Configuracao.h"
#include <vector>
#include <stack>



class ModelChecking{
    
private:
        
    Arena *arena;
    vector<Componente*> componentes;
    void criarRelacioOrdemParcialComponentes();    
    
    void visitComponenteRecursivamente(int posComponente);
    stack<Configuracao*> coloredConfigs;
    
    void colorirFase_1(Configuracao *configuracao);
    void colorirFase_2(int posComponente);
    void colorirRegras_2a(Configuracao *configuracao, Conectivo tipoTestemunha,stack<Configuracao*> *pilhaTemp);
    
    
    
public:
    static bool umaInstancia;
    
    static bool ordemParcial (Componente *c1, Componente *c2){
        return c1->getRelacaoOrdem()->at(c2->getNumComponente())[c1->getNumComponente()];
    }
    
    ModelChecking(Arena *arena);
    
    vector< vector<bool> > relacaoEntreComponenetes;
    void colorir();
    
    void printOrdem();
    
    
};


#endif