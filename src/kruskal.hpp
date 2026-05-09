#ifndef KRUSKAL_HPP
#define KRUSKAL_HPP

#include <vector>
#include "graph.hpp"

using namespace std;


struct ResultadoMST {
    vector<Arista> aristas;
    double         pesoTotal;
};

ResultadoMST kruskal(Grafo& g);

#endif
