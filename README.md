# Proyecto ADA - Instrucciones de compilacion y ejecucion

Este proyecto resuelve tres modulos usando algoritmos clasicos:

- Modulo A: parseo, ordenamiento y busqueda sobre solicitudes.
- Modulo B: construccion de grafo y arbol de expansion minima con Kruskal.
- Modulo C: asignacion de ancho de banda con mochila 0-1.

Las instrucciones de abajo estan pensadas para Windows con PowerShell, que es donde se probo el proyecto.

## 1) Requisitos

- Tener un compilador de C++.
- Estar ubicado en la carpeta raiz del proyecto.

Estructura esperada:

- `src/` con los archivos `.cpp` y `.hpp`
- `data/WA_Fn-UseC_-Telco-Customer-Churn.csv`
- `results/` para los archivos de salida

## 2) Compilacion

En PowerShell, ejecutar este comando desde la raiz del proyecto:

```powershell
g++ -std=c++17 src/main.cpp src/parser.cpp src/mergesort.cpp src/binary_search.cpp src/graph.cpp src/kruskal.cpp src/knapsack.cpp -o programa.exe
```

Si compila bien, se genera el ejecutable `programa.exe` en la raiz.

## 3) Ejecucion

Ejecutar con la ruta del archivo CSV como argumento:

```powershell
.\programa.exe data/WA_Fn-UseC_-Telco-Customer-Churn.csv
```

Si todo sale bien, en consola se ven los resultados de los modulos A, B y C.

## 4) Archivos de salida

Al finalizar, se generan estos archivos en `results/`:

- `solicitudes_ordenadas.csv`
- `busquedas_A.txt`
- `mst_red.txt`
- `asignacion_bw.txt`



## 5) Comando rapido

```powershell
g++ -std=c++17 src/main.cpp src/parser.cpp src/mergesort.cpp src/binary_search.cpp src/graph.cpp src/kruskal.cpp src/knapsack.cpp -o programa.exe; .\programa.exe data/WA_Fn-UseC_-Telco-Customer-Churn.csv
```

## 6) Nota final

El programa espera exactamente un argumento: la ruta del CSV.
Si se ejecuta sin argumento, muestra el uso correcto en consola.
