#ifndef KNAPSACK_HPP
#define KNAPSACK_HPP

#include <vector>
#include <string>
#include "parser.hpp"

using namespace std;


struct ItemMochila {
    string customerID;
    int    peso;
    int    valor;
};

struct ResultadoMochila {
    int            valorOptimo;
    vector<string> seleccionados;
};


vector<ItemMochila> extraerItems(const vector<Solicitud>& solicitudes);


ResultadoMochila mochila01(const vector<ItemMochila>& items, int W);

#endif
