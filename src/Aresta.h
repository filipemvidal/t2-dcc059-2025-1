#ifndef ARESTA_H
#define ARESTA_H

using namespace std;

class No; 

class Aresta {
public:
    Aresta(No* alvo, int peso = 0);
    ~Aresta();

    // ID do nó pro qual ela aponta
    No* no_alvo;
    int peso;

    No* getNoAlvo();
    void setNoAlvo(No* novoNo);
    int getPeso();
    void setPeso(int peso);
};



#endif //ARESTA_H
