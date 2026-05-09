#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>

using namespace std;

struct Solicitud {
    string customerID;
    int tenure;
    double MonthlyCharges;
    double TotalCharges;
    string Churn;
};

vector<Solicitud> leerCSV(const string& ruta, int& totalRegistros, int& totalNulos);

#endif