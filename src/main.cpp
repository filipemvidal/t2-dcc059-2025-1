#include <iostream>
#include "Gerenciador.h"
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <arquivo_entrada> [auto] [arquivo_saida]" << endl;
        return 1;
    }

    Grafo* grafo = nullptr;
    try {
        grafo = Gerenciador::carregarGrafoDoArquivo(argv[1]);
        cout << "Grafo carregado com sucesso!" << endl;
        cout << "Ordem: " << grafo->ordem << endl;
    } catch (const std::exception& e) {
        cerr << "Erro: " << e.what() << endl;
        if (grafo) delete grafo;
        return 1;
    }

    // Modo automático
    if (argc >= 3 && string(argv[2]) == "auto") {
        string outputFile = (argc >= 4) ? argv[3] : "resultados.csv";
        Gerenciador::comandos2Automatico(grafo, outputFile);
        delete grafo;
        return 0;
    }

    // Modo interativo
    bool running = true;
    do {
        cout << "\nQual menu você deseja acessar?" << endl;
        cout << "(1) Menu do trabalho 1" << endl;
        cout << "(2) Menu do trabalho 2" << endl;
        cout << "(0) Sair" << endl;
        char resp;
        cin >> resp;
        switch (resp) {
            case '0':
                running = false;
                break;
            case '1':
                Gerenciador::comandos1(grafo);
                break;
            case '2':
                Gerenciador::comandos2(grafo);
                break;
            default:
                cout << "Opção inválida." << endl;
        }
    } while (running);

    delete grafo;
    return 0;
}
    // g++ *.cpp -o execGrupoX && ./execGrupoX <arquivo_entrada>
