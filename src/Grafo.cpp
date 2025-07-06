#include "Grafo.h"


Grafo::Grafo(){
    
}

Grafo::Grafo(bool direcionado, bool ponderado_vertice, bool ponderado_aresta) {
    this->in_direcionado = direcionado;
    this->in_ponderado_vertice = ponderado_vertice;
    this->in_ponderado_aresta = ponderado_aresta;
    this->ordem = 0; // tem 0 vertices inicialmente
}

Grafo::~Grafo() {
    for(No* no : lista_adj) {
        delete no; // Deleta cada nó
    }
}

void Grafo::adicionarNo(char idNovoNo, int pesoNovoNo){
    cout << "Adicionando no: " << idNovoNo << endl;
    No* novoNo = new No(idNovoNo, in_ponderado_vertice? pesoNovoNo : 0);
    lista_adj.push_back(novoNo);
    ordem++;
}

void Grafo::adicionarAresta(char origemID, char destinoID, int peso){
    cout << "Chamei essa função" << endl;
    No* no1 = nullptr;
    No* no2 = nullptr;

    for(int i=0; i < lista_adj.size(); i++){
        if(lista_adj[i]->getID() == origemID)
            no1 = lista_adj[i];
        if(lista_adj[i]->getID() == destinoID)
            no2 = lista_adj[i];
    // verifica os IDs de cada nó da lista e atribui corretamente se existirem
    }

    if(!no1 || !no2){
        cout<<"Erro: vertice nao encontrado"<<endl;
        return;
        // erro se um dos nós não estiver no grafo
    }

    int peso_aresta = in_ponderado_aresta? peso : 1;
    Aresta* novaAresta = new Aresta(destinoID, peso_aresta);
    // cria a aresta apontando para o destino
    no1->adicionarAresta(novaAresta);
    // adiciona aresta ao nó de origem

    Aresta* arestaInversa = new Aresta(origemID, peso_aresta);
    if(!in_direcionado){
        // adiciona aresta inversa se o grafo não for direcionado
        no2->adicionarAresta(arestaInversa);
    } else {
        // Adiciona aresta invertida nas invertidas para Fecho Transitivo Indireto
        cout << "Aresta invertida adicionada de " << origemID << " para " << destinoID << endl;
        no2->adicionarArestaInvertida(arestaInversa);
    }
}

No* Grafo::getNo(char id) {
    for (No* no : lista_adj) {
        if (no->getID() == id) {
            return no;
        }
    }
    cout << "No com id " << id << " nao encontrado." << endl;
    exit(1);
}

vector<char> Grafo::fecho_transitivo_direto(char id_no) {
    map<char, bool> visitados;
    vector<char> resultado;
    visitados[id_no] = true; // Marca o nó inicial como visitado
    fecho_transitivo_direto_aux(id_no, visitados, resultado);
    return resultado;
}


// Visita recursivamente os nós, como a busca em profundidade, e retorna os nós visitados
void Grafo::fecho_transitivo_direto_aux(char id_no, map<char, bool> &visitados, vector<char> &resultado) {
    No* no = getNo(id_no);
    for(const auto& aresta : no->arestas) {
        char id_alvo = aresta->id_no_alvo;
        if(!visitados[id_alvo]){
            visitados[id_alvo] = true;
            resultado.push_back(id_alvo);
            fecho_transitivo_direto_aux(id_alvo, visitados, resultado);
        }
    }
}

vector<char> Grafo::fecho_transitivo_indireto(char id_no) {
    map<char, bool> visitados;
    vector<char> resultado;
    visitados[id_no] = true; 
    fecho_transitivo_indireto_aux(id_no, visitados, resultado);
    return resultado;
}

// Mesma ideia do fecho direto, mas visita as arestas invertidas
void Grafo::fecho_transitivo_indireto_aux(char id_no, map<char, bool> &visitados, vector<char> &resultado) {
    No* no = getNo(id_no);
    for(const auto& aresta : no->arestas_invertidas) {
        char id_alvo = aresta->id_no_alvo;
        if(!visitados[id_alvo]){
            visitados[id_alvo] = true;
            resultado.push_back(id_alvo);
            fecho_transitivo_indireto_aux(id_alvo, visitados, resultado);
        }
    }
}

vector<char> Grafo::caminho_minimo_dijkstra(const char id_no_a, const char id_no_b) {
    vector<char> caminho;
    priority_queue<pair<int, char>, vector<pair<int, char>>, greater<pair<int, char>>> pq;
    map<char, int> distancias;
    map<char, char> predecessores;

    distancias[id_no_a] = 0;
    pq.push({0, id_no_a}); 

    while (!pq.empty()) { // Enquanto houver nós na fila de prioridade
        // Pega o nó com a menor distância
        char atual = pq.top().second; 
        pq.pop();

        // Se é o nó de destino, termina a busca
        if (atual == id_no_b) {
            break;
        }

        No* noAtual = getNo(atual);
        for (const auto& aresta : noAtual->arestas) { //Olhamos cada aresta do nó atual
            char id_alvo = aresta->id_no_alvo;
            int peso = aresta->getPeso();
            int nova_distancia = distancias[atual] + peso;

            // Calculamos a distância
            // Se a distância não existe no map, é tratada como infinita
            // Se a nova distância for menor que a distância já registrada, atualiza
            if (distancias.find(id_alvo) == distancias.end() || nova_distancia < distancias[id_alvo]) {
                distancias[id_alvo] = nova_distancia;
                predecessores[id_alvo] = atual;
                pq.push({nova_distancia, id_alvo});
            }
        }
    }

    // Reconstrução através dos predecessores
    char atual = id_no_b;
    while (atual != id_no_a) {
        if (predecessores.find(atual) == predecessores.end()) {
            return {}; // Se não houver caminho, retorna vazio
        }
        caminho.push_back(atual);
        atual = predecessores[atual];
    }
    caminho.push_back(id_no_a);
    reverse(caminho.begin(), caminho.end()); // Inverte o caminho para a ordem correta
    return caminho;
}

vector<char> Grafo::caminho_minimo_floyd(char id_no, char id_no_b) {

    // Verifica se os nós existem no grafo
    No* no1 = getNo(id_no);
    No* no2 = getNo(id_no_b);

    vector<char> caminhoIDs;
    vector<int> caminhoIndices;
    
    map<char, int> idIndice;
    map<int, char> indiceID;
    
    int n = lista_adj.size(); // Define a ordem N das matrizes
    
    for(int i=0; i<n; i++){
        char id = lista_adj[i]->getID(); // Obtém o ID do nó
        idIndice[id] = i; // Mapeia o ID do nó para o índice
        indiceID[i] = id; // Mapeia o índice para o ID do nó
    }

    // Inicialização das matrizes de distância e predecessores
    vector<vector<int>> distancia(n, vector<int>(n, INT_MAX));
    vector<vector<int>> predecessor(n, vector<int>(n, -1));

    for(int i=0;i<n;i++){
        distancia[i][i] = 0; // Diagonal principal
        for(Aresta* aresta : lista_adj[i]->arestas){
            int j = idIndice[aresta->getIDalvo()]; // Obtém o índice do nó de destino
            distancia[i][j] = aresta->getPeso(); // Define a distância da aresta
            predecessor[i][j] = i; // Define o predecessor
        }
    }

    // Algoritmo de verificação de menor caminho
    for(int i=0;i<n;i++) {
        for(int j=0;j<n;j++) {
            for(int k=0;k<n;k++) {
                if(distancia[j][i] != INT_MAX && distancia[i][k] != INT_MAX &&
                   distancia[j][i] + distancia[i][k] < distancia[j][k]){
                    distancia[j][k] = distancia[j][i] + distancia[i][k];
                    predecessor[j][k] = predecessor[i][k];
                }
            }
        }
    }

    for (int i = 0; i < n; i++) {
        if (distancia[i][i] < 0) {
            cout << "Ciclo negativo detectado envolvendo o nó " << indiceID[i] << endl;
            return {};
        }
    }

    // Reconstrução do caminho
    int origem = idIndice[id_no];
    int destino = idIndice[id_no_b];

    if(distancia[origem][destino] == INT_MAX){
        cout<<"Sem caminho de " << id_no << " para " << id_no_b << endl;
        return {};
    }

    for(int i = destino; i != origem; i = predecessor[origem][i]){
        if(i == -1) return {}; // Se não houver predecessor, retorna vazio
        caminhoIndices.push_back(i); // Adiciona o índice do nó ao caminho
    }
    caminhoIndices.push_back(origem);
    reverse(caminhoIndices.begin(), caminhoIndices.end());

    for(int idx : caminhoIndices){
        caminhoIDs.push_back(indiceID[idx]); // Converte os índices de volta para IDs
    }

    return caminhoIDs;
}

Grafo* Grafo::arvore_geradora_minima_prim(vector<char> ids_nos) {
    struct prox{
        char id_no_proximo;
        int custo;
    };

    map<char, prox> map_prox; // mapa para armazenar o próximo nó e seu custo
    for(char id_no : ids_nos) {
        map_prox[id_no] = {ids_nos[0], INT_MAX}; // inicializa o mapa com custo infinito
    }

    Grafo *resultado = new Grafo(); // grafo resultado
    resultado->in_direcionado = this->in_direcionado;
    resultado->in_ponderado_aresta = this->in_ponderado_aresta;
    resultado->in_ponderado_vertice = this->in_ponderado_vertice;
    resultado->ordem = ids_nos.size();

    // cria o nó inicial e adiciona ao grafo resultado
    No *no_inicial = new No(ids_nos[0], getNo(ids_nos[0])->getPeso());
    resultado->lista_adj.push_back(no_inicial);
    map_prox[ids_nos[0]] = {'\0', INT_MAX}; // marca no incial como resolvido

    
    No *no_atual = getNo(ids_nos[0]);
    int cont = 1; // contador de nós adicionados
    while(cont < ids_nos.size()) {
        for(Aresta *aresta : no_atual->arestas) {
            char id_no_alvo = aresta->getIDalvo();
            if(find(ids_nos.begin(), ids_nos.end(), id_no_alvo) != ids_nos.end()) {
                if(map_prox[id_no_alvo].custo > aresta->getPeso() and map_prox[id_no_alvo].id_no_proximo != '\0') {
                    // Atualiza o mapa de próximos se o custo for menor
                    map_prox[id_no_alvo] = {no_atual->getID(), aresta->getPeso()};
                }
            }
        }

        auto it = min_element(map_prox.begin(), map_prox.end(),
            [](const pair<char, prox> &a, const pair<char, prox> &b) {
                return a.second.custo < b.second.custo;
            });
        
        // Adiciona o nó com aresta de menor custo ao grafo resultado
        No *novo_no = new No(it->first, getNo(it->first)->getPeso());
        Aresta *nova_aresta = new Aresta(it->second.id_no_proximo, it->second.custo);
        novo_no->adicionarAresta(nova_aresta);
        resultado->lista_adj.push_back(novo_no);
        if(!resultado->in_direcionado) {
            // se o grafo nao for direcionado, adiciona a aresta inversa
            No *no_alvo = resultado->getNo(it->second.id_no_proximo);
            Aresta *nova_aresta_inversa = new Aresta(it->first, it->second.custo);
            no_alvo->adicionarAresta(nova_aresta_inversa);
        }
        
        no_atual = getNo(it->first); // atualiza o nó atual
        
        map_prox[it->first] = {'\0', INT_MAX}; // marca o nó atual como resolvido
        
        cont++;
    }
    
    return resultado;
}

Grafo *Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos)
{

    // "pacote" que engloba o no de inicio de uma aresta e a aresta em si
    struct ArestaInicioFim
    {
        char id_inicio;
        Aresta *aresta;
    };

    vector<ArestaInicioFim *> arestas;
    Grafo *resultado = new Grafo();
    resultado->in_direcionado = this->in_direcionado;
    resultado->in_ponderado_aresta = this->in_ponderado_aresta;
    resultado->in_ponderado_vertice = this->in_ponderado_vertice;
    resultado->ordem = ids_nos.size();
    for (char ind : ids_nos)
    {
        auto it = find_if(lista_adj.begin(), lista_adj.end(), // procura pelo no na lista de adjacencia
                          [ind](const No *no)
                          {
                              return no->id == ind;
                          });

        if (it != lista_adj.end()) // verifica se o no foi ecnontrado
        {
            No *no = *it;

            // cria as |v| subarvores de nós isolados
            No *no_isolado = new No(no->id, no->peso);
            resultado->lista_adj.push_back(no_isolado);

            for (Aresta *aresta : no->arestas)
            {
                // verifica se a aresta aponta para um no que esta no ids_nos
                auto it_alvo = find_if(ids_nos.begin(), ids_nos.end(),
                                       [aresta](char id_alvo)
                                       {
                                           return id_alvo == aresta->id_no_alvo;
                                       });
                if (it_alvo == ids_nos.end()) // se o no alvo nao esta no ids_nos, pula para a proxima aresta
                    continue;

                if (in_direcionado ||                       // se o grafo for direcionado, adiciona todas as arestas
                    find_if(arestas.begin(), arestas.end(), // caso contrario, verifica se a aresta ja foi adicionada
                            [aresta, no](const ArestaInicioFim *a)
                            {
                                return (a->id_inicio == aresta->id_no_alvo && a->aresta->id_no_alvo == no->id);
                            }) == arestas.end())
                {
                    ArestaInicioFim *aresta_inicio_fim = new ArestaInicioFim();
                    aresta_inicio_fim->id_inicio = no->id;
                    aresta_inicio_fim->aresta = aresta;
                    arestas.push_back(aresta_inicio_fim);
                }
            }
        }
    }

    // Ordena as arestas pelo peso
    sort(arestas.begin(), arestas.end(),
         [](const ArestaInicioFim *a, const ArestaInicioFim *b)
         {
             return a->aresta->peso < b->aresta->peso;
         });

    int cont = 0; // contador de arestas adicionadas
    while (cont < resultado->ordem - 1 && !arestas.empty())
    {
        ArestaInicioFim *aresta_inicio_fim = arestas.front();
        arestas.erase(arestas.begin()); // remove a aresta do vetor

        vector<char> fecho_direto = resultado->fecho_transitivo_direto(aresta_inicio_fim->id_inicio);

        if (find(fecho_direto.begin(), fecho_direto.end(), aresta_inicio_fim->aresta->id_no_alvo) == fecho_direto.end())
        {
            // se o no alvo da aresta não é alcançado pelo no de inicio, adiciona a aresta ao grafo resultado
            No *no_inicio = resultado->getNo(aresta_inicio_fim->id_inicio);
            Aresta *nova_aresta = new Aresta(aresta_inicio_fim->aresta->id_no_alvo, aresta_inicio_fim->aresta->peso);
            no_inicio->adicionarAresta(nova_aresta);

            if (!resultado->in_direcionado)
            {
                // se o grafo nao for direcionado, adiciona a aresta inversa
                No *no_alvo = resultado->getNo(aresta_inicio_fim->aresta->id_no_alvo);
                Aresta *nova_aresta_inversa = new Aresta(aresta_inicio_fim->id_inicio, aresta_inicio_fim->aresta->peso);
                no_alvo->adicionarAresta(nova_aresta_inversa);
            }

            cont++;
        }
    }
    return resultado;
}

Grafo * Grafo::arvore_caminhamento_profundidade(char id_no) {
    map<char, bool> visitado;
    
    Grafo *resultado = new Grafo(false, false, true);

    for(No *no: lista_adj) {
        visitado[no->getID()] = false; // inicializa todos os nós como não visitados
    }

    resultado->adicionarNo(id_no, getNo(id_no)->getPeso());
    arvore_caminhamento_profundidade_aux(id_no, '\0' , visitado, *resultado);

    for(No *no: lista_adj) {
        if(no->getID() == id_no)
            continue; // pula o nó inicial, pois ele já foi visitado
        if(!visitado[no->getID()]) {
            resultado->adicionarNo(no->getID(), no->getPeso());
            arvore_caminhamento_profundidade_aux(no->getID(), '\0' , visitado, *resultado);
        }
    }
    return resultado;
}

void Grafo::arvore_caminhamento_profundidade_aux(char id_no, char id_pai, map<char, bool> &visitados, Grafo &resultado) 
{
    visitados[id_no] = true; // marca o nó atual como visitado
    No* no = getNo(id_no);
    
    for(Aresta *aresta : no->arestas) {
        char id_alvo = aresta->id_no_alvo;
        if(!visitados[id_alvo]) {
            resultado.adicionarNo(id_alvo, getNo(id_alvo)->getPeso());
            resultado.adicionarAresta(id_no, id_alvo, 1); // peso 1 significa que é uma aresta de ligação
            arvore_caminhamento_profundidade_aux(id_alvo, id_no, visitados, resultado);
        }
        else{
            // Se o nó já foi visitado, mas é diferente do pai, adiciona a aresta de retorno
            bool aresta_existente = false;
            for(Aresta *aresta: resultado.getNo(id_no)->arestas) {
                if(aresta->id_no_alvo == id_alvo) {
                    // Se a aresta já existe, não adiciona novamente
                    aresta_existente = true;
                }
            }

            if(aresta_existente)
                continue;
            
            if(id_pai != id_alvo)
                resultado.adicionarAresta(id_no, id_alvo, -1); // peso -1 significa que é uma aresta de retorno
        }
    }
}

vector<vector<int>> Grafo::floydWarshall() {
    // Mapeamento de IDs para índices numéricos
    vector<char> todosIDs;
    map<char, int> indiceMap;
    map<int, char> idMap;
    int n = ordem;
    
    for(int i = 0; i < n; i++) {
        char id = lista_adj[i]->getID();
        todosIDs.push_back(id);
        indiceMap[id] = i;
        idMap[i] = id;
    }

    // Inicialização da matriz de distâncias
    vector<vector<int>> matrizDistancias(n, vector<int>(n, INT_MAX));

    // Configuração da diagonal e arestas diretas
    for(int i = 0; i < n; i++) {
        matrizDistancias[i][i] = 0;
        No* no = lista_adj[i];
        
        for(Aresta* aresta : no->arestas) {
            char destinoID = aresta->getIDalvo();
            if(indiceMap.find(destinoID) != indiceMap.end()) {
                int j = indiceMap[destinoID];
                matrizDistancias[i][j] = aresta->getPeso();
            }
        }
    }

    // Algoritmo principal de Floyd-Warshall
    for(int k = 0; k < n; k++) {
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                if(matrizDistancias[i][k] != INT_MAX && 
                   matrizDistancias[k][j] != INT_MAX &&
                   matrizDistancias[i][k] + matrizDistancias[k][j] < matrizDistancias[i][j]) {
                    matrizDistancias[i][j] = matrizDistancias[i][k] + matrizDistancias[k][j];
                }
            }
        }
    }

    return matrizDistancias;
}

vector<int> Grafo::excentricidades(const vector<vector<int>>& matrizDistancias) {
    int n = matrizDistancias.size();
    vector<int> excentricidades(n, 0);

    for(int i = 0; i < n; i++) {
        int maxDist = 0;
        for(int j = 0; j < n; j++) {
            if(matrizDistancias[i][j] != INT_MAX && 
               matrizDistancias[i][j] > maxDist) {
                maxDist = matrizDistancias[i][j];
            }
        }
        excentricidades[i] = maxDist;
    }

    return excentricidades;
}

int Grafo::raio() {
    vector<vector<int>> dist = floydWarshall();
    vector<int> ecc = excentricidades(dist);
    int raio = *min_element(ecc.begin(), ecc.end());
    return raio;
}

int Grafo::diametro() {
    vector<vector<int>> dist = floydWarshall();
    vector<int> ecc = excentricidades(dist);
    int diametro = *max_element(ecc.begin(), ecc.end());
    return diametro;
}

vector<char> Grafo::centro() {
    vector<vector<int>> dist = floydWarshall();
    vector<int> ecc = excentricidades(dist);
    int r = raio();
    vector<char> centroIDs;
    
    for(int i = 0; i < ecc.size(); i++) {
        if(ecc[i] == r) {
            centroIDs.push_back(lista_adj[i]->getID());
        }
    }
    
    return centroIDs;
}

vector<char> Grafo::periferia() {
    vector<vector<int>> dist = floydWarshall();
    vector<int> ecc = excentricidades(dist);
    int d = diametro();
    vector<char> periferiaIDs;
    
    for(int i = 0; i < ecc.size(); i++) {
        if(ecc[i] == d) {
            periferiaIDs.push_back(lista_adj[i]->getID());
        }
    }
    
    return periferiaIDs;
}

vector<char> Grafo::vertices_de_articulacao() {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

string Grafo::output_csAcademy() {
    
    struct ArestaInicioFim {
        char id_inicio;
        Aresta* aresta;
    };
    
    stringstream ss;
    ss << (in_direcionado ? "1" : "0") << " ";
    ss << (in_ponderado_aresta ? "1" : "0") << " ";
    ss << (in_ponderado_vertice ? "1" : "0") << endl;
    ss << to_string(ordem) << endl;

    for (No* no : lista_adj) {
        ss << no->getID();
        if (in_ponderado_vertice) {
            ss << " " << no->getPeso();
        }
        ss << endl;
    }

    vector<ArestaInicioFim*> arestas;
    for(No* no : lista_adj) {
        for(Aresta* aresta : no->arestas) {
            auto it = find_if(arestas.begin(), arestas.end(),
            [aresta, no](const ArestaInicioFim* a)
            {
                return a->id_inicio == aresta->id_no_alvo
                && a->aresta->id_no_alvo == no->getID();
            });

            if (it == arestas.end() || in_direcionado) {
                ss << no->getID() << " " << aresta->getIDalvo();
                if (in_ponderado_aresta) {
                    ss << " " << aresta->getPeso();
                }
                ss << endl;
                ArestaInicioFim* aresta_inicio_fim = new ArestaInicioFim();
                aresta_inicio_fim->id_inicio = no->getID();
                aresta_inicio_fim->aresta = aresta;
                arestas.push_back(aresta_inicio_fim);
            }
        }
    }
    return ss.str();
}

string Grafo::toString() {
    stringstream ss;

    sort(lista_adj.begin(), lista_adj.end(),
         [](No* a, No* b) {
             return a->getID() < b->getID();
         });

    for(No* no : lista_adj) {
        ss << no->getID() << ":";
        int cont = 0;
        for(Aresta* aresta : no->arestas) {
            ss << (cont++ == 0 ? " " : " -> ") << aresta->getIDalvo();
        }
        ss << endl;
    }
    return ss.str();
}

int Grafo::numComponentesConexas() {
    map<char, bool> visitado;
    
    Grafo *resultado = new Grafo(false, false, true);

    for(No *no: lista_adj) {
        visitado[no->getID()] = false; // inicializa todos os nós como não visitados
    }

    int componentes = 0; // contador de componentes conexas
    for(No *no: lista_adj) {
        if(!visitado[no->getID()]) {
            componentes++; // incrementa o contador de componentes conexas
            resultado->adicionarNo(no->getID(), no->getPeso());
            arvore_caminhamento_profundidade_aux(no->getID(), '\0' , visitado, *resultado);
        }
    }
    return componentes;
}

Grafo* Grafo::getSubgrafo(vector<char> ids_nos) {
    Grafo* subgrafo = new Grafo(in_direcionado, in_ponderado_vertice, in_ponderado_aresta);

    for (char id : ids_nos) {
        No* no = getNo(id);
        subgrafo->adicionarNo(no->getID(), no->getPeso());
    }
    
    for(char id_no : ids_nos) {
        No* no = getNo(id_no);
        
        for (Aresta* aresta : no->arestas) {
            if (find(ids_nos.begin(), ids_nos.end(), aresta->getIDalvo()) != ids_nos.end()) {
                subgrafo->adicionarAresta(no->getID(), aresta->getIDalvo(), aresta->getPeso());
            }
        }
    }
    
    return subgrafo;
}

