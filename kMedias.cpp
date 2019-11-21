#include "kMedias.h"

#include <fstream>
#include <vector>
#include <random>
#include <limits>
#include <unordered_set>
#include <time.h>
#include <map>

using namespace std;

namespace KMedias {
    typedef pair<double, double> Posicao;

    struct Objeto {
        string label;
        Posicao posicao;
        int cluster;

        Objeto(string label, Posicao posicao);
    };

    Objeto::Objeto(string label, Posicao posicao) {
        this->label = label;
        this->posicao = posicao;
    }

    inline double sqr(double value) {
        return value * value;
    }

    void kMedias(string arquivoDados, int numClusters, int numIters) {
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

        // Gerar centroids
        srand(time(0));
        vector<Objeto> copia(objetos);
        for (int i = 0; i < numClusters; ++i)
            swap(copia[i], copia[i + rand() % (objetos.size() - i)]);

        vector<Posicao> centroids(numClusters);
        for (int i = 0; i < numClusters; ++i)
            centroids[i] = copia[i].posicao;

        // Loop
        while (numIters--) {
            vector<unordered_set<int>> clusters(numClusters);

            // Distribuir objetos
            for (int i = 0; i < objetos.size(); ++i) {
                int iMinDist = 0;
                double minDist = numeric_limits<double>::max();

                for (int j = 0; j < numClusters; ++j) {
                    double dist = sqrt(
                        sqr(objetos[i].posicao.first - centroids[j].first) +
                        sqr(objetos[i].posicao.second - centroids[j].second));
                    if (dist < minDist) {
                        iMinDist = j;
                        minDist = dist;
                    }
                }

                objetos[i].cluster = iMinDist;
                clusters[iMinDist].insert(i);
            }

            // Recalcular centroids
            for (int i = 0; i < numClusters; ++i) {
                Posicao centroid;

                for (auto j : clusters[i]) {
                    centroid.first += objetos[j].posicao.first;
                    centroid.second += objetos[j].posicao.second;
                }

                centroid.first /= clusters[i].size();
                centroid.second /= clusters[i].size();

                centroids[i] = centroid;
            }
        }

        // Salvar resultados
        ofstream saida(arquivoDados + ".kmedias." + to_string(numClusters) + ".out");

        map<string, int> ordenados;
        for (Objeto objeto : objetos)
            ordenados[objeto.label] = objeto.cluster;

        for (auto it : ordenados)
            saida << it.first << "\t" << it.second << "\n";
    }
}