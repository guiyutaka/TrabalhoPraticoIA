#include <vector>
#include <iostream>

#include "kMedias.h"
#include "singleLink.h"
#include "averageLink.h"

using namespace std;

int main() {
    vector<pair<string, pair<int, int>>> arquivos = {
        {"c2ds1-2sp.txt", {2, 5}},
        {"c2ds3-2g.txt", {2, 5}},
        {"monkey.txt", {5, 12}}};

    for (pair<string, pair<int, int>> arquivo: arquivos) {
        cout << "Analisando " + arquivo.first + " (" << arquivo.second.first << " a " << arquivo.second.second << ")\n";

        // Executar k-médias
        cout << "- K-medias\n";
        for (int i = arquivo.second.first; i <= arquivo.second.second; ++i)
            KMedias::kMedias(arquivo.first, i, 1000);

        // Executar single-link
        SingleLink::singleLink(arquivo.first, arquivo.second.first, arquivo.second.second);

        // Executar average-link
        AverageLink::averageLink(arquivo.first, arquivo.second.first, arquivo.second.second);
    }

    return 0;
}