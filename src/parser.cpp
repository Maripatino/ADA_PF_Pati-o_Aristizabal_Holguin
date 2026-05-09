#include "parser.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

vector<Solicitud> leerCSV(const string& ruta, int& totalRegistros, int& totalNulos) {
    ifstream archivo(ruta);
    vector<Solicitud> solicitudes;

    totalRegistros = 0;
    totalNulos = 0;

    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo\n";
        return solicitudes;
    }

    string linea;
    getline(archivo, linea);

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string campo;
        Solicitud s;
        getline(ss, s.customerID, ',');
        getline(ss, campo, ',');
        getline(ss, campo, ',');
        getline(ss, campo, ',');
        getline(ss, campo, ',');

        getline(ss, campo, ',');
        s.tenure = stoi(campo);
        getline(ss, campo, ',');
        getline(ss, campo, ',');
        getline(ss, campo, ',');
        getline(ss, campo, ',');
        getline(ss, campo, ',');
        getline(ss, campo, ',');
        getline(ss, campo, ',');
        getline(ss, campo, ',');
        getline(ss, campo, ',');
        getline(ss, campo, ',');
        getline(ss, campo, ',');
        getline(ss, campo, ',');
        getline(ss, campo, ',');
        s.MonthlyCharges = stod(campo);
        getline(ss, campo, ',');
        if (campo.empty() || campo == " ") {
            s.TotalCharges = 0.0;
            totalNulos++;
        } else {
            s.TotalCharges = stod(campo);
        }
        getline(ss, s.Churn, ',');

        solicitudes.push_back(s);
        totalRegistros++;
    }

    archivo.close();
    return solicitudes;
}