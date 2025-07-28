#include <iostream>
#include "Gerenciador.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <arquivo_entrada>" << std::endl;
        return 1;
    }

    Grafo* grafo = nullptr;
    try {
        // Agora o carregamento é feito pelo Gerenciador
        grafo = Gerenciador::carregarGrafoDoArquivo(argv[1]);
        cout << "Grafo carregado com sucesso!" << std::endl;
        cout << "Ordem: " << grafo->ordem << std::endl;
    } catch (const std::exception& e) {
        cerr << "Erro: " << e.what() << std::endl;
        if (grafo) delete grafo;
        return 1;
    }

    bool running = true;
    do
    {
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
    }while(running);
    delete grafo;
    return 0;
    // g++ *.cpp -o execGrupoX && ./execGrupoX <arquivo_entrada>
}