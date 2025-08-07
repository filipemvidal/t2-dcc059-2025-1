#include "TwoDominatingSet.h"

TwoDominatingSet::TwoDominatingSet()
{
    srand(time(0));
}

TwoDominatingSet::~TwoDominatingSet()
{

}

vector<char> TwoDominatingSet::Guloso(Grafo* grafo) {

    vector<char> dominatingSet; // Conjunto solução
    vector<char> naoDominados;  // Nós ainda não 2-dominados

    // Inicializa todos os nós como não dominados
    for(No* no : grafo->lista_adj){
        naoDominados.push_back(no->getID());
    }

    while(!naoDominados.empty()){
        char melhorNo = '';
        int melhorContribuicao = -1;

        for(No* candidato : grafo->lista_adj){
            char idCandidato = candidato->getID();

            // Funcao pra ignorar se estiver no conjunto dominante
            if(find(dominatingSet.begin(), dominatingSet.end(), id) != dominatingSet.end())
                continue;

            int contribuicao = 0;

            // Função para avaliar quantos ele ajuda a dominar
            for(char alvo : naoDominados){
                vector<char> conjuntoProvisorio = dominatingSet;
                conjuntoProvisorio.push_back(id);
                if(isDominated(alvo, conjuntoProvisorio, grafo)){
                    contribuicao++;
                }
            }

            if(contribuicao > melhorContribuicao){
                melhorContribuicao = contribuicao;
                melhorNo = idCandidato;
            }
        }

        // Adiciona o vértice escolhido ao conjunto dominante
        dominatingSet.push_back(melhorNo);

        // Atualiza a lista de não dominados
        vector<char> atualizados;
        for(char alvo:naoDominados){
            if(!isDominated(alvo, dominatingSet, grafo)){
                atualizados.push_back(alvo);
            }
        }
        naoDominados = atualizados;
    }

    // Vai retornar o vetor com vértices que compõem o conjunto solução
    return dominatingSet;
}

vector<char> TwoDominatingSet::GulosoAdaptativo(Grafo* grafo) {
    return vector<char>(); // Implementar lógica do algoritmo guloso adaptativo
}

int TwoDominatingSet::pegarAlpha(meuAlpha alphas[], int tam){
    float random = (float)rand() / RAND_MAX;
    float soma = 0;
    for(int i = 0; i < tam; i++){
        soma += alphas[i].probabilidade;
        if(random <= soma){
            return i;
        }
    }
    cout << "Função de pegar alpha não calculou corretamente" << endl;
    cout << "Soma de probabilidades = " << soma << " e valor randomizado obtido = " << random << endl;
    exit(1);
    return -1;
}

void TwoDominatingSet::atualizaProbabilidades(meuAlpha alphas[], int sizeMelhorSolucao, int tam, int tamGrafo){
    vector<float> Q (tam, 0);
    float sumQ = 0;
    for(int i = 0; i < tam; i++){
        int qualidadeSol = tamGrafo - sizeMelhorSolucao; 
        // Quanto melhor a solução, menos nós tem do grafo. Ou seja, precisamos fazer essa conta pra saber a qualidade da solução
        Q[i] = qualidadeSol/alphas[i].media;
        sumQ += Q[i];
    }
    for(int i = 0; i < tam; i++){
        alphas[i].probabilidade = Q[i]/sumQ;
    }
}

vector<char> TwoDominatingSet::GulosoAdaptativoReativo(Grafo* grafo, float vetAlphas[], int tamAlpha, int numIter, int bloco) {
    vector<char> melhorSolucao;
    meuAlpha* alphas = new meuAlpha[tamAlpha];
    
    // Primeira iteração com cada um
    for (int i = 0; i < tamAlpha; i++) {
        alphas[i].alpha = vetAlphas[i];
        alphas[i].probabilidade = 1/tamAlpha;

        vector<char> solucaoAtual = GulosoAdaptativo(grafo, alphas[i].alpha);
        bool isValid = isValidDominatingSet(solucaoAtual, grafo);
        if (isValid) {
            alphas[i].media = solucaoAtual.size();
        } else {
            alphas[i].media = 0.0f; 
            cout << "Solução inicial inválida. Reveja o GulosoAdaptativo" << endl;
        }
    }

    for(int i = 0; i < numIter; i++){
        if(i % bloco == 0){
            atualizaProbabilidades(alphas, melhorSolucao.size(), tamAlpha, grafo->ordem);
        }
        int indexAlpha = pegarAlpha(alphas, tamAlpha);
        meuAlpha* alphaAtual = &alphas[indexAlpha];
        vector<char> solAtual = GulosoAdaptativo(grafo, alphaAtual->alpha);

        // Atualiza média
        alphaAtual->media = (alphaAtual->media + solAtual.size())/(++alphaAtual->contador);

        if(melhorSolucao.empty() || solAtual.size() < melhorSolucao.size()){
            melhorSolucao = solAtual;
        }

    }

    return melhorSolucao;
}

bool TwoDominatingSet::isDominated(char id, const vector<char>& dominatingSet, Grafo* grafo) {
    int cont = 0;
    for (char dominatedId : dominatingSet) {    
        if(grafo->getNo(dominatedId)->isAdjacent(id)) {
            cont++;
        }
    }
    return cont >= 2; // Verifica se o nó está dominado por pelo menos dois nós do conjunto dominante
}

bool TwoDominatingSet::isValidDominatingSet(const vector<char>& dominatingSet, Grafo* grafo) {
    for (No* no : grafo->lista_adj) {
        if(find(dominatingSet.begin(), dominatingSet.end(), no->getID()) != dominatingSet.end()) {
            continue; // Se o nó já está no conjunto dominante, não precisa verificar
        }
        
        if (!isDominated(no->getID(), dominatingSet, grafo)) {
            return false; // Se algum nó não está dominado por pelo menos dois nós do conjunto, o conjunto não é válido
        }
    }
    return true; // Todos os nós estão dominados por pelo menos dois nós do conjunto
}