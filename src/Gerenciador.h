#ifndef GERENCIADOR_H
#define GERENCIADOR_H

#include <iostream>
#include "Grafo.h"
#include <algorithm>
#include <sstream>
#include "TwoDominatingSet.h"
#include <chrono>
#include <numeric>

using namespace std;
class Gerenciador {
    public:
        static void comandos1(Grafo* grafo);
        static Grafo* carregarGrafoDoArquivo(const std::string& nomeArquivo);
        static void imprimeVetorNoArquivo(const std::string& nomeArquivo, vector<char> vetorArquivo);
        static void imprimeGrafoNoArquivo(const std::string& nomeArquivo, Grafo& grafo);
        static char get_id_entrada();
        static std::vector<char> get_conjunto_ids(Grafo *grafo, int tam);
        static bool pergunta_imprimir_arquivo(std::string nome_arquivo);
        static void comandos2(Grafo* grafo);
        static void comandos2Automatico(Grafo* grafo, const std::string& outputFile);
    };

#endif //GERENCIADOR_H