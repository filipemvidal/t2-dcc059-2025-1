#ifndef NO_H
#define NO_H

#include <string>
#include <sstream>
#include <vector>
#include "Aresta.h"

using namespace std;

class No {
public:

    // construtor e destrutor
    No(char id, int peso);
    ~No();

    // variaveis
    char id;
    int peso;
    vector<Aresta*> arestas;
    vector<Aresta*> arestas_invertidas; 

    // funções e operações
    void adicionarAresta(Aresta* novaAresta);
    void adicionarArestaInvertida(Aresta* novaAresta);
    bool isAdjacent(char idNo);

    // gets e sets
    int getPeso();
    void setPeso(int peso);
    char getID();
    void setID(char id);
    int getGrau();

};



#endif //NO_H
