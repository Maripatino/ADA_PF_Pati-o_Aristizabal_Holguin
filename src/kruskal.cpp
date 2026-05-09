#include "kruskal.hpp"
#include <algorithm>


struct UnionFind {
    vector<int> padre;
    vector<int> rango;

    UnionFind(int n) : padre(n), rango(n, 0) {
        for (int i = 0; i < n; i++) padre[i] = i;
    }

    int find(int x) {
        if (padre[x] != x)
            padre[x] = find(padre[x]);
        return padre[x];
    }


    bool unir(int x, int y) {
        int rx = find(x);
        int ry = find(y);
        if (rx == ry) return false;

        if (rango[rx] < rango[ry])      padre[rx] = ry;
        else if (rango[rx] > rango[ry]) padre[ry] = rx;
        else { padre[ry] = rx; rango[rx]++; }

        return true;
    }
};



ResultadoMST kruskal(Grafo& g) {

    sort(g.aristas.begin(), g.aristas.end(),
         [](const Arista& a, const Arista& b) { return a.peso < b.peso; });

    UnionFind uf(g.nNodos);
    ResultadoMST resultado;
    resultado.pesoTotal = 0.0;


    for (const Arista& a : g.aristas) {
        if (uf.unir(a.u, a.v)) {
            resultado.aristas.push_back(a);
            resultado.pesoTotal += a.peso;


            if ((int)resultado.aristas.size() == g.nNodos - 1)
                break;
        }
    }

    return resultado;
}
