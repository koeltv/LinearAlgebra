/**
 * @file register.c Functions on register
 * @author Valentin Koeltgen
 *
 * This file contain all operations on register
 */

#include "register.h"

Object *newObject(){
    Object *new = malloc(sizeof(Object));
    new->matrix = NULL; new->polynomial = NULL;
    return new;
}

Register *newRegister(){
    Register *reg = malloc(sizeof(Register));
    reg->sizes[0] = 0; reg->sizes[1] = 0;
    reg->listOfPolynomials = NULL; reg->listOfMatrices = NULL;
    return reg;
}

Object *searchObject(Register *aRegister, const char *name) {
    Object *result = newObject();
    if (aRegister->listOfPolynomials) {
        for (int i = 0; i < aRegister->sizes[0]; i++) {
            if (!shorterString(aRegister->listOfPolynomials[i]->name, firstWord(name))) {
                result->polynomial = aRegister->listOfPolynomials[i]; return result;
            }
        }
    }
    if (aRegister->listOfMatrices) {
        for (int i = 0; i < aRegister->sizes[1]; i++) {
            if (!shorterString(aRegister->listOfMatrices[i]->name, firstWord(name))) {
                result->matrix = aRegister->listOfMatrices[i]; return result;
            }
        }
    }
    return NULL;
}

void deleteFromRegister(Register *aRegister, Object *toDelete){
    if (toDelete->polynomial) {
        for (int i = 0; i < aRegister->sizes[0]; i++) {
            if (aRegister->listOfPolynomials[i] == toDelete->polynomial) {
                freePolynomial(aRegister->listOfPolynomials[i]);
                for (int j = i; j < aRegister->sizes[0] - 1; j++) aRegister->listOfPolynomials[j] = aRegister->listOfPolynomials[j + 1];
                if (--aRegister->sizes[0] == 0) aRegister->listOfPolynomials = NULL;
                break;
            }
        }
    }
    if (toDelete->matrix) {
        for (int i = 0; i < aRegister->sizes[1]; i++) {
            if (aRegister->listOfMatrices[i] == toDelete->matrix) {
                freeMatrix(aRegister->listOfMatrices[i]);
                for (int j = i; j < aRegister->sizes[1] - 1; j++) aRegister->listOfMatrices[j] = aRegister->listOfMatrices[j + 1];
                if (--aRegister->sizes[1] == 0) aRegister->listOfMatrices = NULL;
                break;
            }
        }
    }
}

void addToRegister(Register *aRegister, Object *newObject){
    if (newObject->polynomial) {
        Object *found = searchObject(aRegister, newObject->polynomial->name);
        if (found && found->polynomial && newObject->polynomial) { //Overwriting current polynomial
            for (int i = 0; i < aRegister->sizes[0]; i++) {
                if (!shorterString(aRegister->listOfPolynomials[i]->name, newObject->polynomial->name)) {
                    aRegister->listOfPolynomials[i] = newObject->polynomial; break;
                }
            }
        } else { //Adding new polynomial (and suppressing matrix with the same name if there is one)
            if (found && found->matrix && newObject->polynomial) deleteFromRegister(aRegister, found);
            aRegister->listOfPolynomials = realloc(aRegister->listOfPolynomials, ++aRegister->sizes[0] * sizeof(Polynomial*));
            aRegister->listOfPolynomials[aRegister->sizes[0] - 1] = newObject->polynomial;
        }
    }
    if (newObject->matrix) {
        Object *found = searchObject(aRegister, newObject->matrix->name);
        if (found && found->matrix && newObject->matrix) { //Overwriting current matrix
            for (int i = 0; i < aRegister->sizes[1]; i++) {
                if (!shorterString(aRegister->listOfMatrices[i]->name, newObject->matrix->name)) {
                    aRegister->listOfMatrices[i] = newObject->matrix; break;
                }
            }
        } else { //Adding new matrix (and suppressing polynomial with the same name if there is one)
            if (found && found->polynomial && newObject->matrix) deleteFromRegister(aRegister, found);
            aRegister->listOfMatrices = realloc(aRegister->listOfMatrices, ++aRegister->sizes[1] * sizeof(Matrix*));
            aRegister->listOfMatrices[aRegister->sizes[1] - 1] = newObject->matrix;
        }
    }
}

void freeRegisterContent(Register *aRegister){
    if (aRegister) {
        for (int i = 0; i < aRegister->sizes[0]; i++) freePolynomial(aRegister->listOfPolynomials[i]);
        free(aRegister->listOfPolynomials); aRegister->listOfPolynomials = NULL;
        aRegister->sizes[0] = 0;
        for (int i = 0; i < aRegister->sizes[1]; i++) freeMatrix(aRegister->listOfMatrices[i]);
        free(aRegister->listOfMatrices); aRegister->listOfMatrices = NULL;
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