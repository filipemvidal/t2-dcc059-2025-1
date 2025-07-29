#include "TwoDominatingSet.h"

TwoDominatingSet::TwoDominatingSet()
{

}

TwoDominatingSet::~TwoDominatingSet()
{

}

vector<char> TwoDominatingSet::Guloso(Grafo* grafo) {
    return vector<char>(); // Implementar lógica do algoritmo guloso
}

vector<char> TwoDominatingSet::GulosoAdaptativo(Grafo* grafo) {
    return vector<char>(); // Implementar lógica do algoritmo guloso adaptativo
}

vector<char> TwoDominatingSet::GulosoAdaptativoReativo(Grafo* grafo) {
    return vector<char>(); // Implementar lógica do algoritmo guloso adaptativo reativo
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