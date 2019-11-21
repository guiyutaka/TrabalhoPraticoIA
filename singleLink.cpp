#include "singleLink.h"

#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <functional>
#include <unordered_set>
#include <iostream>
#include <map>

#include "DisjointSets.h"

using namespace std;

namespace SingleLink {
    // Single link
    typedef pair<double, double> Posicao;

    struct Objeto {
        string label;
        Posicao posicao;
        Objeto(string label, Posicao posicao) {
            this->label = label;
            this->posicao = posicao;
        }
    };

    inline double sqr(double value) {
        return value * value;
    }

    void insertNode(map<string, int> &ordenados, vector<Objeto> &objetos, DisjointNode *node, int index) {
        auto &objeto = objetos[node->item];

        ordenados[objeto.label] = index;

        for (auto &child : node->children)
            insertNode(ordenados, objetos, child, index);
    }

    void singleLink(string arquivoDados, int minClusters, int maxClusters) {
        cout << "- Single-link\n";

        // Pular cabeçalho
        ifstream entrada(arquivoDados);
        string aux;
        entrada >> aux >> aux >> aux;

        // Ler dados
        vector<Objeto> objetos;
        while (entrada >> aux) {
            double d1, d2;
            entrada >> d1 >> d2;
            objetos.push_back(Objeto(aux, Posicao(d1, d2)));
        }

        // Criar grupos
        DisjointSets sets;
        for (int i = 0; i < objetos.size(); ++i)
            sets.add(i);

        // Calcular distâncias
        vector<pair<double, pair<int, int>>> dists;
        for (int i = 0; i < objetos.size(); ++i) {
            for (int j = i + 1; j < objetos.size(); ++j) {
                double dist = sqrt(
                    sqr(objetos[i].posicao.first - objetos[j].posicao.first) +
                    sqr(objetos[i].posicao.second - objetos[j].posicao.second));
                dists.push_back({dist, {i, j}});
            }
        }

        sort(dists.begin(), dists.end(), less<pair<double, pair<int, int>>>());

        // Agrupar clusters
        for (int i = 0, j = objetos.size(); j > minClusters; ++i) {
            if (!sets.join(dists[i].second.first, dists[i].second.second))
                continue;

            if (--j > maxClusters)
                continue;

            ofstream saida(arquivoDados + ".single." + to_string(j) + ".out");

            map<string, int> ordenados;
            int index = 1;
            for (auto set : sets.sets)
                insertNode(ordenados, objetos, set.root, index++);

            for (auto it : ordenados)
                saida << it.first << " " << it.second << "\n";
        }
    }
}