#include "averageLink.h"

#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <functional>
#include <unordered_set>
#include <limits>
#include <iostream>
#include <map>

#include "DisjointSets.h"

using namespace std;

namespace AverageLink {
    // Average-link
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

    void averageLink(string arquivoDados, int minClusters, int maxClusters) {
        cout << "- Average-link\n";

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

        // Agrupar clusters
        for (int i = 0, j = objetos.size(); j > minClusters; ++i) {
            // Obter distâncias médias
            vector<pair<double, pair<DisjointSet *, DisjointSet *>>> avgDists;
            for (auto i = sets.sets.begin(); i != sets.sets.end(); ++i) {
                for (auto j = next(i); j != sets.sets.end(); ++j) {
                    // Obter distância total
                    double totalDist = 0;
                    for (auto x = i->begin(); x != NULL; x = x->next()) {
                        for (auto y = j->begin(); y != NULL; y = y->next()) {
                            totalDist += sqrt(
                                sqr(objetos[x->item].posicao.first - objetos[y->item].posicao.first) +
                                sqr(objetos[x->item].posicao.second - objetos[y->item].posicao.second));
                        }
                    }

                    // Obter e inserir distãncia média
                    int numDists = sets.sets.size() * (sets.sets.size() - 1);
                    double avgDist = totalDist / numDists;
                    avgDists.push_back({avgDist, {&*i, &*j}});
                }
            }

            // Encontrar mais próximos
            pair<double, pair<DisjointSet *, DisjointSet *>> minAvgDist = avgDists.front();
            for (auto &avgDist : avgDists)
                if (avgDist.first < minAvgDist.first)
                    minAvgDist = avgDist;

            // Tentar unir os mais próximos
            if (!sets.join(minAvgDist.second.first->root->item, minAvgDist.second.second->root->item))
                continue;

            // Atualizar e verificar número de clusters
            if (--j > maxClusters)
                continue;

            // Imprimir arquivo
            ofstream saida(arquivoDados + ".average." + to_string(j) + ".out");

            map<string, int> ordenados;
            int index = 1;
            for (auto set : sets.sets)
                insertNode(ordenados, objetos, set.root, index++);

            for (auto it : ordenados)
                saida << it.first << " " << it.second << "\n";
        }
    }
}