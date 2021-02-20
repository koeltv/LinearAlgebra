/**
 * @file register.c Functions on register
 * @author Valentin Koeltgen
 *
 * This file contain all operations on register
 */

#include "register.h"

void freeRegisterContent(Register *aRegister) {
    if (aRegister) {
        free(aRegister->listOfPolynomials); aRegister->listOfPolynomials = NULL;
        aRegister->sizes[POLYNOMIAL] = 0;
        free(aRegister->listOfMatrices); aRegister->listOfMatrices = NULL;
        aRegister->sizes[MATRIX] = 0;
        free(aRegister->listOfVariables); aRegister->listOfVariables = NULL;
        aRegister->sizes[VARIABLE] = 0;
    }
}

Object checkObject(Object input) {
    if ((input.type == POLYNOMIAL && input.any.polynomial.highestDegree < 0) ||
        (input.type == MATRIX && (input.any.matrix.rows < 1 || input.any.matrix.columns < 1)) ||
        (input.type == VARIABLE && input.any.variable.value == IMAGINARY))
        return newObject;
    else return input;
}

Object searchObject(Register *aRegister, const char *name) {
    if (aRegister->listOfPolynomials) {
        for (int i = 0; i < aRegister->sizes[POLYNOMIAL]; i++) {
            if (!shorterString(aRegister->listOfPolynomials[i].name, firstWord(name))) {
                return (Object) {POLYNOMIAL, .any.polynomial = aRegister->listOfPolynomials[i]};
            }
        }
    }
    if (aRegister->listOfMatrices) {
        for (int i = 0; i < aRegister->sizes[MATRIX]; i++) {
            if (!shorterString(aRegister->listOfMatrices[i].name, firstWord(name))) {
                return (Object) {MATRIX, .any.matrix = aRegister->listOfMatrices[i]};
            }
        }
    }
    if (aRegister->listOfVariables) {
        for (int i = 0; i < aRegister->sizes[VARIABLE]; i++) {
            if (!shorterString(aRegister->listOfVariables[i].name, firstWord(name))) {
                return (Object) {VARIABLE, .any.variable = aRegister->listOfVariables[i]};
            }
        }
    }
    return newObject;
}

void deleteFromRegister(Register *aRegister, Object toDelete) {
    if (toDelete.type == POLYNOMIAL) {
        for (int i = 0; i < aRegister->sizes[POLYNOMIAL]; i++) {
            if (!shorterString(aRegister->listOfPolynomials[i].name, toDelete.any.polynomial.name)) {
                freePolynomial(&aRegister->listOfPolynomials[i]);
                for (int j = i; j < aRegister->sizes[POLYNOMIAL] - 1; j++) aRegister->listOfPolynomials[j] = aRegister->listOfPolynomials[j + 1];
                if (--aRegister->sizes[POLYNOMIAL] == 0) aRegister->listOfPolynomials = NULL;
                break;
            }
        }
    } else if (toDelete.type == MATRIX) {
        for (int i = 0; i < aRegister->sizes[MATRIX]; i++) {
                if (!shorterString(aRegister->listOfMatrices[i].name, toDelete.any.matrix.name)) {
                freeMatrix(&aRegister->listOfMatrices[i]);
                for (int j = i; j < aRegister->sizes[MATRIX] - 1; j++) aRegister->listOfMatrices[j] = aRegister->listOfMatrices[j + 1];
                if (--aRegister->sizes[MATRIX] == 0) aRegister->listOfMatrices = NULL;
                break;
            }
        }
    } else if (toDelete.type == VARIABLE) {
        for (int i = 0; i < aRegister->sizes[VARIABLE]; i++) {
            if (!shorterString(aRegister->listOfVariables[i].name, toDelete.any.variable.name)) {
                freeVariable(&aRegister->listOfVariables[i]);
                for (int j = i; j < aRegister->sizes[VARIABLE] - 1; j++) aRegister->listOfVariables[j] = aRegister->listOfVariables[j + 1];
                if (--aRegister->sizes[VARIABLE] == 0) aRegister->listOfVariables = NULL;
                break;
            }
        }
    }
}

void addToRegister(Register *aRegister, Object toAdd) {
    if (toAdd.type == POLYNOMIAL && toAdd.any.polynomial.name) {
        Object found = searchObject(aRegister, toAdd.any.polynomial.name);
        if (found.type == POLYNOMIAL) { //Overwriting current polynomial
            for (int i = 0; i < aRegister->sizes[POLYNOMIAL]; i++) {
                if (!shorterString(aRegister->listOfPolynomials[i].name, toAdd.any.polynomial.name)) {
                    aRegister->listOfPolynomials[i] = toAdd.any.polynomial; break;
                }
            }
            printf("Overwrote polynomial %s\n", toAdd.any.polynomial.name);
        } else { //Adding new polynomial (and suppressing object with the same name if there is one)
            if (found.type == MATRIX || found.type == VARIABLE) {
                deleteFromRegister(aRegister, found);
                printf("Overwrote object %s\n", toAdd.any.polynomial.name);
            } else printf("New polynomial %s added\n", toAdd.any.polynomial.name);
            aRegister->listOfPolynomials = realloc(aRegister->listOfPolynomials, ++aRegister->sizes[POLYNOMIAL] * sizeof(Polynomial));
            aRegister->listOfPolynomials[aRegister->sizes[POLYNOMIAL] - 1] = toAdd.any.polynomial;
        }
    } else if (toAdd.type == MATRIX && toAdd.any.matrix.name) {
        Object found = searchObject(aRegister, toAdd.any.matrix.name);
        if (found.type == MATRIX) { //Overwriting current matrix
            for (int i = 0; i < aRegister->sizes[MATRIX]; i++) {
                if (!shorterString(aRegister->listOfMatrices[i].name, toAdd.any.matrix.name)) {
                    aRegister->listOfMatrices[i] = toAdd.any.matrix; break;
                }
            }
            printf("Overwrote matrix %s\n", toAdd.any.matrix.name);
        } else { //Adding new matrix (and suppressing object with the same name if there is one)
            if (found.type == POLYNOMIAL || found.type == VARIABLE) {
                deleteFromRegister(aRegister, found);
                printf("Overwrote object %s\n", toAdd.any.matrix.name);
            } else printf("New matrix %s added\n", toAdd.any.matrix.name);
            aRegister->listOfMatrices = realloc(aRegister->listOfMatrices, ++aRegister->sizes[MATRIX] * sizeof(Matrix));
            aRegister->listOfMatrices[aRegister->sizes[MATRIX] - 1] = toAdd.any.matrix;
        }
    } else if (toAdd.type == VARIABLE && toAdd.any.variable.name) {
        Object found = searchObject(aRegister, toAdd.any.variable.name);
        if (found.type == VARIABLE) { //Overwriting current variable
            for (int i = 0; i < aRegister->sizes[VARIABLE]; i++) {
                if (!shorterString(aRegister->listOfVariables[i].name, toAdd.any.variable.name)) {
                    aRegister->listOfVariables[i] = toAdd.any.variable; break;
                }
            }
            printf("Overwrote variable %s\n", toAdd.any.variable.name);
        } else { //Adding new variable (and suppressing object with the same name if there is one)
            if (found.type == POLYNOMIAL || found.type == MATRIX) {
                deleteFromRegister(aRegister, found);
                printf("Overwrote object %s\n", toAdd.any.variable.name);
            } else printf("New variable %s added\n", toAdd.any.variable.name);
            aRegister->listOfVariables = realloc(aRegister->listOfVariables, ++aRegister->sizes[VARIABLE] * sizeof(Variable));
            aRegister->listOfVariables[aRegister->sizes[VARIABLE] - 1] = toAdd.any.variable;
        }
    }
}

void printRegister(Register *aRegister) {
    if (aRegister->listOfPolynomials) {
        printf("===============Polynomials================\n");
        for (int i = 0; i < aRegister->sizes[POLYNOMIAL]; i++) {
            if (i != 0) printf("\n");
            printPolynomial(aRegister->listOfPolynomials[i]);
        }
    }
    if (aRegister->listOfMatrices) {
        printf("=================Matrices=================\n");
        for (int i = 0; i < aRegister->sizes[MATRIX]; i++) {
            if (i != 0) printf("\n");
            printMatrix(aRegister->listOfMatrices[i]);
        }
    }
    if (aRegister->listOfVariables) {
        printf("=================Variables=================\n");
        for (int i = 0; i < aRegister->sizes[VARIABLE]; i++) {
            if (i != 0) printf("\n");
            printVariable(aRegister->listOfVariables[i]);
        }
    }
    if (!aRegister->listOfPolynomials && !aRegister->listOfMatrices && !aRegister->listOfVariables) printf("The register is empty\n");
    else printf("==========================================\n");
}