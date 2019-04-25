#include <iostream>
#include <list>
#include <fstream>
#include <string>

using namespace std;

class Grafo {
    int ordem;
    int tamanho;
    list<int> *adjacencia;

    public:
    Grafo() {
        adjacencia = new list<int>[ordem];
    }

    void setOrdem(int ordem) {
        this->ordem = ordem;
    }

    void adicionarArestaNaoDirigida(int verticeOrigem, int verticeDestino) {
        adjacencia[verticeOrigem].push_back(verticeDestino);
        adjacencia[verticeDestino].push_back(verticeOrigem);
    }

    int obtemGrau(int vertice) {
        return adjacencia[vertice].size();
    }

    void printSeqGrau() {
        cout << "(";
        string saida;
        for (int i = 1, len = ordem; i <= len; i++) {
            saida += std::to_string(adjacencia[i].size()) + ",";
        }
        cout << saida.substr(0,saida.length()-1) << ")";
    }
};

int main() 
{
    string linha;
    ifstream arquivoEntrada;
    int nLinha = 0;

    arquivoEntrada.open("grafo1.txt");
    Grafo g1;

    if (arquivoEntrada.is_open()) {
        while (getline(arquivoEntrada, linha)) {
            if (nLinha == 0) { 
                g1.setOrdem(std::stoi(linha));
                nLinha++; 
                continue; 
            }
            g1.adicionarArestaNaoDirigida(std::stoi(linha.substr(0,1)), std::stoi(linha.substr(1,2)));
        }
    } else {
        cout << "Deu erro ao abrir o arquivo.";
        return 0;
    }

    arquivoEntrada.close();

    g1.printSeqGrau();
    return 0;
}