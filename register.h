/**
 * @file main.h Header file of register.c
 * @author Valentin Koeltgen
 */

#ifndef LINEARALGEBRA_REGISTER_H
#define LINEARALGEBRA_REGISTER_H

#include "matrix.h"

typedef struct {
    int sizes[2];
    Polynomial **listOfPolynomials;
    Matrix **listOfMatrices;
} Register;

Register *newRegister();

Matrix *searchMatrix(Register *MainRegister, const char *name);

Polynomial *searchPolynomial(Register *MainRegister, const char *name);

void addToRegister(Register *MainRegister, Polynomial *newPolynomial, Matrix *newMatrix);

void freeRegisterContent(Register *mainRegister);

#endif //LINEARALGEBRA_REGISTER_H