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

    vector<char> dominatingSet; // Conjunto solução
    vector<char> naoDominados;  // Nós ainda não 2-dominados

    // Inicializa todos os nós como não dominados
    for(No* no : grafo->lista_adj){
        naoDominados.push_back(no->getID());
    }

    while(!naoDominados.empty()){
        char melhorNo = ' ';
        int melhorContribuicao = -1;

        for(No* candidato : grafo->lista_adj){
            char idCandidato = candidato->getID();

            // Funcao pra ignorar se estiver no conjunto dominante
            if(find(dominatingSet.begin(), dominatingSet.end(), idCandidato) != dominatingSet.end())
                continue;

            int contribuicao = 0;
            // Função para avaliar quantos ele ajuda a dominar
            for(char alvo : naoDominados){
                vector<char> conjuntoProvisorio = dominatingSet;
                conjuntoProvisorio.push_back(idCandidato);
                if(isDominated(alvo, conjuntoProvisorio, grafo)){
                    contribuicao++;
                }
            }

            // Atualiza o melhor candidato (guloso)
            if(contribuicao > melhorContribuicao){
                melhorContribuicao = contribuicao;
                melhorNo = idCandidato;
            }
        }

        // Adiciona o vértice escolhido ao conjunto dominante
        conjuntoDominante.push_back(melhorNo);

        // Atualiza a lista de não dominados
        vector<char> novosNaoDominados;
        for(char alvo : naoDominados){
            if(!estaDominado(alvo, conjuntoDominante, grafo)){
                novosNaoDominados.push_back(alvo);
            }
        }
        naoDominados = novosNaoDominados;
    }

    return conjuntoDominante;
}



vector<char> TwoDominatingSet::GulosoRandomizadoAdaptativo(Grafo* grafo, float alpha) {
    vector<char> conjuntoDominante; // Conjunto solução
    vector<char> naoDominados;  // Nós não dominados

    // Inicializa 
    for(No* no : grafo->lista_adj){
        naoDominados.push_back(no->getID());
    }

    while(!naoDominados.empty()){
        vector<pair<char, int>> candidatos; // Lista de candidatos com suas contribuições
        int melhorContribuicao = -1;

        // Avalia cada nó candidato
        for(No* candidato : grafo->lista_adj){
            char idCandidato = candidato->getID();

            // Ignora nós já no conjunto solução
            if(find(conjuntoDominante.begin(), conjuntoDominante.end(), idCandidato) != conjuntoDominante.end())
                continue;

            int contribuicao = 0;
            // Calcula quantos não dominados seriam cobertos por este candidato
            for(char alvo : naoDominados){
                vector<char> conjuntoProvisorio = conjuntoDominante;
                conjuntoProvisorio.push_back(idCandidato);
                if(isDominated(alvo, conjuntoProvisorio, grafo)){
                    contribuicao++;
                }
            }

            // Só considera candidatos com contribuição positiva
            if(contribuicao > 0) {
                candidatos.push_back(make_pair(idCandidato, contribuicao));
                // Atualiza a melhor contribuição encontrada
                if(contribuicao > melhorContribuicao) {
                    melhorContribuicao = contribuicao;
                }
            }
        }

        // Se não há candidatos, sai do loop
        if(candidatos.empty()) 
            break;

        // Construir a Lista Restrita de Candidatos
        vector<char> ListaCandidatos;
        float limite = melhorContribuicao - alpha * melhorContribuicao;

        for(auto& par : candidatos) {
            if(par.second >= limite) {
                ListaCandidatos.push_back(par.first);
            }
        }

        // Seleciona aleatoriamente um nó da ListaCandidatos
        char noEscolhido;
        if(!ListaCandidatos.empty()) {
            int indiceAleatorio = rand() % ListaCandidatos.size();
            noEscolhido = ListaCandidatos[indiceAleatorio];
        } else {
            // Fallback: escolhe o candidato com a maior contribuição
            auto melhorCandidato = max_element(candidatos.begin(), candidatos.end(), 
                [](const pair<char, int>& a, const pair<char, int>& b) {
                    return a.second < b.second;
                });
            noEscolhido = melhorCandidato->first;
        }

        // Adiciona o nó escolhido ao conjunto solução
        conjuntoDominante.push_back(noEscolhido);

        // Atualiza a lista de não dominados
        vector<char> novosNaoDominados;
        for(char alvo : naoDominados) {
            if(!estaDominado(alvo, conjuntoDominante, grafo)) {
                novosNaoDominados.push_back(alvo);
            }
        }
        naoDominados = novosNaoDominados;
    }

    return conjuntoDominante;
}



vector<char> TwoDominatingSet::GulosoRandomizadoAdaptativoReativo(Grafo* grafo, float vetAlphas[], int tamAlpha, int numIter, int bloco) {
    vector<char> melhorSolucao;
    meuAlpha* alphas = new meuAlpha[tamAlpha];
    
    // Antes: Inicialização simplificada sem contador
    // Agora: Inicialização completa de todas as variáveis
    for (int i = 0; i < tamAlpha; i++) {
        alphas[i].alpha = vetAlphas[i];
        alphas[i].probabilidade = 1.0f / tamAlpha; // Probabilidade inicial uniforme
        alphas[i].media = 0.0f;
        alphas[i].contador = 1; // Novo: Contador para média móvel
    }

    // Primeira iteração com cada um
    for (int i = 0; i < tamAlpha; i++) {
        vector<char> solucaoAtual = GulosoRandomizadoAdaptativo(grafo, alphas[i].alpha);
        
        // Agora: Tem mais tratamento pra soluçoes inválidas
        if (conjuntoDominanteValido(solucaoAtual, grafo)) {
            // Se a solução é válida, armazena o tamanho dela como média
            alphas[i].media = solucaoAtual.size();
        } else {
            // Penalização: atribui um valor alto (ordem do grafo) para alphas que geram soluções inválidas
            // Isso evita divisão por zero e penaliza alphas ruins
            alphas[i].media = grafo->ordem; 
            cout << "Solução inválida com alpha=" << alphas[i].alpha << ". Atribuído valor de penalização." << endl;
        }

        /* 
        
        Se usássemos media_alpha = 0.0f para soluções inválidas:
        - Q[i] = (tamGrafo - melhorSolucao) / 0.0f iria gerar divisão por zero
        
        grafo->ordem (número de vértices) como penalização:
        - valor alto o suficiente para reduzir significativamente Q[i]
        - Alphas ruins continuam tendo menor probabilidade
        */

        if (melhorSolucao.empty() || solucaoAtual.size() < melhorSolucao.size()) {
            melhorSolucao = solucaoAtual;
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
        // Agora: Média móvel ponderada usando contador
        alphaAtual.media = (alphaAtual.media * alphaAtual.contador + solAtual.size()) / (alphaAtual.contador + 1);
        alphaAtual.contador++; // Incrementa o contador para a próxima iteração

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
    for(int i = 0; i < quantidade; i++){
        soma += alphas[i].probabilidade;
        if(random <= soma){
            return i;
        }
    }
    // Caso não encontre (deve encontrar, mas por segurança)
    cout << "Erro: função pegarAlpha não encontrou um alpha. Soma=" << soma << ", random=" << random << endl;
    return 0;
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
        if(grafo->getNo(idDominador)->ehAdjacente(id)) {
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