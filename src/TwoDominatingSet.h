#ifndef TWODOMINATINGSET_H
#define TWODOMINATINGSET_H

#pragma once

#include "Grafo.h"
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

struct meuAlpha {
    float alpha;
    float probabilidade;
    float media;
    int contador = 1;
};

class TwoDominatingSet
{
    public:
        TwoDominatingSet();
        ~TwoDominatingSet();

        vector<char> Guloso(Grafo* grafo);
        vector<char> GulosoAdaptativo(Grafo* grafo);
        vector<char> GulosoAdaptativoReativo(Grafo* grafo, float alpha[], int tamAlpha, int numIter, int bloco);

        bool isValidDominatingSet(const vector<char>& dominatingSet, Grafo* grafo);
        
        private:
        bool isDominated(char id, const vector<char>& dominatingSet, Grafo* grafo);
        void atualizaProbabilidades(meuAlpha alphas[], int sizeMelhorSolucao, int tam, int tamGrafo);
        int pegarAlpha(meuAlpha alphas[], int tam);

};

#endif