#include "No.h"
#include "Aresta.h"

// ===== Construtor e destrutor =====
No::No(char id, int peso){
    setID(id);
    setPeso(peso);
    domina(this); // O nó domina a si mesmo
}

No::~No(){
    for(int i=0;i<arestas.size(); i++){
        delete arestas[i];
    }
}

// ===== Funções e operações =====
void No::adicionarAresta(Aresta* novaAresta){
    arestas.push_back(novaAresta);

    // Adiciona como dominado o nó que a aresta aponta
    domina(novaAresta->getNoAlvo());
    for(const auto& invertida : arestas_invertidas){
        invertida->getNoAlvo()->domina(this);
    }
    // Em um grafo direcionado, essa linha atualiza a lista de dominados dos nós que atingem ele com distância 2.

    // TODO: Precisamos entender como fazer isso em um grafo não direcionado
}

void No::adicionarAresta(Aresta* novaAresta, bool ehDirecionado){
    // OBS: Precisamos fazer antes de add a aresta para evitar que o nó se auto domine novamente
    // Se não for direcionado, dizemos que todos os nós conectados dominam o novo nó por distância 2
    if(!ehDirecionado){
        for(const auto& aresta : arestas){
            aresta->getNoAlvo()->domina(novaAresta->getNoAlvo());
        }
    }
    
    arestas.push_back(novaAresta); // Adiciona a aresta ao nó

    // Adiciona como dominado o nó que a aresta aponta, com distância 1
    domina(novaAresta->getNoAlvo());
    
    if(ehDirecionado) return; // Se for direcionado, não vai para as arestas invertidas
    
    for(const auto& invertida : arestas_invertidas){
        invertida->getNoAlvo()->domina(novaAresta->getNoAlvo());
    }
    // Em um grafo direcionado, esse for atualiza a lista de dominados dos nós que atingem ele com distância 2.
}

void No::adicionarArestaInvertida(Aresta* novaAresta){
    arestas_invertidas.push_back(novaAresta);
}

void No::domina(No* noDominado) {
    nosDominados.insert(noDominado);
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