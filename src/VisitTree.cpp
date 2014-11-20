#include <sstream>

#include "VisitTree.h"

VisitTree::VisitTree(Formula* form, int numtab){
    this->numTab = numtab;
    this->form = form;
}

void VisitTree::visit(){
    for(int i = 0 ; i < numTab ; i++){
        cout << "    ";
    }
    cout << this->form->toStr() << endl;
  //  numTab++;
    list<Formula*> filhos = this->form->getFilhos();
    for(list<Formula*>::iterator it = filhos.begin() ; it != filhos.end() ; it++){
        //int temp = numTab;
        //numTab = temp;
        VisitTree *t = new VisitTree(*it, numTab + 1);
        t->visit();
    }
    
}



VisitConfiguracao::VisitConfiguracao(Configuracao *config, int numtab){
    this->configuracao = config;
    this->numTab = numtab;
}

void VisitConfiguracao::visit(Path *path){
//    for(int i = 0 ; i < numTab ; i++){
//        cout << "    ";
//    }
    
    cout << this->configuracao->toStr() << endl;
//    
    list<Configuracao::TransicaoConfig> filhos = this->configuracao->getFilhos();
    
//    cout << "numero de filhos " << endl;
//    cout << this->configuracao->getFilhos().size() << endl;
    
//    for(list<Configuracao::TransicaoConfig>::iterator it = filhos.begin() ; it != filhos.end() ; it++){
////        int temp = numTab;
////        numTab = temp;
//          for(int i = 0 ; i <= numTab ; i++){
//              cout << "    ";              
//          }
//          cout << it->destino->toStr() << endl;
//          
//          
//          
//    //    VisitConfiguracao *t = new VisitConfiguracao(*it, 0);
//     //   t->visit();
//    }
    
        for(list<Configuracao::TransicaoConfig>::iterator it = filhos.begin() ; 
                it != filhos.end() ; it++){
            
            if( (this->configuracao->getConectivo() == C_AX || 
                    this->configuracao->getConectivo() == C_EX) && 
                    this->configuracao->getNumNome() > 
                    this->configuracao->getFilhos().front().destino->getNumNome() ) {
                stringstream ss ;
                ss.str("");
                
                list<Configuracao::TransicaoConfig> filhos = this->configuracao->getFilhos();
                
                for(list<Configuracao::TransicaoConfig>::iterator it = filhos.begin() ; it != filhos.end(); it++){
                    ss << "\\path [style=dashed,red, ->] (" << this->configuracao->getNumNome() 
                        <<") edge (" << it->destino->getNumNome() << ");\n"; 
                }
                
                path->add(ss.str());               
               
                
                return;                
            }
           
            string str = "";
//            for(int i = 0 ; i < numTab ;i++){
//                str += "\t";
//            }
            
            cout <<  "child [black]{" << endl;
            
            VisitConfiguracao *vc = new VisitConfiguracao(it->destino,numTab+1);
                        
            str = "";
            for(int i = 0 ; i < numTab ;i++){
                str += "\t";
            }
            
            vc->visit(path);
            
            cout << "}\n";
            
        }
    
//    for(list<Configuracao::TransicaoConfig>::iterator it = filhos.begin() ; it != filhos.end() ; it++){
////        int temp = numTab;
////        numTab = temp;
////          for(int i = 0 ; i <= numTab ; i++){
////              cout << "    ";              
////          }
//        //  cout << (*it)->toStr() << endl;
//         
//        VisitConfiguracao *t = new VisitConfiguracao(it->destino, 0);       
//        t->visit();
//    }
}
