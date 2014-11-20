/* 
 * File:   RefineGame.cpp
 * Author: jan
 * 
 * Created on December 10, 2013, 4:58 PM
 */

#include "RefineGame.h"
#include "ModelChecking.h"

RefineGame::RefineGame(Arena *arena, int numEstadosModelo) {
    this->modificationsList = *(new list<SetOperations>);
    this->arena = arena;
    this->tabelaFalhasRelacionadas = *(new vector <TabelaConfigsDeFalhaRelacionadas>(numEstadosModelo));

    //bool (*fn_cmp)(TabelaConfigsDeFalhaRelacionadas, TabelaConfigsDeFalhaRelacionadas) = RefineGame::compConfigsFalhaRelacionadas;
    bool (*fn_falha)(FalhasLiteral, FalhasLiteral) = RefineGame::comFalhasLiteral;
    bool (*fn_trans)(FalhasTransicoes, FalhasTransicoes) = RefineGame::compConfigsFalhaRelacionadas;
    for (int i = 0; i < numEstadosModelo; i++) {
        this->tabelaFalhasRelacionadas[i].literaisDeFalha =
                *(new set<FalhasLiteral, bool(*)(FalhasLiteral, FalhasLiteral)>(fn_falha));
        this->tabelaFalhasRelacionadas[i].transicoesDeFalha =
                *(new set<FalhasTransicoes, bool(*)(FalhasTransicoes, FalhasTransicoes)>(fn_trans));
    }

    this->testemunhasDeFalha = getFailWitness(this->arena);
    // printRelatedFailTable(this->testemunhasDeFalha);
    startRefine();

}

list<TestemunhaDeFalha> RefineGame::getFailWitness(Arena *arena) {

    list<TestemunhaDeFalha> testemunhasDeFalha = *(new list<TestemunhaDeFalha>);

    testemunhasDeFalha = *(new list<TestemunhaDeFalha>);


    set < Configuracao*, bool(*)(Configuracao*, Configuracao*) > configs =
            arena->getIndefinedConfigs();

    list<Configuracao::TransicaoConfig*> testemunhas =
            *(new list<Configuracao::TransicaoConfig*>);

    TestemunhaDeFalha tFalha;
    Configuracao::TransicaoConfig *transConf;

    for (set < Configuracao*, bool(*)(Configuracao*, Configuracao*)>::iterator it = configs.begin();
            it != configs.end(); it++) {

        list<Configuracao::TransicaoConfig> filhos = (*it)->getFilhos();

        for (list<Configuracao::TransicaoConfig>::iterator it2 = filhos.begin();
                it2 != filhos.end(); it2++) {

            // TODO adicionar à tabela de relações de falha

            if ((it2)->destino->getCor() == C_INDEF &&
                    (it2)->destino->isLiteral()) {

                tFalha = *(new TestemunhaDeFalha);
                tFalha.origem = (*it);
                tFalha.isDuplicated = false;
                transConf = new Configuracao::TransicaoConfig;
                transConf->destino = it2->destino;
                transConf->isMust = it2->isMust;
                tFalha.destino = transConf; //&(*it2);                
                testemunhasDeFalha.push_back(tFalha);
                list<TestemunhaDeFalha>::iterator itAux = testemunhasDeFalha.end();
                itAux--;
                insertIntoRelatedFailTable(itAux);

                if ((*it)->getConectivo() == C_AX && !it2->isMust) {
                    tFalha = *(new TestemunhaDeFalha);
                    tFalha.origem = (*it);
                    tFalha.isDuplicated = true;
                    transConf = new Configuracao::TransicaoConfig;
                    transConf->destino = it2->destino;
                    transConf->isMust = it2->isMust;
                    tFalha.destino = transConf; //&(*it2);                
                    testemunhasDeFalha.push_back(tFalha);
                    list<TestemunhaDeFalha>::iterator itAux = testemunhasDeFalha.end();
                    itAux--;
                    insertIntoRelatedFailTable(itAux);
                }

            } else if (((*it)->getConectivo() == C_AX ||
                    (*it)->getConectivo() == C_EX) &&
                    !it2->isMust) {

                if (((*it)->getConectivo() == C_AX &&
                        (it2->destino->getCor() == C_FALSE ||
                        it2->destino->getCor() == C_INDEF)) ||
                        ((*it)->getConectivo() == C_EX &&
                        (it2->destino->getCor() == C_TRUE
                        || (it2->destino->getCor() == C_INDEF &&
                        it2->destino->isMaxFixPoint())))
                        ) {

                    tFalha = *(new TestemunhaDeFalha);
                    tFalha.origem = (*it);
                    tFalha.isDuplicated = false;
                    transConf = new Configuracao::TransicaoConfig;
                    transConf->destino = it2->destino;
                    transConf->isMust = it2->isMust;
                    tFalha.destino = transConf;

                    //cout << " ++++++++++++++++++++++++++++++++++++++++  " << tFalha.destino->destino->toStr() << endl; 
                    testemunhasDeFalha.push_back(tFalha);
                    list<TestemunhaDeFalha>::iterator itAux = testemunhasDeFalha.end();
                    itAux--;
                    insertIntoRelatedFailTable(itAux);
                }

            }


        }



    }

    return testemunhasDeFalha;

}

void RefineGame::insertIntoRelatedFailTable(list<TestemunhaDeFalha>::iterator itf) {

    TestemunhaDeFalha tf = *itf;
    TabelaConfigsDeFalhaRelacionadas entidade;
    TabelaConfigsDeFalhaRelacionadas novaEntidade; //= *(new TabelaConfigsDeFalhaRelacionadas);

    if (tf.destino->destino->isLiteral()) {
        FalhasLiteral fl = *(new FalhasLiteral);
        fl.confLiterais = *(new list<Configuracao*>);
        fl.confLiterais.push_back(tf.destino->destino);
        TabelaConfigsDeFalhaRelacionadas tbfr = this->tabelaFalhasRelacionadas[tf.destino->destino->getNumEstado()];
        set<FalhasLiteral, bool(*)(FalhasLiteral, FalhasLiteral) > flTemp =
                this->tabelaFalhasRelacionadas[tf.destino->destino->getNumEstado()].literaisDeFalha;
        //        
        set<FalhasLiteral, bool(*)(FalhasLiteral, FalhasLiteral)>::iterator itLit =
                flTemp.find(fl);
        //               
        if (itLit == flTemp.end()) {
            fl.testemunhasFalhasRelacionadas = *(new listItTestFail);
            fl.testemunhasFalhasRelacionadas.push_back(itf);
            this->tabelaFalhasRelacionadas[tf.destino->destino->getNumEstado()].literaisDeFalha.insert(fl);
        } else {
            itLit->confLiterais.push_back(tf.destino->destino);
            itLit->testemunhasFalhasRelacionadas.push_back(itf);
        }
    } else {

        FalhasTransicoes falTrans = *(new FalhasTransicoes);
        falTrans.numEstado = tf.destino->destino->getNumEstado();


        set <FalhasTransicoes, bool(*)(FalhasTransicoes, FalhasTransicoes) > setFalha =
                this->tabelaFalhasRelacionadas[tf.origem->getNumEstado()].transicoesDeFalha;

        set <FalhasTransicoes, bool(*)(FalhasTransicoes, FalhasTransicoes)>::iterator setIt =
                setFalha.find(falTrans);

        if (setIt == setFalha.end()) {
            falTrans.testemunhasTransicoesRelacionadas = *(new listItTestFail);
            falTrans.transicoes = *(new list<Configuracao::TransicaoConfig*>);
            falTrans.transicoes.push_back(tf.destino);
            falTrans.testemunhasTransicoesRelacionadas.push_back(itf);
            // cout << " ++++++++++++++++++++++++++++++++++++++++  " << falTrans.testemunhasTransicoesRelacionadas.back().destino->destino->toStr() << endl;
            this->tabelaFalhasRelacionadas[tf.origem->getNumEstado()].transicoesDeFalha.insert(falTrans);
        } else {
            setIt->transicoes.push_back(tf.destino);
            setIt->testemunhasTransicoesRelacionadas.push_back(itf);
            // cout << " ++++++++++++++++++++++++++++++++++++++++  " << tf.destino->destino->toStr() << endl; 
        }

    }

}

void RefineGame::printRelatedFailTable(list<TestemunhaDeFalha> testemunhas) {
 

    for (list<TestemunhaDeFalha>::iterator it = testemunhas.begin();
            it != testemunhas.end(); it++) {
        if (it->isDuplicated) {
            cout << " $$ ";
        }
        cout << "\t" << it->origem->toStr() << "\t -- " << it->destino->destino->toStr();
    }

}

void RefineGame::startRefine() {

    list<Configuracao*> matriz = this->arena->getMatrizConfiguracao();
    list<Estado> *modeloFromArena = this->arena->getModelo();

    map<string, Estado*> mapModeloAuxiliar = *(new map<string, Estado*>);
    this->modelo = *(new list<Estado*>);

    for (list<Estado>::iterator itEst = modeloFromArena->begin();
            itEst != modeloFromArena->end(); itEst++) {
        this->modelo.push_back(&(*itEst));
    }
    
    //printRelatedFailTable(this->testemunhasDeFalha);

    for (list<TestemunhaDeFalha>::iterator itFails = this->testemunhasDeFalha.begin();
            itFails != this->testemunhasDeFalha.end(); itFails++) {

        SetOperations setModifications = *(new SetOperations);
        refineGame(*(new list<TestemunhaDeFalha>(itFails, this->testemunhasDeFalha.end())),
                setModifications);

        OperationStruct ops = refinePLay(*itFails);
        undoOperation(this->modelo, ops);

    }

    //Arena *ar = new Arena(*(this->arena->getModelo()), this->arena->getForm());

    //cout << "CONCLUIDO" << endl;
    int i = 0;
//    for(list<SetOperations>::iterator it = this->modificationsList.begin() ; 
//            it != this->modificationsList.end() ; it++){
//        
//        cout << "X" << i << endl;
//        cout << endl;
//        
//        it->print();
//        
//        cout << "#############################################" << endl;
//        i++;
//        
//    }

}

list<Estado> RefineGame::prepareModelToArena() {

    list<Estado> modelArena = *(new list<Estado>);

    for (list<Estado*>::iterator it = this->modelo.begin();
            it != this->modelo.end(); it++) {
        modelArena.push_back(**it);
    }

    return modelArena;


}

void RefineGame::refineGame(list<TestemunhaDeFalha> testemunhas, SetOperations setOperations) {

//    printRelatedFailTable(testemunhas);
//    cout << "##################################" << endl;

    TestemunhaDeFalha failure = testemunhas.front();

    OperationStruct change = refinePLay(failure);

    //    if (change.operation == OP_DEFN_LIT) {
    //        cout << "DF LITERAL" << endl;
    //        cout << change.literal.literal << endl;
    //    } else if (change.operation == OP_MAY_TO_MUST) {
    //        cout << "MAY TO MUST" << endl;
    //    } else if (change.operation == OP_REMOVE_MAYG) {
    //        cout << "REMOVE MAY" << endl;
    //    }
    //
    //    for (list<Estado*>::iterator it = this->modelo.begin();
    //            it != this->modelo.end(); it++) {
    //        cout << (*it)->toStr() << endl;
    //    }

    setOperations.add(change);

    applyOperation(this->modelo, change);

    Arena *arena = new Arena(prepareModelToArena(), this->arena->getForm());

    ModelChecking md = *(new ModelChecking(arena));

    list<Estado> preparado = prepareModelToArena();

    //    cout << "$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
    //    
    //        for (list<Estado>::iterator it = preparado.begin() ;
    //                it != preparado.end() ; it++){
    //            cout << (it)->toStr() << endl;
    //        }

    md.colorir();

    if (arena->getCabecasTabuleiro()[0]->getCor() == C_TRUE) {
        this->modificationsList.push_back(setOperations.copy());
//        
//        string str ="";
        for(list<Estado*>::iterator itP = this->modelo.begin() ; itP != this->modelo.end() ;itP++){
            cout << (*itP)->getNome() + " ( "  + (*itP)->imprimirEstado() + " )" << endl;            
        }
        
        for(list<Estado*>::iterator itP = this->modelo.begin() ; itP != this->modelo.end() ;itP++){
                       
            cout << (*itP)->imprimirTransicoes() << endl;
        }
        
        
        
        cout << "-------------------------------------------------------------------" << endl;
//        cout << " ALTERACAO FEITA DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD" << endl;
//        setOperations.print();
    } else if (arena->getCabecasTabuleiro()[0]->getCor() == C_INDEF) {
        list<TestemunhaDeFalha> nWitness = getFailWitness(arena);
        for (list<TestemunhaDeFalha>::iterator it = nWitness.begin();
                it != nWitness.end(); it++) {
            refineGame(*(new list<TestemunhaDeFalha>(it, nWitness.end())), setOperations);
            undoOperation(this->modelo, refinePLay(*it));
        }
    }


}

OperationStruct RefineGame::refinePLay(TestemunhaDeFalha testemunha) {

    OperationStruct ops = *(new OperationStruct);

    ops.operation = OP_NULL;

    if (testemunha.origem->getConectivo() == C_EX &&
            (testemunha.destino->destino->getCor() == C_TRUE ||
            testemunha.destino->destino->isMaxFixPoint())) {


        ops.init(testemunha.origem->getNumEstado(), testemunha.destino->destino->getNumEstado(),
                *(new literalNegativo), OP_MAY_TO_MUST);

        // setOperations.transMaysToMus.push_back(ops);

    } else if (testemunha.origem->getConectivo() == C_AX) {
        if (testemunha.destino->destino->getCor() == C_FALSE ||
                !testemunha.destino->destino->isLiteral()) {

            ops.init(testemunha.origem->getNumEstado(),
                    testemunha.destino->destino->getNumEstado(),
                    *(new literalNegativo), OP_REMOVE_MAYG);

            // setOperations.removedMays.push_back(ops);

        } else if (testemunha.destino->destino->getCor() == C_INDEF
                && testemunha.destino->destino->isLiteral()) {

            if (!testemunha.isDuplicated) {
                ops.init(testemunha.origem->getNumEstado(),
                        testemunha.destino->destino->getNumEstado(),
                        testemunha.destino->destino->getLiteralNegativo(),
                        OP_DEFN_LIT);
                //   setOperations.definedLiterals.push_back(ops);
            } else {
                ops.init(testemunha.origem->getNumEstado(),
                        testemunha.destino->destino->getNumEstado(),
                        *(new literalNegativo), OP_REMOVE_MAYG);
                //   setOperations.removedMays.push_back(ops);
            }
        }
    } else if (testemunha.destino->destino->isLiteral()) {
        ops.init(testemunha.origem->getNumEstado(),
                testemunha.destino->destino->getNumEstado(),
                testemunha.destino->destino->getLiteralNegativo(), OP_DEFN_LIT);
        // setOperations.definedLiterals.push_back(ops);
    }

    return ops;

}

void RefineGame::undoOperation(list<Estado*> modelo, OperationStruct operation) {

    for (list<Estado*>::iterator itEst = modelo.begin(); itEst != modelo.end();
            itEst++) {

        if (operation.operation == OP_DEFN_LIT) {

            if (this->arena->namesToNumEstates((*itEst)->getNome()) == operation.to) {
                (*itEst)->indefineLiteral(operation.literal);
                break;
            }

        } else if (this->arena->namesToNumEstates((*itEst)->getNome()) == operation.from) {
            list<Estado::Transicao> transitions = (*itEst)->getTransicoes();

            if (operation.operation == OP_REMOVE_MAYG) {

                Estado::Transicao trans = *(new Estado::Transicao);

                for (list<Estado*>::iterator it = this->modelo.begin();
                        it != this->modelo.end(); it++) {
                    int numEstado = this->arena->namesToNumEstates((*it)->getNome());

                    if (numEstado == operation.to) {
                        trans.filho = (*it);
                        trans.tipo = MAY;
                        (*itEst)->addTransicao(trans);
                        break;
                    }

                }
            } else {
                for (list<Estado::Transicao>::iterator itTrans = transitions.begin();
                        itTrans != transitions.end(); itTrans++) {
                    if (this->arena->namesToNumEstates(itTrans->filho->getNome()) ==
                            operation.to) {
                        if (operation.operation == OP_MAY_TO_MUST) {
                            itTrans->tipo = MAY;
                            break;
                        }
                    }
                }
            }
        }        
    }

}

void RefineGame::applyOperation(list<Estado*> modelo, OperationStruct operation) {

    for (list<Estado*>::iterator itEst = modelo.begin(); itEst != modelo.end();
            itEst++) {

        if (operation.operation == OP_DEFN_LIT) {
            if (this->arena->namesToNumEstates((*itEst)->getNome()) == operation.to) {
                (*itEst)->defineLiteral(operation.literal);
                //                cout << "++++++++++++++++++++++++++++++++++++++++++++++" << endl;
                //                cout << (*itEst)->toStr() << endl;
                break;
            }
        } else if (this->arena->namesToNumEstates((*itEst)->getNome()) == operation.from) {
            //cout << "9999999999999999999 transicao FROM **********" << endl;
            list<Estado::Transicao> *transitions = (*itEst)->getOwnTransicoes();
            for (list<Estado::Transicao>::iterator itTrans = transitions->begin();
                    itTrans != transitions->end(); itTrans++) {
                if (this->arena->namesToNumEstates(itTrans->filho->getNome()) ==
                        operation.to) {
                    if (operation.operation == OP_MAY_TO_MUST) {
                       // cout << "************************ achado **********" << endl;
                        itTrans->tipo = MUST;
                    } else {
                        (*itEst)->removeTransicao(*itTrans);
                    }

                    break;
                }
            }
            break;
        }





    }


}

RefineGame::~RefineGame() {
}

