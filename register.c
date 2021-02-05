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

Matrix *searchMatrix(Register *aRegister, const char *name){
    if (aRegister->listOfMatrices) {
        for (int i = 0; i < aRegister->sizes[1]; i++) {
            if (!shorterString(aRegister->listOfMatrices[i]->name, name)) return aRegister->listOfMatrices[i];
        }
    }
    return NULL;
}

Polynomial *searchPolynomial(Register *aRegister, const char *name){
    if (aRegister->listOfPolynomials) {
        for (int i = 0; i < aRegister->sizes[0]; i++) {
            if (!shorterString(aRegister->listOfPolynomials[i]->name, name)) return aRegister->listOfPolynomials[i];
        }
    }
    return NULL;
}

void addToRegister(Register *aRegister, Polynomial *newPolynomial, Matrix *newMatrix){
    if (newPolynomial) {
        if (searchPolynomial(aRegister, newPolynomial->name)) {
            for (int i = 0; i < aRegister->sizes[0]; i++) {
                if (!shorterString(aRegister->listOfPolynomials[i]->name, newPolynomial->name)) {
                    freePolynomial(aRegister->listOfPolynomials[i]);
                    aRegister->listOfPolynomials[i] = newPolynomial;
                }
            }
        } else {
            aRegister->listOfPolynomials = realloc(aRegister->listOfPolynomials, ++aRegister->sizes[0] * sizeof(Polynomial*));
            aRegister->listOfPolynomials[aRegister->sizes[0] - 1] = newPolynomial;
        }
    }
    if (newMatrix) {
        if (searchMatrix(aRegister, newMatrix->name)) {
            for (int i = 0; i < aRegister->sizes[1]; i++) {
                if (shorterString(aRegister->listOfMatrices[i]->name, newMatrix->name) == 0) {
                    freeMatrix(aRegister->listOfMatrices[i]);
                    aRegister->listOfMatrices[i] = newMatrix;
                }
            }
        } else {
            aRegister->listOfMatrices = realloc(aRegister->listOfMatrices, ++aRegister->sizes[1] * sizeof(Matrix*));
            aRegister->listOfMatrices[aRegister->sizes[1] - 1] = newMatrix;
        }
    }
}

void freeRegisterContent(Register *aRegister){
    if (aRegister) {
        for (int i = 0; i < aRegister->sizes[0]; i++) freePolynomial(aRegister->listOfPolynomials[i]);
        free(aRegister->listOfPolynomials);
        aRegister->listOfPolynomials = NULL;
        aRegister->sizes[0] = 0;
        for (int i = 0; i < aRegister->sizes[1]; i++) freeMatrix(aRegister->listOfMatrices[i]);
        free(aRegister->listOfMatrices);
        aRegister->listOfMatrices = NULL;
        aRegister->sizes[1] = 0;
    }
}

void printRegister(Register *aRegister){
    if (aRegister->listOfPolynomials) {
        printf("===============Polynomials================\n");
        for (int i = 0; i < aRegister->sizes[0]; i++) {
            printPolynomial(aRegister->listOfPolynomials[i]); printf("\n");
        }
    }
    if (aRegister->listOfMatrices) {
        printf("=================Matrices=================\n");
        for (int i = 0; i < aRegister->sizes[1]; i++) {
            printMatrix(aRegister->listOfMatrices[i]); printf("\n");
        }
    }
    if (!aRegister->listOfPolynomials && !aRegister->listOfMatrices) printf("The register is empty\n");
    else printf("==========================================\n");
}