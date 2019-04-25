#include <iostream>
#include <list>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

class Grafo {
    int ordem;
    int tamanho;
    list<int> *adjacencia;

    public:
    void setOrdem(int ordem) {
        this->ordem = ordem;
        this->tamanho = 0;
        this->adjacencia = new list<int>[ordem+1];
    }

    int getOrdem() {
        return ordem;
    }

    int getTamanho() {
        return tamanho;
    }

    void adicionarArestaNaoDirigida(int verticeOrigem, int verticeDestino) {
        this->adjacencia[verticeOrigem].push_back(verticeDestino);
        this->adjacencia[verticeDestino].push_back(verticeOrigem);
        this->tamanho++;
    }

    int obtemGrau(int vertice) {
        return this->adjacencia[vertice].size();
    }

    void printSeqGrau() {
        cout << "(";
        vector<int> graus;
        
        for (int i = 1; i <= ordem; i++) {
            graus.push_back(this->obtemGrau(i));
        }
        std::sort(graus.begin(), graus.end());
        for (int i = 0, len = graus.size(); i < len; i++) {
            cout << graus[i];
            if ((i+1) < len) cout << ",";
        }
        cout << ")";
    }
};

void importarArquivo(string arquivo, Grafo &grafo) {
    string linha;
    int nLinha = 0;
    ifstream arquivoEntrada;

    arquivoEntrada.open(arquivo);
    if (arquivoEntrada.is_open()) {
        while (getline(arquivoEntrada, linha)) {
            if (nLinha == 0) {
                grafo.setOrdem(std::stoi(linha));
                nLinha++; 
                continue; 
            }
            grafo.adicionarArestaNaoDirigida(std::stoi(linha.substr(0,1)), std::stoi(linha.substr(1,2)));
        }
        arquivoEntrada.close();
    }
}

int main() {
    Grafo g1;
    importarArquivo("grafo1.txt", g1);
    g1.printSeqGrau();
    cout << endl << g1.getTamanho();
    cout << endl << g1.getOrdem() << endl;

    Grafo g2;
    importarArquivo("grafo2.txt", g2);
    g2.printSeqGrau();
    cout << endl << g2.getTamanho();
    cout << endl << g2.getOrdem() << endl;
    return 0;
}