#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include "parser.hpp"

using namespace std;


struct Arista {
    int    u, v;
    double peso;
};


struct Grafo {
    int            nNodos;
    int            nAristas;
    vector<Arista> aristas;
    double         costoPromedio;
};


Grafo construirGrafo(const vector<Solicitud>& solicitudes);

#endif
