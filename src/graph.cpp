#include "graph.hpp"
#include <cmath>

Grafo construirGrafo(const vector<Solicitud>& solicitudes) {
    const int N = 20;

    vector<double> suma(N, 0.0);
    vector<int>    count(N, 0);

    for (int i = 0; i < (int)solicitudes.size(); i++) {
        int grupo = i % N;
        suma[grupo]  += solicitudes[i].MonthlyCharges;
        count[grupo] += 1;
    }


    vector<double> avg(N);
    for (int k = 0; k < N; k++) {
        double raw = suma[k] / count[k];
        avg[k] = round(raw * 100.0) / 100.0;
    }

    Grafo g;
    g.nNodos = N;
    double totalPeso = 0.0;

    for (int u = 0; u < N; u++) {
        for (int v = u + 1; v < N; v++) {
            Arista a;
            a.u    = u;
            a.v    = v;
            a.peso = floor(avg[u] + avg[v]);
            g.aristas.push_back(a);
            totalPeso += a.peso;
        }
    }

    g.nAristas     = (int)g.aristas.size();
    g.costoPromedio = totalPeso / g.nAristas;

    return g;
}
