#include "Aresta.h"
#include "No.h"

Aresta::Aresta(No* alvo, int peso){
    setNoAlvo(alvo);
    setPeso(peso);
}

Aresta::~Aresta(){

}

No* Aresta::getNoAlvo(){
    return this->no_alvo;
}

void Aresta::setNoAlvo(No* no){
    this->no_alvo = no;
}

int Aresta::getPeso(){
    return this->peso;
}

void Aresta::setPeso(int peso){
    this->peso = peso;
}

char Aresta::getIDalvo() {
    return no_alvo->getID();
}