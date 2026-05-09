#include "binary_search.hpp"

int busquedaBinariaRecursiva(const vector<Solicitud>& arr, int left, int right, int k) {
    if (left > right)
        return -1;
    int mid = left + (right - left) / 2;

    if (arr[mid].tenure >= k) {
        int posible = busquedaBinariaRecursiva(arr, mid + 1, right, k);
        if (posible != -1)
            return posible;
        return mid;
    }

    return busquedaBinariaRecursiva(arr, left, mid - 1, k);
}