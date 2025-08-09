#include "Gerenciador.h"
#include <fstream>

void Gerenciador::comandos1(Grafo* grafo) {
    bool running = true;
    while (running){
        cout<<"Digite uma das opcoes abaixo e pressione enter:"<<endl<<endl;
        cout<<"(a) Fecho transitivo direto de um no;"<<endl;
        cout<<"(b) Fecho transitivo indireto de um no;"<<endl;
        cout<<"(c) Caminho minimo (Djikstra);"<<endl;
        cout<<"(d) Caminho minimo (Floyd);"<<endl;
        cout<<"(e) Arvore Geradora Minima (Algoritmo de Prim);"<<endl;
        cout<<"(f) Arvore Geradora Minima (Algoritmo de Kruskal);"<<endl;
        cout<<"(g) Arvore de caminhamento em profundidade;"<<endl;
        cout<<"(h) Raio, diametro, centro e periferia do grafo;"<<endl;
        cout<<"(0) Sair;"<<endl<<endl;
        char resp;
        cin >> resp;
       switch (resp) {
            case 'a': {

                char id_no = get_id_entrada();
                vector<char> fecho_transitivo_direto = grafo->fecho_transitivo_direto(id_no);
                if(fecho_transitivo_direto.empty()) {
                    cout << "O nó " << id_no << " não possui nós no fecho transitivo direto." << endl;
                    cout << endl;
                    break;
                }
                cout << "Fecho transitivo direto do nó " << id_no << ": " << endl;
                for(int i = 0; i < fecho_transitivo_direto.size(); ++i) {
                    cout << fecho_transitivo_direto[i];
                    if(i < fecho_transitivo_direto.size() - 1)
                        cout << ",";
                }
                cout<<endl<<endl;

                if(pergunta_imprimir_arquivo("fecho_trans_dir.txt")) {
                    imprimeVetorNoArquivo("fecho_trans_dir.txt", fecho_transitivo_direto);
                    cout<<endl<<endl;
                }
                break;
            }

            case 'b':{

                char id_no = get_id_entrada();
                vector<char> fecho_transitivo_indireto = grafo->fecho_transitivo_indireto(id_no);

                if(fecho_transitivo_indireto.empty()) {
                    cout << "O nó " << id_no << " não possui nós no fecho transitivo indireto." << endl;
                    cout << endl;
                    break;
                }
            
                cout << "Fecho transitivo indireto do no " << id_no << ": " << endl;
                for(int i = 0; i < fecho_transitivo_indireto.size(); ++i) {
                    cout << fecho_transitivo_indireto[i];
                    if(i < fecho_transitivo_indireto.size() - 1)
                        cout << ",";
                }
                cout<<endl<<endl;

                if(pergunta_imprimir_arquivo("fecho_trans_indir.txt")) {
                    imprimeVetorNoArquivo("fecho_trans_indir.txt", fecho_transitivo_indireto);
                }
                break;
            }

            case 'c': {

                char id_no_1 = get_id_entrada();
                char id_no_2 = get_id_entrada();
                vector<char> caminho_minimo_dijkstra = grafo->caminho_minimo_dijkstra(id_no_1,id_no_2);
                cout<<"Caminho minimo entre " << id_no_1 << " e " << id_no_2 << ": " << endl;
                for(int i = 0; i < caminho_minimo_dijkstra.size(); ++i) {
                    cout << caminho_minimo_dijkstra[i];
                    if(i < caminho_minimo_dijkstra.size() - 1)
                        cout << ",";
                }
                cout<<endl<<endl;

                if(pergunta_imprimir_arquivo("caminho_minimo_dijkstra.txt")) {
                    imprimeVetorNoArquivo("caminho_minimo_dijkstra.txt", caminho_minimo_dijkstra);
                }


                break;
            }

            case 'd': {

                char id_no_1 = get_id_entrada();
                char id_no_2 = get_id_entrada();
                vector<char> caminho_minimo_floyd = grafo->caminho_minimo_floyd(id_no_1,id_no_2);
                cout << "Caminho minimo entre " << id_no_1 << " e " << id_no_2 << ": " << endl;
                for(int i = 0; i < caminho_minimo_floyd.size(); ++i) {
                    cout << caminho_minimo_floyd[i];
                    if(i < caminho_minimo_floyd.size() - 1)
                        cout << ",";
                }
                cout<<endl<<endl;

                if(pergunta_imprimir_arquivo("caminho_minimo_floyd.txt")) {
                    imprimeVetorNoArquivo("caminho_minimo_floyd.txt", caminho_minimo_floyd);
                }


                break;
            }
            case 'e': {

                int tam;
                cout<<"Digite o tamanho do subconjunto: ";
                cin>>tam;

                if(tam > 0 && tam <= grafo->ordem) {

                    vector<char> ids = get_conjunto_ids(grafo,tam);

                    Grafo *subgrafo = grafo->getSubgrafo(ids);
                    if(subgrafo->numComponentesConexas() > 1) {
                        cout<<"O subgrafo escolhido nao é conexo, impossivel calcular a arvore geradora minima\n\n";
                        delete subgrafo;
                        break;
                    }
                    delete subgrafo;


                    Grafo* arvore_geradora_minima_prim = grafo->arvore_geradora_minima_prim(ids);
                    cout<< "Arvore geradora minima pelo método de Prim: " << endl;
                    cout << arvore_geradora_minima_prim->toString() << endl;

                    if(pergunta_imprimir_arquivo("agm_prim.txt")) {
                        imprimeGrafoNoArquivo("agm_prim.txt", *arvore_geradora_minima_prim);
                    }

                    delete arvore_geradora_minima_prim;

                }else {
                    cout<<"Valor invalido"<<endl;
                }

                break;
            }

            case 'f': {

                int tam;
                cout<<"Digite o tamanho do subconjunto: ";
                cin>>tam;

                if(tam > 0 && tam <= grafo->ordem) {

                    vector<char> ids = get_conjunto_ids(grafo,tam);

                    Grafo *subgrafo = grafo->getSubgrafo(ids);
                    if(subgrafo->numComponentesConexas() > 1) {
                        cout<<"O subgrafo escolhido nao é conexo, impossivel calcular a arvore geradora minima\n\n";
                        delete subgrafo;
                        break;
                    }
                    delete subgrafo;

                    Grafo* arvore_geradora_minima_kruskal = grafo->arvore_geradora_minima_kruskal(ids);
                    cout << "Arvore geradora minima pelo método de Kruskal: " << endl;
                    cout << arvore_geradora_minima_kruskal->toString() << endl;

                    if(pergunta_imprimir_arquivo("agm_kruskal.txt")) {
                        imprimeGrafoNoArquivo("agm_kruskal.txt", *arvore_geradora_minima_kruskal);
                    }

                    delete arvore_geradora_minima_kruskal;

                }else {
                    cout<<"Valor invalido"<<endl;
                }

                break;
            }

            case 'g': {

                char id_no = get_id_entrada();
                Grafo* arvore_caminhamento_profundidade = grafo->arvore_caminhamento_profundidade(id_no);
                cout<< "Arvore dada pela busca em profundidade a partir do nó " << id_no << endl;
                cout << arvore_caminhamento_profundidade->toString() << endl;
                cout << "Aresta com peso -1 são arestas de retorno" << endl;

                if(pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt")) {
                    imprimeGrafoNoArquivo("arvore_caminhamento_profundidade.txt", *arvore_caminhamento_profundidade);
                }

                delete arvore_caminhamento_profundidade;
                break;
            }

            case 'h': {
                int r = grafo->raio();
                int d = grafo->diametro();
                vector<char> c = grafo->centro();
                vector<char> p = grafo->periferia();
            

                // Raio (apenas o valor)
                cout << r << endl;
                
                // Diâmetro (apenas o valor)
                cout << d << endl;
                
                // Centro (elementos separados por vírgula)
                for (int i = 0; i < c.size(); i++) {
                    cout << c[i];
                    if (i < c.size() - 1) cout << ",";
                }
                cout << endl;
                
                // Periferia (elementos separados por vírgula)
                for (int i = 0; i < p.size(); i++) {
                    cout << p[i];
                    if (i < p.size() - 1) cout << ",";
                }
                cout << endl;
            
                // Opção de salvar em arquivo
                if(pergunta_imprimir_arquivo("raio_diametro_centro_periferia.txt")) {
                    ofstream outfile("raio_diametro_centro_periferia.txt");
                    if (outfile) {
                        outfile << r << endl;
                        outfile << d << endl;
                        
                        for (int i = 0; i < c.size(); i++) {
                            outfile << c[i];
                            if (i < c.size() - 1) outfile << ",";
                        }
                        outfile << endl;
                        
                        for (int i = 0; i < p.size(); i++) {
                            outfile << p[i];
                            if (i < p.size() - 1) outfile << ",";
                        }
                        outfile << endl;

                        cout << "Resultados salvos em raio_diametro_centro_periferia.txt" << endl;
                    } else {
                        cout << "Erro ao salvar arquivo!" << endl;
                    }
                }
                break;
            }

            case '0': {
                running = !running;
                cout<<"Programa terminado!"<<endl;
                break;
            }
            default: {
                cout<<"Opção inválida"<<endl;
            }
        }
    }
}

char Gerenciador::get_id_entrada() {
    cout<<"Digite o id de um no: ";
    char id;
    cin>>id;
    cout<<endl;
    return id;
}

vector<char> Gerenciador::get_conjunto_ids(Grafo *grafo, int tam) {
    vector<char> ids = {};
    while((int)ids.size() < tam) {
        char id_no =get_id_entrada();
        bool existe = false;
        for(No* no: grafo->lista_adj){
            if(no->id == id_no){
                existe = true;
                break;
            }
        }

        if(!existe){
            cout<<"Vertice nao existe"<<endl;
        }else{
            bool repetido = find(ids.begin(), ids.end(),id_no) != ids.end();
            if(repetido) {
                    cout<<"Valor repetido"<<endl;
            }else {
                ids.push_back(id_no);
            }
        }

    }

    return ids;
}

bool Gerenciador::pergunta_imprimir_arquivo(string nome_arquivo) {

    cout<<"Imprimir em arquivo externo? ("<<nome_arquivo<<")"<<endl;
    cout<<"(1) Sim;"<<endl;
    cout<<"(2) Nao."<<endl;
    int resp;
    cin>>resp;
    cout<<endl;

    switch (resp) {
        case 1:
            return true;
        case 2:
            return false;
        default:
            cout<<"Resposta invalida"<<endl;
            return pergunta_imprimir_arquivo(nome_arquivo);
    }
}

// MÉTODO NOVO
Grafo* Gerenciador::carregarGrafoDoArquivo(const std::string& nomeArquivo) {
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo) {
        throw std::runtime_error("Erro ao abrir: " + nomeArquivo);
    }

    // Lê dados da linha 1
    bool direcionado, ponderadoVertices, ponderadoArestas;
    if (!(arquivo >> direcionado >> ponderadoArestas >> ponderadoVertices)) {
        throw std::runtime_error("Formato inválido na linha 1");
    }

    // Criar o grafo e setar propriedades
    Grafo* grafo = new Grafo(direcionado, ponderadoVertices, ponderadoArestas);

    // Lê número de vértices (linha 2)
    int numVertices;
    if (!(arquivo >> numVertices) || numVertices <= 0) {
        delete grafo;
        throw std::runtime_error("Número de vértices inválido");
    }

    // Lê vértices
    char id;
    int peso;
    for (int i = 0; i < numVertices; i++) {
        if (ponderadoVertices) {
            if (!(arquivo >> id >> peso)) {
                delete grafo;
                throw std::runtime_error("Erro ao ler vértice " + std::to_string(i+1));
            }
        } else {
            if (!(arquivo >> id)) {
                delete grafo;
                throw std::runtime_error("Erro ao ler vértice " + std::to_string(i+1));
            }
            peso = 0; // Peso padrão para vértices não ponderados
        }
        
        grafo->adicionarNo(id, peso);
    }

    // Lê arestas
    std::string linha;
    std::getline(arquivo, linha); // Consumir a quebra de linha restante
    
    while (std::getline(arquivo, linha)) {
        if (linha.empty()) continue;
        
        std::istringstream iss(linha);
        char origem, destino;
        int pesoAresta = 1; // Default para grafos não ponderados
        
        if (!(iss >> origem >> destino)) {
            delete grafo;
            throw std::runtime_error("Formato inválido de aresta: " + linha);
        }
        
        if (ponderadoArestas && !(iss >> pesoAresta)) {
            delete grafo;
            throw std::runtime_error("Peso faltando em aresta: " + linha);
        }
        
        // Adicionar aresta
        grafo->adicionarAresta(origem, destino, pesoAresta);
    }

    return grafo;
}

void Gerenciador::imprimeVetorNoArquivo(const std::string& nomeArquivo, vector<char> vetorArquivo){
    std::ofstream outfile;
    outfile.open(nomeArquivo, ios::out);
    if(!outfile) throw std::runtime_error("Erro ao criar: " + nomeArquivo);

    if(vetorArquivo.empty()) {
        outfile << "Vetor vazio" << std::endl;
        outfile.close();
        std::cout << "Vetor vazio, nada para imprimir no arquivo." << std::endl;
        return;
    }

    for(int i = 0; i < vetorArquivo.size(); ++i) {
        outfile << vetorArquivo[i];
        if(i < vetorArquivo.size() - 1)
            outfile << ",";
    }

    outfile.close();

    std::cout << "Conteúdo impresso no arquivo com sucesso" << std::endl;
}

void Gerenciador::imprimeGrafoNoArquivo(const std::string& nomeArquivo, Grafo& grafoArquivo){
    std::ofstream outfile;
    outfile.open(nomeArquivo, ios::out);
    if(!outfile) throw std::runtime_error("Erro ao criar: " + nomeArquivo);

    if(grafoArquivo.lista_adj.empty()) {
        outfile << "Grafo vazio" << std::endl;
        outfile.close();
        std::cout << "Grafo vazio, nada para imprimir no arquivo." << std::endl;
        return;
    }

    outfile << grafoArquivo.toString() << endl << endl;

    outfile.close();

    std::cout << "Conteúdo impresso no arquivo com sucesso" << std::endl;
}

void Gerenciador::comandos2(Grafo* grafo) {
    bool running = true;
    do
    {
        cout << "\nOs algoritmos a seguir resolvem o problema de 2-dominating set." << endl;
        cout << "Digite uma das opcoes abaixo e pressione enter:" << endl << endl;
        cout << "(a) Algoritmo guloso" << endl;
        cout << "(b) Algoritmo guloso adaptativo" << endl;
        cout << "(c) Algoritmo guloso adaptativo reativo" << endl;
        cout << "(d) Verificar se um conjunto é um 2-dominating set válido válido" << endl;
        cout << "(0) Sair" << endl << endl;
        char resp;
        cin >> resp;
        switch (resp) {
            case 'a': {
                cout << "Algoritmo guloso selecionado." << endl;
                TwoDominatingSet *tds = new TwoDominatingSet();
                vector<char> resultado = tds->Guloso(grafo);
                delete tds;
                
                if (resultado.empty()) {
                    cout << "Nenhum nó foi selecionado pelo algoritmo guloso." << endl;
                    cout << endl;
                    break;
                }

                cout << "Resultado do algoritmo guloso: ";
                for (char id : resultado) {
                    cout << id << " ";
                }
                cout << endl;
                
                if (pergunta_imprimir_arquivo("resultado_guloso.txt")) {
                    imprimeVetorNoArquivo("resultado_guloso.txt", resultado);
                }
                break;
            }
            case 'b': {
                cout << "Algoritmo guloso adaptativo selecionado." << endl;

                cout << "Digite o valor desejado de alfa:";
                float alfa = 0.5;
                cin >> alfa;

                TwoDominatingSet *tds = new TwoDominatingSet();
                vector<char> resultado = tds->GulosoRandomizadoAdaptativo(grafo, alfa);
                delete tds;
                
                if (resultado.empty()) {
                    cout << "Nenhum nó foi selecionado pelo algoritmo guloso adaptativo." << endl;
                    cout << endl;
                    break;
                }

                cout << "Resultado do algoritmo guloso adaptativo: ";
                for (char id : resultado) {
                    cout << id << " ";
                }
                cout << endl;
                
                if (pergunta_imprimir_arquivo("resultado_guloso_adaptativo.txt")) {
                    imprimeVetorNoArquivo("resultado_guloso_adaptativo.txt", resultado);
                }
                break;
            }
            case 'c': {
                cout << "Algoritmo guloso randomizado adaptativo reativo selecionado." << endl;

                int tam, nIter, bloco;
                cout << "Diga com quantos alfas deseja testar: ";
                cin >> tam;
                float* alfas = new float[tam];
                cout << "Digite os seus alfas: " << endl;
                for(int i = 0; i < tam; i++){
                    cout << "Alfa " << (i + 1) << ": ";
                    cin >> alfas[i];
                }
                cout << "Com quantas iterações? ";
                cin >> nIter;
                cout << "Qual o tamanho do bloco? ";
                cin >> bloco;
                

                TwoDominatingSet *tds = new TwoDominatingSet();
                vector<char> resultado = tds->GulosoRandomizadoAdaptativoReativo(grafo, alfas, tam, nIter, bloco);
                delete tds;
                
                if (resultado.empty()) {
                    cout << "Nenhum nó foi selecionado pelo algoritmo guloso adaptativo reativo." << endl;
                    cout << endl;
                    break;
                }

                cout << "Resultado do algoritmo guloso adaptativo reativo: ";
                for (char id : resultado) {
                    cout << id << " ";
                }
                cout << endl;
                
                if (pergunta_imprimir_arquivo("resultado_guloso_adaptativo_reativo.txt")) {
                    imprimeVetorNoArquivo("resultado_guloso_adaptativo_reativo.txt", resultado);
                }
                break;
            }
            case 'd':{
                TwoDominatingSet *tds = new TwoDominatingSet();

                cout << "Digite o tamanho do conjunto dominante: ";
                int tamanho;
                cin >> tamanho;
                if (tamanho <= 0 || tamanho > grafo->ordem) {
                    cout << "Tamanho inválido. Deve ser entre 1 e " << grafo->ordem << "." << endl;
                    delete tds;
                    break;
                }

                cout << "Digite os IDs dos nós do conjunto dominante (separados por espaço): ";
                vector<char> idsDominantes;
                for (int i = 0; i < tamanho; ++i) {
                    char id;
                    cin >> id;
                    if (find(idsDominantes.begin(), idsDominantes.end(), id) != idsDominantes.end()) {
                        cout << "ID " << id << " já foi inserido. Por favor, insira um ID único." << endl;
                        --i; // Decrementa para repetir a entrada
                    } else {
                        idsDominantes.push_back(id);
                    }
                }
                cin.ignore(); // Limpa o buffer de entrada

                
                if (tds->conjuntoDominanteValido(idsDominantes, grafo)) {
                    cout << "O conjunto dominante é válido." << endl;
                } else {
                    cout << "O conjunto dominante não é válido." << endl;
                }

                delete tds;
                break;
            }
            case '0': {
                running = false;
                break;
            }
            default: {
                cout << "Opção inválida" << endl;
            }
        }
    }while(running);
}