#include "No.h"

// ===== Construtor e destrutor =====
No::No(char id, int peso){
    setID(id);
    setPeso(peso);
}

No::~No(){
    for(int i=0;i<arestas.size(); i++){
        delete arestas[i];
    }
}

// ===== Funções e operações =====
void No::adicionarAresta(Aresta* novaAresta){
    arestas.push_back(novaAresta);
    // Adiciona a nova aresta ao final do vetor de arestas do nó
}

void No::adicionarArestaInvertida(Aresta* novaAresta){
    arestas_invertidas.push_back(novaAresta);
}

// ===== Gets e sets =====
int No::getPeso(){
    return this->peso;
}

void No::setPeso(int peso){
    this->peso = peso;
}

char No::getID(){
    return this->id;
}

void No::setID(char id){
    this->id = id;
}

bool No::isAdjacent(char idNo) {
    for (const auto& aresta : arestas) {
        if (aresta->getIDalvo() == idNo) {
            return true;
        }
    }
    return false;
}