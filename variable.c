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

double sqrt(double x) {
    if (x > 0) {
        double squareRoot = x, precision = 1e-9;
        for (int i = 0; i < 1000 && absolute(x - squareRoot * squareRoot) > precision; i++) {
            squareRoot = (squareRoot + x / squareRoot) / 2;
        }
        return squareRoot;
    } else return x;
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