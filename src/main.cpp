#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <chrono>
#include <cmath>
#include "parser.hpp"
#include "mergesort.hpp"
#include "binary_search.hpp"
#include "graph.hpp"
#include "kruskal.hpp"
#include "knapsack.hpp"

using namespace std;
using namespace chrono;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Uso: programa <ruta_csv>" << endl;
        return 1;
    }

    int totalRegistros = 0, totalNulos = 0;
    vector<Solicitud> solicitudes = leerCSV(argv[1], totalRegistros, totalNulos);

    if (solicitudes.empty()) {
        cout << "Error: no se cargaron datos." << endl;
        return 1;
    }

    cout << "============================================" << endl;
    cout << "  MODULO A - PROCESAMIENTO DE SOLICITUDES  " << endl;
    cout << "=============================================" << endl;

    cout << "\n--- A1: Parseo del CSV ---" << endl;
    cout << "Total de registros cargados:   " << totalRegistros << endl;
    cout << "TotalCharges nulos (tenure=0): " << totalNulos     << endl;

    cout << "\n--- A2: MergeSort (descendente por tenure) ---" << endl;

    mergeSort(solicitudes, 0, solicitudes.size() - 1);

    cout << "\ncustomerID       | tenure" << endl;
    cout << "-----------------+-------" << endl;
    for (int i = 0; i < 6; i++) {
        cout << solicitudes[i].customerID << " | " << solicitudes[i].tenure << endl;
    }
    cout << "..." << endl;
    cout << solicitudes.back().customerID << " | " << solicitudes.back().tenure << "  (ultimo)" << endl;


    {
        ofstream f("results/solicitudes_ordenadas.csv");
        f << "customerID,tenure,MonthlyCharges,TotalCharges,Churn\n";
        for (const Solicitud& s : solicitudes)
            f << s.customerID << "," << s.tenure << ","
              << fixed << setprecision(2) << s.MonthlyCharges << ","
              << s.TotalCharges << "," << s.Churn << "\n";
    }

    cout << "\n--- A3: Busqueda binaria recursiva ---" << endl;

    int    consultas[] = {72, 60, 45, 30, 12};
    string etiquetas[] = {"Q_A01", "Q_A02", "Q_A03", "Q_A04", "Q_A05"};

    for (int i = 0; i < 5; i++) {
        int k     = consultas[i];
        int index = busquedaBinariaRecursiva(solicitudes, 0, solicitudes.size() - 1, k);

        if (index != -1) {
            cout << etiquetas[i] << " | k=" << k
                 << " | customerID: " << solicitudes[index].customerID
                 << " | tenure: "     << solicitudes[index].tenure << endl;
        } else {
            cout << etiquetas[i] << " | k=" << k << " | No encontrado" << endl;
        }
    }


    {
        ofstream f("results/busquedas_A.txt");
        f << "RESULTADOS BUSQUEDA BINARIA RECURSIVA\n";
        f << "Consulta | k  | customerID   | tenure\n";
        f << "---------+----+--------------+-------\n";
        for (int i = 0; i < 5; i++) {
            int k = consultas[i];
            int idx = busquedaBinariaRecursiva(solicitudes, 0, solicitudes.size()-1, k);
            if (idx != -1)
                f << etiquetas[i] << "      | " << k << "  | "
                  << solicitudes[idx].customerID << " | "
                  << solicitudes[idx].tenure << "\n";
            else
                f << etiquetas[i] << "      | " << k << "  | No encontrado\n";
        }
    }

    cout << "\n--- A4: Analisis empirico de tiempos ---" << endl;

    int    tamanos[] = {1000, 3500, (int)solicitudes.size()};
    double tiempos[3];

    for (int t = 0; t < 3; t++) {
        int n = tamanos[t];
        vector<Solicitud> muestra(solicitudes.begin(), solicitudes.begin() + n);
        auto ini = high_resolution_clock::now();
        mergeSort(muestra, 0, n - 1);
        auto fin = high_resolution_clock::now();
        tiempos[t] = duration<double, milli>(fin - ini).count();
    }

    cout << fixed << setprecision(2);
    cout << "\n+--------+------------+-------------+------------------+" << endl;
    cout << "|   n    | Tiempo(ms) |  n*log2(n)  | Tiempo/nlogn*1e6 |" << endl;
    cout << "+--------+------------+-------------+------------------+" << endl;
    for (int t = 0; t < 3; t++) {
        int    n     = tamanos[t];
        double nlogn = n * log2((double)n);
        double ratio = (tiempos[t] / nlogn) * 1e6;
        cout << "| " << setw(6) << n
             << " | " << setw(10) << tiempos[t]
             << " | " << setw(11) << nlogn
             << " | " << setw(16) << ratio
             << " |" << endl;
    }
    cout << "+--------+------------+-------------+------------------+" << endl;



    cout << "\n\n============================================" << endl;
    cout << "  MODULO B - RED DE MINIMO COSTO (KRUSKAL)" << endl;
    cout << "============================================" << endl;


    cout << "\n--- B1: Construccion del grafo ---" << endl;
    Grafo grafo = construirGrafo(solicitudes);
    cout << "Numero de nodos:   " << grafo.nNodos    << endl;
    cout << "Numero de aristas: " << grafo.nAristas  << endl;
    cout << fixed << setprecision(2);
    cout << "Costo promedio de arista: " << grafo.costoPromedio << endl;

  
    cout << "\n--- B2: MST por Kruskal ---" << endl;
    ResultadoMST mst = kruskal(grafo);

    cout << "Aristas del MST (nodo_u -- nodo_v : peso):" << endl;
    for (const Arista& a : mst.aristas) {
        cout << "  Nodo " << setw(2) << a.u
             << " -- Nodo " << setw(2) << a.v
             << "  :  " << a.peso << endl;
    }
    cout << "Peso total del MST: " << mst.pesoTotal << endl;


    cout << "\n--- B4: Verificacion subgrafo primeros 5 nodos ---" << endl;
    cout << "Aristas del MST que conectan solo nodos 0-4:" << endl;
    int contSub = 0;
    double pesoSub = 0.0;
    for (const Arista& a : mst.aristas) {
        if (a.u <= 4 && a.v <= 4) {
            cout << "  Nodo " << a.u << " -- Nodo " << a.v
                 << "  :  " << a.peso << endl;
            pesoSub += a.peso;
            contSub++;
        }
    }
    if (contSub == 0)
        cout << "  (ninguna arista del MST global conecta solo nodos 0-4)" << endl;

 
    {
        ofstream f("results/mst_red.txt");
        f << "MST - RED DE MINIMO COSTO (KRUSKAL)\n";
        f << "Nodos: " << grafo.nNodos << "  |  Aristas MST: " << mst.aristas.size() << "\n\n";
        f << "nodo_u,nodo_v,peso\n";
        for (const Arista& a : mst.aristas)
            f << a.u << "," << a.v << "," << fixed << setprecision(2) << a.peso << "\n";
        f << "\nPeso total MST: " << mst.pesoTotal << "\n";
    }

 

    cout << "\n\n============================================" << endl;
    cout << "  MODULO C - ASIGNACION DE ANCHO DE BANDA  " << endl;
    cout << "============================================" << endl;


    cout << "\n--- C1: Mochila 0-1 por tabulacion ---" << endl;
    const int W = 500;
    vector<ItemMochila> items = extraerItems(solicitudes);

    cout << "Items extraidos (primeras 50 con Churn=No):" << endl;
    cout << fixed << setprecision(0);
    cout << setw(14) << "customerID"
         << setw(8)  << "peso"
         << setw(8)  << "valor" << endl;
    cout << string(30, '-') << endl;
    for (const ItemMochila& it : items) {
        cout << setw(14) << it.customerID
             << setw(8)  << it.peso
             << setw(8)  << it.valor << endl;
    }

    cout << "\nCapacidad W = " << W << endl;
    ResultadoMochila resultado = mochila01(items, W);

    cout << "Valor optimo total: " << resultado.valorOptimo << endl;
    cout << "Solicitudes seleccionadas (" << resultado.seleccionados.size() << "):" << endl;
    int pesoUsado = 0;
    for (const string& id : resultado.seleccionados) {
        for (const ItemMochila& it : items) {
            if (it.customerID == id) {
                cout << "  " << id
                     << "  peso=" << it.peso
                     << "  valor=" << it.valor << endl;
                pesoUsado += it.peso;
                break;
            }
        }
    }
    cout << "Peso total usado: " << pesoUsado << " / " << W << endl;


    cout << "\n--- C2: Fallo del enfoque codicioso ---" << endl;
    cout << "Contraejemplo con 3 solicitudes del conjunto anterior:" << endl;


    int W_sub = 0;
    ItemMochila iA, iB, iC;
    bool encontrado = false;
    for (int a = 0; a < (int)items.size() && !encontrado; a++) {
        for (int b = a+1; b < (int)items.size() && !encontrado; b++) {
            for (int c = b+1; c < (int)items.size() && !encontrado; c++) {
              
                int cap = items[b].peso + items[c].peso;
                if (cap > W) continue;
               
                double rA = (double)items[a].valor / items[a].peso;
                double rB = (double)items[b].valor / items[b].peso;
                double rC = (double)items[c].valor / items[c].peso;
                
                if (items[a].peso > cap) continue;
               
                if (rA > rB && rA > rC) {
                    int valorCodicioso = items[a].valor; 
                    int valorPD = items[b].valor + items[c].valor;
                    if (valorPD > valorCodicioso) {
                        iA = items[a]; iB = items[b]; iC = items[c];
                        W_sub = cap;
                        encontrado = true;
                    }
                }
            }
        }
    }

    if (encontrado) {
        double rA = (double)iA.valor / iA.peso;
        double rB = (double)iB.valor / iB.peso;
        double rC = (double)iC.valor / iC.peso;
        cout << "Capacidad del subejemplo: " << W_sub << endl;
        cout << setw(14) << "customerID"
             << setw(8)  << "peso"
             << setw(8)  << "valor"
             << setw(10) << "ratio v/w" << endl;
        cout << string(40, '-') << endl;
        cout << setw(14) << iA.customerID << setw(8) << iA.peso
             << setw(8)  << iA.valor
             << setw(10) << fixed << setprecision(2) << rA << endl;
        cout << setw(14) << iB.customerID << setw(8) << iB.peso
             << setw(8)  << iB.valor
             << setw(10) << rB << endl;
        cout << setw(14) << iC.customerID << setw(8) << iC.peso
             << setw(8)  << iC.valor
             << setw(10) << rC << endl;
        cout << string(40, '-') << endl;
        cout << "Codicioso elige: " << iA.customerID
             << " (mayor ratio=" << rA << ")  -> valor=" << iA.valor
             << "  NO optimo" << endl;
        cout << "PD elige:        " << iB.customerID << " + " << iC.customerID
             << "  -> valor=" << iB.valor + iC.valor
             << "  SI optimo" << endl;
    } else {
        cout << "(No se encontro contraejemplo con estos 3 items)" << endl;
    }


    cout << "\n--- C3: Reconstruccion por backtracking ---" << endl;
    cout << "La solucion optima se reconstruye recorriendo la tabla dp" << endl;
    cout << "de abajo hacia arriba: si dp[i][w] != dp[i-1][w], el item i" << endl;
    cout << "fue incluido; se reduce w en peso[i] y se sube a i-1." << endl;
    cout << "\nTraza de backtracking (solo items incluidos):" << endl;
    cout << setw(5) << "i"
         << setw(14) << "customerID"
         << setw(8)  << "peso"
         << setw(8)  << "valor"
         << setw(8)  << "w antes"
         << setw(9)  << "w despues" << endl;
    cout << string(52, '-') << endl;
    {
        int w_bt = W;
        int n_bt = (int)items.size();
        vector<vector<int>> dp_bt(n_bt + 1, vector<int>(W + 1, 0));
        for (int i = 1; i <= n_bt; i++) {
            int p = items[i-1].peso, v = items[i-1].valor;
            for (int ww = 0; ww <= W; ww++) {
                dp_bt[i][ww] = dp_bt[i-1][ww];
                if (p <= ww && dp_bt[i-1][ww-p] + v > dp_bt[i][ww])
                    dp_bt[i][ww] = dp_bt[i-1][ww-p] + v;
            }
        }
        for (int i = n_bt; i >= 1; i--) {
            if (dp_bt[i][w_bt] != dp_bt[i-1][w_bt]) {
                int w_antes = w_bt;
                w_bt -= items[i-1].peso;
                cout << setw(5) << i
                     << setw(14) << items[i-1].customerID
                     << setw(8)  << items[i-1].peso
                     << setw(8)  << items[i-1].valor
                     << setw(8)  << w_antes
                     << setw(9)  << w_bt << endl;
            }
        }
    }
    cout << "Capacidad restante: " << (W - pesoUsado) << endl;


    cout << "\n--- C4: Analisis de complejidad ---" << endl;
    cout << "n = " << items.size() << " items, W = " << W << endl;
    cout << "Tiempo:  Theta(n * W) = Theta(" << items.size()
         << " * " << W << ") = Theta("
         << items.size() * W << ")" << endl;
    cout << "Espacio: Theta(n * W) = Theta("
         << items.size() * W << ") enteros en la tabla dp" << endl;
    cout << "Es pseudopolinomial: W no es el tamano de la entrada" << endl;
    cout << "sino su valor numerico. Si W fuera 10^9, seria inviable." << endl;


    {
        ofstream f("results/asignacion_bw.txt");
        f << "ASIGNACION DE ANCHO DE BANDA - MOCHILA 0-1\n";
        f << "Capacidad W = " << W << "\n";
        f << "Valor optimo = " << resultado.valorOptimo << "\n\n";
        f << "Solicitudes seleccionadas:\n";
        f << "customerID,peso,valor\n";
        for (const string& id : resultado.seleccionados) {
            for (const ItemMochila& it : items) {
                if (it.customerID == id) {
                    f << it.customerID << "," << it.peso << "," << it.valor << "\n";
                    break;
                }
            }
        }
        f << "\nCONTRAEJEMPLO CODICIOSO:\n";
        if (encontrado) {
            double rA = (double)iA.valor / iA.peso;
            double rB = (double)iB.valor / iB.peso;
            double rC = (double)iC.valor / iC.peso;
            f << fixed << setprecision(2);
            f << "Capacidad subejemplo = " << W_sub << "\n";
            f << "Codicioso elige: " << iA.customerID
              << " (ratio=" << rA << ") -> valor=" << iA.valor << " [NO optimo]\n";
            f << "PD elige: " << iB.customerID << " + " << iC.customerID
              << " -> valor=" << iB.valor + iC.valor << " [SI optimo]\n";
        }
    }

    return 0;
}
