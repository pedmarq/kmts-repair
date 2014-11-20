#include "ModelChecking.h"
#include "Configuracao.h"
#include "Arena.h"
#include "Componente.h"
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

ModelChecking::ModelChecking(Arena* arena) {

    this->arena = arena;
    list<Componente*> comps = arena->getComponentes();
    this->componentes = *(new vector<Componente*>(comps.begin(), comps.end()));

    this->coloredConfigs = *(new stack<Configuracao*>);

    this-> criarRelacioOrdemParcialComponentes();

    stable_sort(this->componentes.begin(), this->componentes.end(), ModelChecking::ordemParcial);

    for (int i = 0; i < this->componentes.size(); i++) {
        this->componentes[i]->setNumComponente(i);
    }


    //    for(int i = 0 ; i < this->componentes.size() ; i++){
    //        cout << this->componentes[i]->toStr() << endl;
    //    }

}

void ModelChecking::criarRelacioOrdemParcialComponentes() {

    relacaoEntreComponenetes = *(new vector < vector<bool> >(this->componentes.size()));
    //
    for (int i = 0; i < relacaoEntreComponenetes.size(); i++) {
        relacaoEntreComponenetes[i] = *(new vector<bool>(this->componentes.size()));
        this->componentes[i]->setRelacao(&this->relacaoEntreComponenetes);

        list<Configuracao*> filhos = this->componentes[i]->getConfiguracoes();
        this->componentes[i]->setNumComponente(i);

        for (list<Configuracao*>::iterator it = filhos.begin(); it != filhos.end(); it++) {
            (*it)->setNumeroComponente(i);
        }

    }

    // varrer vetor de componentes e marcar suas associacoes componente a componente

    for (int i = 0; i < this->componentes.size(); i++) {
        list<Configuracao*> confgs = this->componentes[i]->getConfiguracoes();

        relacaoEntreComponenetes[i][i] = true;

        //para cada configuracao do componente
        for (list<Configuracao*>::iterator it = confgs.begin(); it != confgs.end(); it++) {
            list<Configuracao::TransicaoConfig> filhos = (*it)->getFilhos();
            //para cada filho da configuracao do componente
            for (list<Configuracao::TransicaoConfig>::iterator filho = filhos.begin();
                    filho != filhos.end(); filho++) {

                relacaoEntreComponenetes[i][filho->destino->getNumComponente()] = true;

            }
        }
    }

    //preencher recursivamente tabela de relacoes

    for (int i = 0; i < this->componentes.size(); i++) {
        if (!this->componentes[i]->foiVisitado()) {
            visitComponenteRecursivamente(i);
        }
    }


}

void ModelChecking::visitComponenteRecursivamente(int posComponente) {

    list<int> filhos;

    if (this->componentes[posComponente]->foiVisitado()) {
        return;
    }

    this->componentes[posComponente]->setVisitado(true);



    for (int i = 0; i < componentes.size(); i++) {
        if (i != posComponente && relacaoEntreComponenetes[posComponente][i]) {
            filhos.push_back(i);
            visitComponenteRecursivamente(i);
        }
    }

    //faz relacao de ordem com o filho
    for (list<int>::iterator it = filhos.begin(); it != filhos.end(); it++) {
        for (int i = 0; i < (this->componentes.size()); i++) {
            this->relacaoEntreComponenetes[posComponente][i] =
                    (this->relacaoEntreComponenetes[posComponente][i] ||
                    this->relacaoEntreComponenetes[*it][i]);
        }
    }


}

void ModelChecking::printOrdem() {

    cout << "#################COMPONENTES################" << endl;

    for (int i = 0; i < this->componentes.size(); i++) {
        cout << this->componentes[i]->toStr() << endl;
    }
}

void ModelChecking::colorir() {

    int posCFC = 0;

    list<Configuracao*> confComp = this->componentes[posCFC]->getConfiguracoes();

    list<Configuracao*> matriz = this->arena->getMatrizConfiguracao();
    for(list<Configuracao*>::iterator it = matriz.begin() ;
            it != matriz.end() ; it++){
        (*it)->setCor(C_UNCOLORED);
    }

    while (posCFC < this->componentes.size()) {

        confComp = this->componentes[posCFC]->getConfiguracoes();

        if (confComp.front()->isLiteral()) {
            Estado *e = this->arena->estadoNaPos(confComp.front()->getNumEstado());

            LogicalValues lv = e->valueOfLiteral(confComp.front()->getLiteralNegativo());

            if (lv == L_INDEFINED) {
                confComp.front()->setCor(C_INDEF);
            } else {
                if (confComp.front()->getLiteralNegativo().valorLogico &&
                        lv == L_TRUE) {
                    confComp.front()->setCor(C_TRUE);
                } else {
                    confComp.front()->setCor(C_FALSE);
                }
            }

            this->componentes[posCFC]->incrementNumConfiguracoesColoridos();
            this->coloredConfigs.push(confComp.front());

            while (!this->coloredConfigs.empty()) {

                list<Configuracao*> parents;

                Configuracao *topo = this->coloredConfigs.top();
                parents = topo->getParents();
                this->coloredConfigs.pop();

                for (list<Configuracao*>::iterator it = parents.begin();
                        it != parents.end(); it++) {
                    colorirFase_1(*it);
                }
            }

        } else {

            int numAnt = -1;

            while (numAnt < this->componentes[posCFC]->getNumConfiguracoesColoridas()
                    && this->componentes[posCFC]->getNumConfiguracoesColoridas() !=
                    this->componentes[posCFC]->getConfiguracoes().size()) {



                for (list<Configuracao*>::iterator it = confComp.begin();
                        it != confComp.end(); it++) {
                    colorirFase_1(*it);
                }
                //
                numAnt = this->componentes[posCFC]->getNumConfiguracoesColoridas();
                //
                while (!this->coloredConfigs.empty()) {

                    list<Configuracao*> parents;

                    Configuracao *topo = this->coloredConfigs.top();
                    parents = topo->getParents();
                    this->coloredConfigs.pop();

                    for (list<Configuracao*>::iterator it = parents.begin();
                            it != parents.end(); it++) {
                        colorirFase_1(*it);
                    }
                }

            }


            for (list<Configuracao*>::iterator it = confComp.begin();
                    it != confComp.end(); it++) {
                colorirFase_2(posCFC);
            }
            // TODO colorir fase2
            while (!this->coloredConfigs.empty()) {

                list<Configuracao*> parents;

                Configuracao *topo = this->coloredConfigs.top();
                parents = topo->getParents();
                this->coloredConfigs.pop();

                for (list<Configuracao*>::iterator it = parents.begin();
                        it != parents.end(); it++) {
                    colorirFase_1(*it);
                }
            }

        }

        posCFC++;

    }

}

void ModelChecking::colorirFase_1(Configuracao *configuracao) {

    if (configuracao->getCor() != C_UNCOLORED) {
        return;
    }

    if(configuracao->getConectivo() == C_MINPT){
        configuracao->setJogador(EXISTE);
    }

    list<Configuracao::TransicaoConfig> filhos = configuracao->getFilhos();

    bool cond = true;

    bool hasUncolored = false;
    bool hasNoLosingStrategy = false;

    if (configuracao->getJogador() == PARATODO) {

        switch (configuracao->getConectivo()) {
            case C_AX:
                for (list<Configuracao::TransicaoConfig>::iterator it = filhos.begin();
                        it != filhos.end(); it++) {
                    if (it->isMust && it->destino->getCor() == C_FALSE) {
                        configuracao->setCor(C_FALSE);
                        this->componentes[configuracao->getNumComponente()]->incrementNumConfiguracoesColoridos();
                        this->coloredConfigs.push(configuracao);
                        return;
                    } else if (it->destino->getCor() == C_UNCOLORED) {
                        hasUncolored = true;
                    } else {
                        cond = cond && (it->destino->getCor() == C_TRUE ? true : false);
                        if (it->destino->getCor() == C_FALSE ||
                                it->destino->getCor() == C_INDEF) {
                            hasNoLosingStrategy = true;
                        }
                    }

                }

                //se chegou a este ponto é porque ou ainda não deve ser colorido
                // ou a unica chance é jogar para não perder
                if (cond && !hasUncolored) {
                    configuracao->setCor(C_TRUE);
                    this->componentes[configuracao->getNumComponente()]->incrementNumConfiguracoesColoridos();
                    this->coloredConfigs.push(configuracao);
                } else if (!hasUncolored && hasNoLosingStrategy) {
                    configuracao->setCor(C_INDEF);
                    this->componentes[configuracao->getNumComponente()]->incrementNumConfiguracoesColoridos();
                    this->coloredConfigs.push(configuracao);
                }

                break;

            case C_AND:
                Configuracao *filho1, *filho2;
                filho1 = configuracao->getFilhos().front().destino;
                filho2 = configuracao->getFilhos().back().destino;

                if (filho1->getCor() == C_TRUE && filho2->getCor() == C_TRUE) {
                    configuracao->setCor(C_TRUE);
                    this->componentes[configuracao->getNumComponente()]->incrementNumConfiguracoesColoridos();
                    this->coloredConfigs.push(configuracao);
                } else if (filho1->getCor() == C_FALSE || filho2->getCor() == C_FALSE) {
                    configuracao->setCor(C_FALSE);
                    this->componentes[configuracao->getNumComponente()]->incrementNumConfiguracoesColoridos();
                    this->coloredConfigs.push(configuracao);
                } else if ((filho1->getCor() == C_TRUE || filho1->getCor() == C_INDEF)
                        && (filho2->getCor() == C_TRUE || filho2->getCor() == C_INDEF)) {
                    configuracao->setCor(C_INDEF);
                    this->componentes[configuracao->getNumComponente()]->incrementNumConfiguracoesColoridos();
                    this->coloredConfigs.push(configuracao);
                }

                break;

        }

    } else { // jogador existe quem joga

        Cor corFilho;

        switch (configuracao->getConectivo()) {
            case C_MAXPT:
            case C_MINPT:
            case C_NONE: // Aqui indica uma variavel, pois esta funcao
                // só é chamada para os pais das configuracoes nas pilhas


                corFilho = configuracao->getFilhos().front().destino->getCor();
                configuracao->setCor(corFilho);

                this->coloredConfigs.push(configuracao);

                this->componentes[configuracao->getNumComponente()]->incrementNumConfiguracoesColoridos();

                break;

            case C_EX:
                cond = false;

                for (list<Configuracao::TransicaoConfig>::iterator it = filhos.begin();
                        it != filhos.end(); it++) {

                    if (it->isMust && it->destino->getCor() == C_TRUE) {
                        configuracao->setCor(C_TRUE);
                        this->componentes[configuracao->getNumComponente()]->incrementNumConfiguracoesColoridos();
                        this->coloredConfigs.push(configuracao);
                        return;
                    } else if (it->destino->getCor() == C_UNCOLORED) {
                        hasUncolored = true;
                    } else {
                        cond = cond || (it->destino->getCor() == C_FALSE ? false : true);
                        if (it->destino->getCor() == C_TRUE ||
                                it->destino->getCor() == C_INDEF) {
                            hasNoLosingStrategy = true;
                        }
                    }

                }

                //se chegou a este ponto é porque ou tem um nó não colorido
                // ou possivelmente uma estrategia de não perder

                if (!hasUncolored && !cond) {
                    configuracao->setCor(C_FALSE);
                    this->componentes[configuracao->getNumComponente()]->incrementNumConfiguracoesColoridos();
                    this->coloredConfigs.push(configuracao);
                } else if (!hasUncolored && hasNoLosingStrategy) {
                    configuracao->setCor(C_INDEF);
                    this->componentes[configuracao->getNumComponente()]->incrementNumConfiguracoesColoridos();
                    this->coloredConfigs.push(configuracao);
                }


                break;

            case C_OR:

                Configuracao *filho1, *filho2;
                filho1 = configuracao->getFilhos().front().destino;
                filho2 = configuracao->getFilhos().back().destino;

                if (filho1->getCor() == C_TRUE || filho2->getCor() == C_TRUE) {
                    configuracao->setCor(C_TRUE);
                    this->componentes[configuracao->getNumComponente()]->incrementNumConfiguracoesColoridos();
                    this->coloredConfigs.push(configuracao);
                } else if (filho1->getCor() == C_FALSE && filho2->getCor() == C_FALSE) {
                    configuracao->setCor(C_FALSE);
                    this->componentes[configuracao->getNumComponente()]->incrementNumConfiguracoesColoridos();
                    this->coloredConfigs.push(configuracao);
                } else if ((filho1->getCor() == C_INDEF || filho1->getCor() == C_FALSE)
                        && (filho2->getCor() == C_INDEF || filho2->getCor() == C_FALSE)) {
                    configuracao->setCor(C_INDEF);
                    this->componentes[configuracao->getNumComponente()]->incrementNumConfiguracoesColoridos();
                    this->coloredConfigs.push(configuracao);
                }


                break;
        }

    }

}

void ModelChecking::colorirFase_2(int posComponente) {

    list<Configuracao*> configuracoes = this->componentes[posComponente]->getConfiguracoes();
    Conectivo tipoTestemunha;

    list<Configuracao*> naoColoridos;

    for (list<Configuracao*>::iterator it = configuracoes.begin(); it != configuracoes.end(); it++) {
        if ((*it)->getCor() == C_UNCOLORED) {
            naoColoridos.push_back(*it);
        }
        if ((*it)->getConectivo() == C_NONE && (*it)->getFilhos().size() == 1) {

            if ((*it)->isMaxFixPoint()) {
                tipoTestemunha = C_MAXPT;
            } else {
                tipoTestemunha = C_MINPT;
            }
        }
    }
    stack<Configuracao*> pilhaTemporaria;

    //primeiro pecorre a lista dos não coloridos pinta a depender da possibilidade

    for (list<Configuracao*>::iterator it = naoColoridos.begin();
            it != naoColoridos.end(); it++) {
        colorirRegras_2a(*it, tipoTestemunha, &pilhaTemporaria);
    }

    while (!pilhaTemporaria.empty()) {
        Configuracao *configuracao = pilhaTemporaria.top();
        pilhaTemporaria.pop();

        list<Configuracao*> parents = configuracao->getParents();

        for (list<Configuracao*>::iterator it = parents.begin();
                it != parents.end(); it++) {
            if ((*it)->getNumComponente() == posComponente) {
                colorirRegras_2a(*it, tipoTestemunha, &pilhaTemporaria);
            }
        }


    }

    //colorir nao coloridos restantes a depender do tipo da testemunha

    for (list<Configuracao*>::iterator it = naoColoridos.begin();
            it != naoColoridos.end(); it++) {
        if ((*it)->getCor() == C_UNCOLORED) {

            if (tipoTestemunha == C_MINPT) {
                (*it)->setCor(C_FALSE);
                this->componentes[(*it)->getNumComponente()]->incrementNumConfiguracoesColoridos();
                this->coloredConfigs.push(*it);
            } else {
                (*it)->setCor(C_TRUE);
                this->componentes[(*it)->getNumComponente()]->incrementNumConfiguracoesColoridos();
                this->coloredConfigs.push(*it);
            }


        }
    }

}

void ModelChecking::colorirRegras_2a(Configuracao* configuracao, Conectivo tipoTestemunha,
        stack<Configuracao*> *pilhaTemp) {

    if (configuracao->getCor() != C_UNCOLORED) {
        return;
    }

    list<Configuracao::TransicaoConfig> filhos = configuracao->getFilhos();
    Configuracao *filho1, *filho2;
    bool condicao = true;

    if (tipoTestemunha == C_MINPT) {

        switch (configuracao->getConectivo()) {

            case C_AX:
                for (list<Configuracao::TransicaoConfig>::iterator it = filhos.begin();
                        it != filhos.end(); it++) {
                    if (it->isMust) {
                        if (it->destino->getCor() == C_TRUE || it->destino->getCor() == C_INDEF) {
                            condicao = condicao && true;
                        } else {
                            condicao = condicao && false;
                            break;
                        }
                    }
                }

                if (condicao) {
                    configuracao->setCor(C_INDEF);
                    this->componentes[configuracao->getNumComponente()]->incrementNumConfiguracoesColoridos();
                    this->coloredConfigs.push(configuracao);
                    pilhaTemp->push(configuracao);
                }

                break;

            case C_AND:

                filho1 = configuracao->getFilhos().front().destino;
                filho2 = configuracao->getFilhos().back().destino;

                if (filho1->getCor() != C_UNCOLORED && filho2->getCor() != C_UNCOLORED &&
                        (filho1->getCor() == C_INDEF || filho2->getCor() == C_INDEF)) {
                    configuracao->setCor(C_INDEF);
                    this->componentes[configuracao->getNumComponente()]->incrementNumConfiguracoesColoridos();
                    this->coloredConfigs.push(configuracao);
                    pilhaTemp->push(configuracao);
                }

                break;

            case C_EX:

                for (list<Configuracao::TransicaoConfig>::iterator it = filhos.begin();
                        it != filhos.end(); it++) {
                    if (it->destino->getCor() == C_TRUE || it->destino->getCor() == C_INDEF) {
                        configuracao->setCor(C_INDEF);
                        this->componentes[configuracao->getNumComponente()]->incrementNumConfiguracoesColoridos();
                        this->coloredConfigs.push(configuracao);
                        pilhaTemp->push(configuracao);
                        break;
                    }
                }

                break;

            case C_OR:

                filho1 = configuracao->getFilhos().front().destino;
                filho2 = configuracao->getFilhos().back().destino;

                if (filho1->getCor() == C_INDEF || filho2->getCor() == C_INDEF) {
                    configuracao->setCor(C_INDEF);
                    this->componentes[configuracao->getNumComponente()]->incrementNumConfiguracoesColoridos();
                    this->coloredConfigs.push(configuracao);
                    pilhaTemp->push(configuracao);
                }

                break;

            case C_NONE:
                configuracao->setCor(C_INDEF);
                this->componentes[configuracao->getNumComponente()]->incrementNumConfiguracoesColoridos();
                this->coloredConfigs.push(configuracao);
                pilhaTemp->push(configuracao);
                break;

        }


    } else { // tipo maior ponto fixo

        switch (configuracao->getConectivo()) {

            case C_AX:

                for (list<Configuracao::TransicaoConfig>::iterator it = filhos.begin();
                        it != filhos.end(); it++) {
                    if (it->destino->getCor() == C_FALSE || it->destino->getCor() == C_INDEF) {
                        configuracao->setCor(C_INDEF);
                        this->componentes[configuracao->getNumComponente()]->incrementNumConfiguracoesColoridos();
                        this->coloredConfigs.push(configuracao);
                        pilhaTemp->push(configuracao);
                        break;
                    }
                }

                break;

            case C_AND:

                filho1 = configuracao->getFilhos().front().destino;
                filho2 = configuracao->getFilhos().back().destino;

                if (filho1->getCor() == C_INDEF || filho2->getCor() == C_INDEF) {
                    configuracao->setCor(C_INDEF);
                    this->componentes[configuracao->getNumComponente()]->incrementNumConfiguracoesColoridos();
                    this->coloredConfigs.push(configuracao);
                    pilhaTemp->push(configuracao);
                }

                break;


            case C_EX:
                condicao = true;

                for (list<Configuracao::TransicaoConfig>::iterator it = filhos.begin();
                        it != filhos.end(); it++) {

                    if (it->isMust) {

                        if (it->destino->getCor() == C_FALSE || it->destino->getCor() == C_INDEF) {
                            condicao = condicao && true;
                        } else {
                            condicao = false;
                            break;
                        }

                    }

                }

                if (condicao) {
                    configuracao->setCor(C_INDEF);
                    this->componentes[configuracao->getNumComponente()]->incrementNumConfiguracoesColoridos();
                    this->coloredConfigs.push(configuracao);
                    pilhaTemp->push(configuracao);
                }

                break;

            case C_OR:

                if (filho1->getCor() != C_UNCOLORED && filho2->getCor() != C_UNCOLORED) {
                    if (filho1->getCor() == C_INDEF || filho2->getCor() == C_INDEF) {
                        configuracao->setCor(C_INDEF);
                        this->componentes[configuracao->getNumComponente()]->incrementNumConfiguracoesColoridos();
                        this->coloredConfigs.push(configuracao);
                        pilhaTemp->push(configuracao);
                    }
                }

                break;

            case C_NONE:
                configuracao->setCor(C_INDEF);
                this->componentes[configuracao->getNumComponente()]->incrementNumConfiguracoesColoridos();
                this->coloredConfigs.push(configuracao);
                pilhaTemp->push(configuracao);
                break;


        }

    }




}
