/**
 * @file variable.c Functions on variables
 * @author Valentin Koeltgen
 *
 * This file contain all operations on variable
 */

#include "variable.h"

Variable newVariable(double value) {
    Variable newVariable = {NULL, value};
    //Variable *newVariable = malloc(sizeof(Variable));
    //newVariable->name = NULL; newVariable->value = value;
    return newVariable;
}

void freeVariable(Variable *toFree) {
    free(toFree);
    toFree = NULL;
}

void printVariable(Variable variable) {
    if (variable.name) printf("%s = ", variable.name);
    printf("%lf\n", variable.value);
}

Polynomial variableToPolynomial(Variable variable) {
    Polynomial result = newPolynomial(0);
    result.coefficient[0] = variable.value;
    return result;
}