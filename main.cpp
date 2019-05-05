#include <iostream>
#include <list>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

class Grafo {
    public:
    void adicionarArestaNaoDirigida(unsigned int verticeOrigem, unsigned int verticeDestino, bool somarTamanho);
    unsigned int obtemGrau(unsigned int vertice);
    void setOrdem(unsigned int ordem);
    void printSeqGrau(ofstream &arquivoSaida);
    unsigned int getOrdem();
    unsigned int getTamanho();
    void realizarBuscaProfundidade(unsigned int verticeOrigem, bool exportar, string arquivo);
    bool realizarBuscaLargura(unsigned int verticeOrigem, bool exportar, string arquivo);
    unsigned int getNivelV(unsigned int vertice);
};

class GrafoMatriz : public Grafo {
    protected:
    unsigned int ordem;
    unsigned int tamanho;
    unsigned int **matriz;
    unsigned int *nivelV;

    // método que utiliza a recursão para percorrer toda a lista de adjacencia do vértice em questão
    void realizarVisitaVertice(unsigned int vertice, bool visitados[], unsigned int pai[], int nivel, unsigned int nivelV[]) {
        // oficialmente ele foi visitado
        visitados[vertice] = true;
        cout << "Vertice " << (vertice+1) << " foi visitado." << endl;

        // vamos visitar todas as adjacencias do vertice agora....
        nivel++;
        for (unsigned int i = 0; i < ordem; i++) {
            unsigned int verticeAdjacente = matriz[vertice][i];
            // se não for adjacente apenas ignora
            if (verticeAdjacente == 0) {
                continue;
            }
            // a adjacencia ainda não foi visitada?            
            if (!visitados[i]) {
                nivelV[i] = nivel;
                pai[i] = vertice;
                // pois agora será...
               realizarVisitaVertice(i, visitados, pai, nivel, nivelV);
            }
        }
    }

    public:
    ~GrafoMatriz() {
        delete this->matriz;
    }

    void setOrdem(unsigned int ordem) {
        this->ordem = ordem;
        this->tamanho = 0;
        this->matriz = new unsigned int*[ordem];
        
        for(unsigned int i = 0; i < ordem; i++) {
            this->matriz[i] = new unsigned int[ordem]{0};
        }
    }

    void adicionarArestaNaoDirigida(unsigned int verticeOrigem, unsigned int verticeDestino, bool somarTamanho) {
        this->matriz[verticeOrigem-1][verticeDestino-1]++;
        this->matriz[verticeDestino-1][verticeOrigem-1]++;
        if (somarTamanho) {
            tamanho++;
        }
    }

    unsigned int getOrdem() {
        return ordem;
    }

    unsigned int getTamanho() {
        return tamanho;
    }

    unsigned int obtemGrau(unsigned int vertice) {
        unsigned int soma = 0;
        for (unsigned int i = 0; i < ordem; i++) {
            soma += this->matriz[vertice-1][i];
        }
        return soma;
    }

    void printSeqGrau(ofstream &arquivoSaida) {
        arquivoSaida << "(";
        vector<unsigned int> graus;
        
        for (unsigned int i = 1; i <= ordem; i++) {
            graus.push_back(this->obtemGrau(i));
        }
        std::sort(graus.begin(), graus.end());
        for (unsigned int i = 0, len = graus.size(); i < len; i++) {
            arquivoSaida << graus[i];
            if ((i+1) < len) arquivoSaida << ",";
        }
        arquivoSaida << ")";
    }

    void realizarBuscaProfundidade(unsigned int verticeOrigem, bool exportar, string arquivo) {
        cout << "BFS ... Origem: " << verticeOrigem << endl;
        
        // inicialmente nenhum vértice foi visitado
        bool *visitados = new bool[ordem]{false};
        unsigned int *pai = new unsigned int[ordem]{0};
        unsigned int nivel = 0;
        this->nivelV = new unsigned int[ordem]{0};

        // iremos buscar à partir de um vertice origem
        this->realizarVisitaVertice((verticeOrigem-1), visitados, pai, nivel, this->nivelV);

        if (exportar) {
            ofstream arquivoSaida;
            arquivoSaida.open(arquivo);
            if (arquivoSaida.is_open()) {
                for (unsigned int i = 0; i < ordem; i++) {
                    arquivoSaida << "[V] " << (i+1) << " [P] ";
                    if (this->nivelV[i] == 0) {
                        arquivoSaida << "n";
                    } else {
                        arquivoSaida << (pai[i]+1);
                    }
                    arquivoSaida << " [L] " << this->nivelV[i] << endl;
                }
                arquivoSaida.close();
            }
        }
    }

    bool realizarBuscaLargura(unsigned int verticeOrigem, bool exportar, string arquivo) {
        cout << "BFS ... Origem: " << verticeOrigem << endl;

        // inicialmente nenhum vértice foi visitado
        bool *visitados = new bool[ordem]{false};
        unsigned int *pai = new unsigned int[ordem]{0};
        unsigned int nivel = 0;
        this->nivelV = new unsigned int[ordem]{0};

        // o algoritmo se baseia na fila
        list<unsigned int> fila;

        // oficialmente ele foi visitado
        visitados[(verticeOrigem-1)] = true;
        // empilha o vertice visitado no final
        fila.push_back((verticeOrigem-1));

        list<unsigned int>::iterator i;
        while (!fila.empty()) {
            // vertice à ser explorado
            unsigned int vertice = fila.front();
            cout << "Vertice " << (vertice+1) << " foi visitado." << endl;
            // desempilha o vertice que está sendo explorado
            fila.pop_front();
            nivel++;

            // vamos visitar todas as adjacencias do vertice agora....
            for (unsigned int i = 0; i < ordem; i++) {
                unsigned int verticeAdjacente = matriz[vertice][i];
                // se não for adjacente apenas ignora
                if (verticeAdjacente == 0) {
                    continue;
                }
                // a adjacencia ainda não foi visitada?            
                if (!visitados[i]) {
                    pai[i] = vertice;
                    this->nivelV[i] = nivel;
                    // pois agora será...
                    visitados[i] = true;
                    // empilha ela para percorrer também
                    fila.push_back(i);
                }
            }
        }
        
        if (exportar) {
            ofstream arquivoSaida;
            arquivoSaida.open(arquivo);
            if (arquivoSaida.is_open()) {
                for (unsigned int i = 0; i < ordem; i++) {
                    arquivoSaida << "[V] " << (i+1) << " [P] ";
                    if (this->nivelV[i] == 0) {
                        arquivoSaida << "n";
                    } else {
                        arquivoSaida << (pai[i]+1);
                    }
                    arquivoSaida << " [L] " << this->nivelV[i] << endl;
                }
                arquivoSaida.close();
            }
        }

        for (int i = 0; i < ordem; i++) {
            if (!visitados[i]) {
                return false;
            }
        }
        return true;
    }

    unsigned int getNivelV(unsigned int vertice) {
        return this->nivelV[vertice];
    }
};

class GrafoLista : public Grafo {
    protected:
    unsigned int ordem;
    unsigned int tamanho;
    list<unsigned int> *adjacencia;
    unsigned int *nivelV;

    // método que utiliza a recursão para percorrer toda a lista de adjacencia do vértice em questão
    void realizarVisitaVertice(unsigned int vertice, bool visitados[], unsigned int pai[], int nivel, unsigned int nivelV[]) {
        // oficialmente ele foi visitado
        visitados[vertice] = true;
        cout << "Vertice " << (vertice+1) << " foi visitado." << endl;

        // vamos visitar todas as adjacencias do vertice agora....
        list<unsigned int>::iterator i;
        nivel++;
        for (i = this->adjacencia[vertice].begin(); i != this->adjacencia[vertice].end(); ++i) {
            // a adjacencia ainda não foi visitada?
            if (!visitados[*i]) {
                // pois agora será...
                pai[*i] = vertice;
                nivelV[*i] = nivel;
                realizarVisitaVertice(*i, visitados, pai, nivel, nivelV);
            }
        }
    }

    public:
    ~GrafoLista() {
        delete this->adjacencia;
    }

    void setOrdem(unsigned int ordem) {
        this->ordem = ordem;
        this->tamanho = 0;
        this->adjacencia = new list<unsigned int>[ordem];
    }

    void setTamanho(unsigned int tamanho) {
        this->tamanho = tamanho;
    }

    void adicionarArestaNaoDirigida(unsigned int verticeOrigem, unsigned int verticeDestino, bool somarTamanho) {
        this->adjacencia[verticeOrigem-1].push_back(verticeDestino-1);
        this->adjacencia[verticeDestino-1].push_back(verticeOrigem-1);
        if (somarTamanho) {
            this->tamanho++;
        }
    }

    unsigned int obtemGrau(unsigned int vertice) {
        return this->adjacencia[vertice-1].size();
    }

    unsigned int getOrdem() {
        return ordem;
    }

    unsigned int getTamanho() {
        return tamanho;
    }

    void printSeqGrau(ofstream &arquivoSaida) {
        arquivoSaida << "(";
        vector<unsigned int> graus;
        
        for (unsigned int i = 1; i <= ordem; i++) {
            graus.push_back(this->obtemGrau(i));
        }
        std::sort(graus.begin(), graus.end());
        for (unsigned int i = 0, len = graus.size(); i < len; i++) {
            arquivoSaida << graus[i];
            if ((i+1) < len) arquivoSaida << ",";
        }
        arquivoSaida << ")";
    }

    void realizarBuscaProfundidade(unsigned int verticeOrigem, bool exportar, string arquivo) {
        cout << "BFS ... Origem: " << verticeOrigem << endl;
        
        // inicialmente nenhum vértice foi visitado
        bool *visitados = new bool[ordem]{false};
        unsigned int *pai = new unsigned int[ordem]{0};
        unsigned int nivel = 0;
        this->nivelV = new unsigned int[ordem]{0};

        // iremos buscar à partir de um vertice origem
        this->realizarVisitaVertice((verticeOrigem-1), visitados, pai, nivel, this->nivelV);
        
        if (exportar) {
            ofstream arquivoSaida;
            arquivoSaida.open(arquivo);
            if (arquivoSaida.is_open()) {
                for (unsigned int i = 0; i < ordem; i++) {
                    arquivoSaida << "[V] " << (i+1) << " [P] ";
                    if (this->nivelV[i] == 0) {
                        arquivoSaida << "n";
                    } else {
                        arquivoSaida << (pai[i]+1);
                    }
                    arquivoSaida << " [L] " << this->nivelV[i] << endl;
                }
                arquivoSaida.close();
            }
        }
    }

    bool realizarBuscaLargura(unsigned int verticeOrigem, bool exportar, string arquivo) {
        cout << "BFS ... Origem: " << verticeOrigem << endl;

        // inicialmente nenhum vértice foi visitado
        bool *visitados = new bool[ordem]{false};
        unsigned int *pai = new unsigned int[ordem]{0};
        unsigned int nivel = 0;
        this->nivelV = new unsigned int[ordem]{0};

        // o algoritmo se baseia na fila
        list<unsigned int> fila;

        // oficialmente ele foi visitado
        visitados[(verticeOrigem-1)] = true;
        // empilha o vertice visitado no final
        fila.push_back((verticeOrigem-1));

        list<unsigned int>::iterator i;
        while (!fila.empty()) {
            // vertice à ser explorado
            unsigned int vertice = fila.front();
            cout << "Vertice " << (vertice+1) << " foi visitado." << endl;
            // desempilha o vertice que está sendo explorado
            fila.pop_front();
            nivel++;

            // vamos visitar todas as adjacencias do vertice agora....
            for (i = this->adjacencia[vertice].begin(); i != this->adjacencia[vertice].end(); ++i) {
                // a adjacencia ainda não foi visitada?
                if (!visitados[*i]) {
                    pai[*i] = vertice;
                    this->nivelV[*i] = nivel;
                    // pois agora será...
                    visitados[*i] = true;
                    // empilha ela para percorrer também
                    fila.push_back(*i);
                }
            }
        }

        if (exportar) {
            ofstream arquivoSaida;
            arquivoSaida.open(arquivo);
            if (arquivoSaida.is_open()) {
                for (unsigned int i = 0; i < ordem; i++) {
                    arquivoSaida << "[V] " << (i+1) << " [P] ";
                    if (this->nivelV[i] == 0) {
                        arquivoSaida << "n";
                    } else {
                        arquivoSaida << (pai[i]+1);
                    }
                    arquivoSaida << " [L] " << this->nivelV[i] << endl;
                }
                arquivoSaida.close();
            }
        }

        for (int i = 0; i < ordem; i++) {
            if (!visitados[i]) {
                return false;
            }
        }
        return true;
    }

    unsigned int getNivelV(unsigned int vertice) {
        return this->nivelV[vertice];
    }
};

void importarArquivoMatriz(string arquivo, GrafoMatriz &grafo) {
    string linha;
    unsigned int nLinha = 0;
    ifstream arquivoEntrada;

    arquivoEntrada.open(arquivo);
    if (arquivoEntrada.is_open()) {
        while (getline(arquivoEntrada, linha)) {
            if (nLinha == 0) {
                grafo.setOrdem(std::stoull(linha));
                nLinha++; 
                continue; 
            }
            unsigned int pos = linha.find(" ");
            grafo.adicionarArestaNaoDirigida(std::stoul(linha.substr(0, pos)), std::stoul(linha.substr(pos+1, sizeof(linha)+1)), true);
        }
        arquivoEntrada.close();
    }
}

void importarArquivoLista(string arquivo, GrafoLista &grafo) {
    string linha;
    unsigned int nLinha = 0;
    ifstream arquivoEntrada;

    arquivoEntrada.open(arquivo);
    if (arquivoEntrada.is_open()) {
        while (getline(arquivoEntrada, linha)) {
            if (nLinha == 0) {
                grafo.setOrdem(std::stoull(linha));
                nLinha++; 
                continue;
            }
            unsigned int pos = linha.find(" ");
            grafo.adicionarArestaNaoDirigida(std::stoul(linha.substr(0, pos)), std::stoul(linha.substr(pos+1, sizeof(linha)+1)), true);
        }
        arquivoEntrada.close();
    }
}

void resolverArquivoEnergia(string nomeArquivoEntrada, string nomeArquivoSaida) {
    string linha;
    unsigned int nLinha = 0;
    unsigned int nTeste = 0;
    ifstream arquivoEntrada;
    ofstream arquivoSaida;
    GrafoLista grafo;

    arquivoEntrada.open(nomeArquivoEntrada);
    if (arquivoEntrada.is_open()) {
        while (getline(arquivoEntrada, linha)) {
            unsigned int pos = linha.find(" ");
            if (nLinha == 0) {
                if (std::stoul(linha.substr(0, pos)) == 0 && std::stoul(linha.substr(pos+1, sizeof(linha)+1)) == 0) {
                    continue;
                }
                grafo = GrafoLista();
                if (std::stoul(linha.substr(0, pos)) < 3 || std::stoul(linha.substr(0, pos)) > 100) {
                    // restrição
                }
                if ((std::stoul(linha.substr(pos+1, sizeof(linha)+1)) < (std::stoul(linha.substr(0, pos))-1)) || (std::stoul(linha.substr(pos+1, sizeof(linha)+1)) > (std::stoul(linha.substr(0, pos))*(((std::stoul(linha.substr(0, pos))-1)/2))))) {
                    // restrição
                }
                grafo.setOrdem(std::stoul(linha.substr(0, pos)));
                grafo.setTamanho(std::stoul(linha.substr(pos+1, sizeof(linha)+1)));
                nLinha++;
                continue; 
            }
            if (nLinha == grafo.getTamanho()) {
                nTeste++;
                if (!arquivoSaida.is_open()) {
                    arquivoSaida.open(nomeArquivoSaida);
                }
                if (arquivoSaida.is_open()) {
                    arquivoSaida << "Teste " << nTeste << endl;
                    // se ao realizar um busca em largura continuar vertices não visitados significa q é desconexo
                    if (grafo.realizarBuscaLargura(1, false, "")) {
                        // o grafo não sendo desconexo significa que não existem estações sem linhas
                        arquivoSaida << "normal" << endl;
                    } else {
                        // o grafo sendo desconexo significa que existem estações sem linhas
                        arquivoSaida << "falha" << endl;
                    }
                    arquivoSaida << endl;
                }
                nLinha = 0;
            } else {
                nLinha++;
                grafo.adicionarArestaNaoDirigida(std::stoul(linha.substr(0, pos)), std::stoul(linha.substr(pos+1, sizeof(linha)+1)), false);
            }
        }
        arquivoSaida.close();
        arquivoEntrada.close();
    }
}

void resolverArquivoDengue(string nomeArquivoEntrada, string nomeArquivoSaida) {
    string linha;
    unsigned int nLinha = 0;
    unsigned int nTeste = 0;
    ifstream arquivoEntrada;
    ofstream arquivoSaida;
    GrafoLista grafo;

    arquivoEntrada.open(nomeArquivoEntrada);
    if (arquivoEntrada.is_open()) {
        while (getline(arquivoEntrada, linha)) {
            if (nLinha == 0) {
                grafo.setOrdem(std::stoul(linha));
                nLinha++; 
                continue; 
            }
            unsigned int pos = linha.find(" ");
            if (pos == std::string::npos) {
                nTeste++;
                if (nTeste > 100) {
                    // restrição
                }
                if (!arquivoSaida.is_open()) {
                    arquivoSaida.open(nomeArquivoSaida);
                }
                if (arquivoSaida.is_open()) {
                    arquivoSaida << "Teste " << nTeste << endl;
                    
                    // realiza a busca no primeiro vertice
                    grafo.realizarBuscaLargura(1, false, "");

                    unsigned int ordem = grafo.getOrdem();
                    unsigned int *niveis = new unsigned int[ordem];

                    // busca o mais longe do primeiro vertice
                    unsigned int maiorIndice = 0, maiorValor = 0, v1, v2;
                    for (unsigned int j = 0; j < ordem; j++) {
                        niveis[j] = grafo.getNivelV(j);
                        if (niveis[j] > maiorValor) {
                            maiorIndice = j;
                            maiorValor = niveis[j];
                        }
                    }

                    // realiza novamente a busca no vertice mais longe
                    v1 = (maiorIndice+1);
                    grafo.realizarBuscaLargura(v1, false, "");

                    for (unsigned int k = 0; k < ordem; k++) {
                        niveis[k] = grafo.getNivelV(k);
                    }
                    
                    // ordena sem perder os indices
                    vector<pair<int, int> > vp;  
                    for (unsigned int l = 0; l < ordem; l++) {
                        vp.push_back(make_pair(niveis[l], l));
                    }
                    sort(vp.begin(), vp.end());

                    unsigned int middle = 0;
                    if (vp.size() % 2 == 0) {
                        middle = (vp.size()/2)-1;
                    } else {
                        middle = vp.size()/2;
                    }

                    // teoricamente o vertice do meio dos 2 maiores caminhos é o central
                    v2 = vp[middle].second;

                    arquivoSaida << (v2+1) << endl;
                    arquivoSaida << endl;
                }
                if (std::stoul(linha) == 0) {
                    continue;
                }
                grafo = GrafoLista();
                grafo.setOrdem(std::stoul(linha));
            } else {
                grafo.adicionarArestaNaoDirigida(std::stoul(linha.substr(0, pos)), std::stoul(linha.substr(pos+1, sizeof(linha)+1)), true);
            }
        }
        arquivoSaida.close();
        arquivoEntrada.close();
    }
}

void exportarArquivoMatriz(string arquivo, GrafoMatriz &grafo) {
    ofstream arquivoSaida;
    arquivoSaida.open(arquivo);
    if (arquivoSaida.is_open()) {
        arquivoSaida << grafo.getOrdem() << endl;
        arquivoSaida << grafo.getTamanho() << endl;
        grafo.printSeqGrau(arquivoSaida);
        arquivoSaida.close();
    }
}

void exportarArquivoLista(string arquivo, GrafoLista &grafo) {
    ofstream arquivoSaida;
    arquivoSaida.open(arquivo);
    if (arquivoSaida.is_open()) {
        arquivoSaida << grafo.getOrdem() << endl;
        arquivoSaida << grafo.getTamanho() << endl;
        grafo.printSeqGrau(arquivoSaida);
        arquivoSaida.close();
    }
}

int main() {
    GrafoLista g1;
    importarArquivoLista("grafo4.txt", g1);
    exportarArquivoLista("out_grafo4.txt", g1);
    
    GrafoMatriz g2;
    importarArquivoMatriz("grafo4.txt", g2);
    exportarArquivoMatriz("out_grafo44.txt", g2);

    g1.realizarBuscaProfundidade(3, true, "grafo4_dfs_3.txt");
    cout << endl;
    g2.realizarBuscaProfundidade(3, true, "grafo44_dfs_3.txt");
    cout << endl;
    g1.realizarBuscaProfundidade(6, true, "grafo4_dfs_6.txt");
    cout << endl;
    g2.realizarBuscaProfundidade(6, true, "grafo44_dfs_6.txt");
    cout << endl;
    g1.realizarBuscaLargura(1, true, "grafo4_bfs_1.txt");
    cout << endl;
    g2.realizarBuscaLargura(1, true, "grafo44_bfs_1.txt");
    cout << endl;
    g1.realizarBuscaLargura(5, true, "grafo4_bfs_5.txt");
    cout << endl;
    g2.realizarBuscaLargura(5, true, "grafo44_bfs_5.txt");
    resolverArquivoEnergia("grafo_energia.txt", "out_grafo_energia.txt");
    resolverArquivoDengue("grafo_dengue.txt", "out_grafo_dengue.txt");


    return 0;
}