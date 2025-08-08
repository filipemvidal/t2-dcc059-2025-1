#ifndef TWODOMINATINGSET_H
#define TWODOMINATINGSET_H

#pragma once

#include "Grafo.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm> // Para max_element, find, etc.
#include <iostream>  // Para cout

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

        // Algoritmo 1: Guloso determinístico
        vector<char> Guloso(Grafo* grafo);
        
        // Algoritmo 2: Guloso Randomizado Adaptativo
        vector<char> GulosoAdaptativo(Grafo* grafo, float alpha);
        
        // Algoritmo 3: Guloso Randomizado Adaptativo Reativo
        vector<char> GulosoAdaptativoReativo(Grafo* grafo, float alpha[], int tamAlpha, int numIter, int bloco);

        // Função de validação
        bool conjuntoDominanteValido(const vector<char>& conjuntoDominante, Grafo* grafo);
        
    private:
        // Funções auxiliares
        bool estaDominado(char id, const vector<char>& conjuntoDominante, Grafo* grafo);
        void atualizaProbabilidades(meuAlpha alphas[], int tamanhoMelhorSolucao, int quantidade, int tamanhoGrafo);
        int pegarAlpha(meuAlpha alphas[], int quantidade);
        
        // Função de verificação de adjacência
        bool ehAdjacente(No* no, char id);
};

#endif