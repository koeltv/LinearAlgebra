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