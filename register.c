/**
 * @file register.c Functions on register
 * @author Valentin Koeltgen
 *
 * This file contain all operations on register
 */

#include "register.h"

Register *newRegister(){
    Register *reg = malloc(sizeof(Register));
    reg->sizes[0] = 0; reg->sizes[1] = 0;
    reg->listOfPolynomials = NULL;
    reg->listOfMatrices = NULL;
    return reg;
}

Matrix *searchMatrix(Register *MainRegister, const char *name){
    if (MainRegister->listOfMatrices != NULL) {
        for (int i = 0; i < MainRegister->sizes[1]; i++) {
            if (shorterString(MainRegister->listOfMatrices[i]->name, name) == 0) return MainRegister->listOfMatrices[i];
        }
    }
    return NULL;
}

Polynomial *searchPolynomial(Register *MainRegister, const char *name){
    if (MainRegister->listOfPolynomials != NULL) {
        for (int i = 0; i < MainRegister->sizes[0]; i++) {
            if (shorterString(MainRegister->listOfPolynomials[i]->name, name) == 0) return MainRegister->listOfPolynomials[i];
        }
    }
    return NULL;
}

void addToRegister(Register *MainRegister, Polynomial *newPolynomial, Matrix *newMatrix){
    if (newPolynomial != NULL) {
        if (searchPolynomial(MainRegister, newPolynomial->name) != NULL) {
            for (int i = 0; i < MainRegister->sizes[0]; i++) {
                if (shorterString(MainRegister->listOfPolynomials[i]->name, newPolynomial->name) == 0) {
                    freePolynomial(MainRegister->listOfPolynomials[i]);
                    MainRegister->listOfPolynomials[i] = newPolynomial;
                }
            }
        } else {
            MainRegister->listOfPolynomials = realloc(MainRegister->listOfPolynomials, ++MainRegister->sizes[0] * sizeof(Polynomial*));
            MainRegister->listOfPolynomials[MainRegister->sizes[0]] = newPolynomial;
        }
    } else if (newMatrix != NULL) {
        if (searchMatrix(MainRegister, newMatrix->name) != NULL) {
            for (int i = 0; i < MainRegister->sizes[1]; i++) {
                if (shorterString(MainRegister->listOfMatrices[i]->name, newMatrix->name) == 0) {
                    freeMatrix(MainRegister->listOfMatrices[i]);
                    MainRegister->listOfMatrices[i] = newMatrix;
                }
            }
        } else {
            MainRegister->listOfMatrices = realloc(MainRegister->listOfMatrices,++MainRegister->sizes[1] * sizeof(Matrix*));
            MainRegister->listOfMatrices[MainRegister->sizes[1] - 1] = newMatrix;
        }
    }
}

void freeRegisterContent(Register *mainRegister){
    if (mainRegister != NULL) {
        for (int i = 0; i < mainRegister->sizes[0]; i++) freePolynomial(mainRegister->listOfPolynomials[i]);
        free(mainRegister->listOfPolynomials);
        mainRegister->listOfPolynomials = NULL;
        for (int i = 0; i < mainRegister->sizes[1]; i++) freeMatrix(mainRegister->listOfMatrices[i]);
        free(mainRegister->listOfMatrices);
        mainRegister->listOfMatrices = NULL;
    }
}