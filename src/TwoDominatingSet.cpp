#include "TwoDominatingSet.h"

TwoDominatingSet::TwoDominatingSet()
{
    srand(time(0)); // Inicializa a semente para números aleatórios
}

TwoDominatingSet::~TwoDominatingSet()
{
    // Destrutor vazio
}

vector<char> TwoDominatingSet::Guloso(Grafo* grafo) {    
    vector<char> dominatingSet;
    map<char, int> ehDominadoQtd;
    vector<pair<float, char>> pq;
    // Ordena os nós por grau (nós com mais arestas primeiro)
    for(No* no : grafo->lista_adj){
        // O nó é melhor se tiver mais arestas, ou seja, ordem - grau é o critério de ordenação
        pq.push_back({(grafo->ordem - no->getGrau()), no->getID()});

        ehDominadoQtd[no->getID()] = 0; // Inicializa como não dominado
    }

    
    while(!pq.empty()){
        sort(pq.begin(), pq.end()); // Ordena a lista de prioridades

        char idNo = pq[0].second;
        pq.erase(pq.begin()); 
        
        No* noAtual = grafo->getNo(idNo);
        
        int contribuicao = 0;
        bool deveAdicionar = ehDominadoQtd[idNo] < 2; // Verifica se o nó deve ser adicionado ao conjunto dominante
        if(deveAdicionar) contribuicao++;
        for(Aresta* n : noAtual->arestas){
            char idAlvo = n->getIDalvo();
            if(ehDominadoQtd[idAlvo] < 2){
                ehDominadoQtd[idAlvo]++;
                deveAdicionar = true;
                contribuicao++;
            }
        }

        if(deveAdicionar){
            dominatingSet.push_back(idNo);
            ehDominadoQtd[idNo] += 2; // O próprio nó também é dominado
        }

        for(auto it = pq.begin(); it != pq.end(); ++it){
            char idAtual = it->second;
            No* n = grafo->getNo(idAtual);
            float contribuicao = ehDominadoQtd[idAtual] < 2 ? 1 : 0; // Se já está dominado, não conta
            for(Aresta* a : n->arestas){
                char idAlvo = a->getIDalvo();
                if(ehDominadoQtd[idAlvo] < 2){
                    contribuicao++;
                    if(ehDominadoQtd[idAlvo] == 1){
                        contribuicao += 0.5; // Bônus para nós que já estão parcialmente dominados
                    }
                }
            }
            it->first = grafo->ordem - contribuicao; // Atualiza a prioridade
        }
    }
    return dominatingSet;
}

vector<char> TwoDominatingSet::auxGulosoRandomizadoAdaptativo(Grafo* grafo, float alpha){
    vector<char> dominatingSet;
    map<char, int> ehDominadoQtd;
    vector<pair<float, char>> pq;

    for(No* no : grafo->lista_adj){
        // O nó é melhor se tiver mais arestas, ou seja, ordem - grau é o critério de ordenação
        pq.push_back({(grafo->ordem - no->getGrau()), no->getID()});
        ehDominadoQtd[no->getID()] = 0; // Inicializa como não dominado
    }
    
    // Ordena a prioridade dos nós: Esse sort pega a first e compara, se for igual, pega a second
    while(!pq.empty()){
        // Ordena a lista de prioridades
        sort(pq.begin(), pq.end());
        
        // Calcula o índice de acordo com o alpha, mas garantindo que não seja menor que 1
        int maiorIndex = max(1, (int)(pq.size() * alpha));
        maiorIndex = min(maiorIndex, (int)pq.size()); 
        int index = rand() % maiorIndex;

        char idNo = pq[index].second;
        pq.erase(pq.begin() + index); // Remove o nó escolhido da lista de prioridades

        No* noAtual = grafo->getNo(idNo);
        bool deveAdicionar = ehDominadoQtd[idNo] < 2; // Verifica se o nó deve ser adicionado ao conjunto dominante inicialmente
        for(Aresta* n : noAtual->arestas){
            char idAlvo = n->getIDalvo();
            if(ehDominadoQtd[idAlvo] < 2){
                ehDominadoQtd[idAlvo]++; // Aqui eu posso já aumentar, já que ele será adicionado aqui
                deveAdicionar = true;
            }
        }
        // Se deve adicionar, adiciona ao conjunto dominante
        if(deveAdicionar){
            dominatingSet.push_back(idNo);
            ehDominadoQtd[idNo] += 2; // O próprio nó também é dominado
        }

        // Atualiza as prioridades dos nós restantes na lista
        for(auto it = pq.begin(); it != pq.end(); ++it){
            char idAtual = it->second;
            No* n = grafo->getNo(idAtual);
            float contribuicao = ehDominadoQtd[idAtual] < 2 ? 1 : 0; // Se já está dominado, não conta
            for(Aresta* a : n->arestas){
                char idAlvo = a->getIDalvo();
                if(ehDominadoQtd[idAlvo] < 2){
                    contribuicao++;
                    if(ehDominadoQtd[idAlvo] == 1){
                        contribuicao += 0.5; // Bônus para nós que já estão parcialmente dominados
                    }
                }
            }
            it->first = grafo->ordem - contribuicao; // Atualiza a prioridade
        }

    }
    return dominatingSet;
}

vector<char> TwoDominatingSet::GulosoRandomizadoAdaptativo(Grafo* grafo, float alpha, int numIter) {
    vector<char> melhorSolucao = auxGulosoRandomizadoAdaptativo(grafo, alpha);

    for(int i = 1; i < numIter; i++) {
        vector<char> solucaoAtual = auxGulosoRandomizadoAdaptativo(grafo, alpha);
        
        // Se a solução atual é melhor, atualiza a melhor solução
        if(solucaoAtual.size() < melhorSolucao.size()) {
            melhorSolucao = solucaoAtual;
        }
    }
    return melhorSolucao;
}

vector<char> TwoDominatingSet::GulosoRandomizadoAdaptativoReativo(Grafo* grafo, float vetAlphas[], int tamAlpha, int numIter, int bloco) {
    vector<char> melhorSolucao;
    meuAlpha* alphas = new meuAlpha[tamAlpha];
    
    for (int i = 0; i < tamAlpha; i++) {
        alphas[i].alpha = vetAlphas[i];
        alphas[i].probabilidade = 1.0f / tamAlpha; // Probabilidade inicial uniforme
        alphas[i].media = 0.0f;
        alphas[i].contador = 1; // Novo: Contador para média móvel
    }

    // Primeira iteração com cada um
    for (int i = 0; i < tamAlpha; i++) {
        vector<char> solucaoAtual = GulosoRandomizadoAdaptativo(grafo, alphas[i].alpha);
        
        if (conjuntoDominanteValido(solucaoAtual, grafo)) {
            // Se a solução é válida, armazena o tamanho dela como média
            alphas[i].media = solucaoAtual.size();
            if (melhorSolucao.empty() || solucaoAtual.size() < melhorSolucao.size()) {
                melhorSolucao = solucaoAtual;
            }
        } else {
            // Penalização: atribui um valor alto (ordem do grafo) para alphas que geram soluções inválidas
            // Isso evita divisão por zero e penaliza alphas ruins
            alphas[i].media = grafo->ordem;
            cout << "Solução inválida com alpha=" << alphas[i].alpha << ". Atribuído valor de penalização." << endl;
        }
    }

  
    for(int i = 0; i < numIter; i++) {
        // Atualiza as probabilidades a cada bloco de iterações
        if(i % bloco == 0) {
            atualizaProbabilidades(alphas, melhorSolucao.size(), tamAlpha, grafo->ordem);
        }

        
        int indexAlpha = pegarAlpha(alphas, tamAlpha);
        meuAlpha& alphaAtual = alphas[indexAlpha]; 

     
        vector<char> solAtual = GulosoRandomizadoAdaptativo(grafo, alphaAtual.alpha);

        // Antes: Atualização simples da média
        alphaAtual.media = (alphaAtual.media + solAtual.size())/(++alphaAtual.contador);
 
        // Atualiza a melhor solução global
        if(melhorSolucao.empty() || solAtual.size() < melhorSolucao.size()) {
            melhorSolucao = solAtual;
        }
    }

    delete[] alphas;
    return melhorSolucao;
}

// FUNÇÕES AUXILIARES
int TwoDominatingSet::pegarAlpha(meuAlpha alphas[], int quantidade){
    float random = (float)rand() / RAND_MAX; // Gera um número entre 0 e 1
    float soma = 0.0f;
    int i = 0;
    for(; i < quantidade; i++){
        soma += alphas[i].probabilidade;
        if(random <= soma){
            return i;
        }
    }
    return i - 1; // Retorna o último índice se não encontrou (caso raro)
}

void TwoDominatingSet::atualizaProbabilidades(meuAlpha alphas[], int tamanhoMelhorSolucao, int quantidade, int tamanhoGrafo){
    vector<float> Q(quantidade, 0.0f);
    float sumQ = 0.0f;

    // Calcula a qualidade de cada alpha
    for(int i = 0; i < quantidade; i++){
        // Quanto melhor a solução, menos nós tem do grafo.
        // Quanto menor a média (melhor a solução), maior a qualidade
        Q[i] = (tamanhoGrafo - tamanhoMelhorSolucao) / alphas[i].media;
        sumQ += Q[i];
    }

    // Atualiza as probabilidades
    for(int i = 0; i < quantidade; i++){
        alphas[i].probabilidade = Q[i] / sumQ;
    }
}

bool TwoDominatingSet::estaDominado(char id, const vector<char>& conjuntoDominante, Grafo* grafo) {
    int cont = 0;
    for (char idDominador : conjuntoDominante) {
        if(grafo->getNo(idDominador)->isAdjacent(id)) {
            cont++;
        }
    }
    return cont >= 2; // 2-dominating: pelo menos dois dominadores
}

bool TwoDominatingSet::conjuntoDominanteValido(const vector<char>& conjuntoDominante, Grafo* grafo) {
    for (No* no : grafo->lista_adj) {
        // Se o nó está no conjunto dominante, não precisa ser dominado
        if(find(conjuntoDominante.begin(), conjuntoDominante.end(), no->getID()) != conjuntoDominante.end()) {
            continue;
        }
        
        // Verifica se o nó é 2-dominado
        if (!estaDominado(no->getID(), conjuntoDominante, grafo)) {
            return false;
        }
    }
    return true;
}