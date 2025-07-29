#ifndef TWODOMINATINGSET_H
#define TWODOMINATINGSET_H

#pragma once

#include "Grafo.h"
#include <vector>

using namespace std;

class TwoDominatingSet
{
public:
    TwoDominatingSet();
    ~TwoDominatingSet();

    vector<char> Guloso(Grafo* grafo);
    vector<char> GulosoAdaptativo(Grafo* grafo);
    vector<char> GulosoAdaptativoReativo(Grafo* grafo);

    bool isValidDominatingSet(const vector<char>& dominatingSet, Grafo* grafo);
    
    private:
    bool isDominated(char id, const vector<char>& dominatingSet, Grafo* grafo);

};

#endif