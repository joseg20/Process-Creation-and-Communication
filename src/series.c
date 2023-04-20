#include "../include/series.h"
//Funcion que calcula la serie de Gregory-Leibniz
double gregory_leibniz_series(unsigned int n) {
    double pi = 0.0;
    for (unsigned int i = 0; i < n; ++i) {
        pi += (i % 2 == 0 ? 1.0 : -1.0) / (2 * i + 1);
    }
    pi *= 4;
    return pi;
}
//FUncion que calcula la serie de Nilakantha
double nilakantha_series(unsigned int n) {
    double pi = 3.0;
    if (n == 0) return pi;

    for (unsigned int i = 1; i <= n; ++i) {
        pi += (i % 2 == 0 ? -1.0 : 1.0) * 4.0 / (2 * i * (2 * i + 1) * (2 * i + 2));
    }
    return pi;
}
