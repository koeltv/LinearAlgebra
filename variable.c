/**
 * @file variable.c Functions on variables
 * @author Valentin Koeltgen
 *
 * This file contain all operations on variable
 */

#include "variable.h"

Variable newVariable(double value) {
    return (Variable) {NULL, value};
}

void freeVariable(Variable *toFree) {
    free(toFree); toFree = NULL;
}

void printVariable(Variable variable) {
    if (variable.name) printf("%s = ", variable.name);
    printf("%lf\n", variable.value);
}

double absolute(double x) {
    return x >= 0 ? x : -x;
}

double power(double x, int power) {
    if (power == 0) return 1;
    else if (power < 0) {
        x = 1 / x; power *= -1;
    }
    double result = x;
    for (int i = 1; i < power; i++) result *= x;
    return result;
}

double sqrt(double x) {
    if (x < 0) {
        return IMAGINARY;
    } else {
        if (x == 0 || x == 1) return x;
        double squareRoot = x;
        for (int i = 0; i < 100 && absolute(x - power(squareRoot, 2)) >= 1e-9; i++) squareRoot = (squareRoot + x / squareRoot) / 2;
        return squareRoot;
    }
}

int roundDouble(double value) {
    int intPart = (int) value;
    if (value >= intPart + 0.5) return intPart + 1;
    else return intPart;
}

double roundPreciseDouble(double value) {
    if (absolute((roundDouble(value)) - value) < 1e-9) return roundDouble(value);
    else return value;
}