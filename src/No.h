#ifndef NO_H
#define NO_H

#include <string>
#include <sstream>
#include <vector>
#include <set>


using namespace std;

class Aresta; // Forward declaration

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
    void adicionarAresta(Aresta* novaAresta, bool ehDirecionado);
    void adicionarArestaInvertida(Aresta* novaAresta);

    // gets e sets
    int getPeso();
    void setPeso(int peso);
    char getID();
    void setID(char id);

    set<No*> nosDominados;
    void domina(No* noDominado);
};



#endif //NO_H
