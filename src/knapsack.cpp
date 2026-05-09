#include "knapsack.hpp"
#include <cmath>


vector<ItemMochila> extraerItems(const vector<Solicitud>& solicitudes) {
    vector<ItemMochila> items;
    for (const Solicitud& s : solicitudes) {
        if (s.Churn == "No") {
            ItemMochila item;
            item.customerID = s.customerID;
            item.peso       = (int)round(s.TotalCharges / 10.0);
            item.valor      = (int)round(s.MonthlyCharges * 10.0);
            items.push_back(item);
            if ((int)items.size() == 50) break;
        }
    }
    return items;
}

ResultadoMochila mochila01(const vector<ItemMochila>& items, int W) {
    int n = (int)items.size();
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));

    for (int i = 1; i <= n; i++) {
        int peso  = items[i - 1].peso;
        int valor = items[i - 1].valor;
        for (int w = 0; w <= W; w++) {

            dp[i][w] = dp[i - 1][w];

            if (peso <= w) {
                int conItem = dp[i - 1][w - peso] + valor;
                if (conItem > dp[i][w])
                    dp[i][w] = conItem;
            }
        }
    }


    ResultadoMochila resultado;
    resultado.valorOptimo = dp[n][W];

    int w = W;
    for (int i = n; i >= 1; i--) {

        if (dp[i][w] != dp[i - 1][w]) {
            resultado.seleccionados.push_back(items[i - 1].customerID);
            w -= items[i - 1].peso;
        }
    }

    return resultado;
}
