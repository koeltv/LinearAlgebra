/**
 * @file register.c Functions on register
 * @author Valentin Koeltgen
 *
 * This file contain all operations on register
 */

#include "register.h"

Register newRegister() {
    return (Register) {{0, 0, 0}, NULL, NULL, NULL};
}

void freeRegister(Register **toFree) {
    if (toFree) {
        if ((*toFree)->listOfPolynomials) {
            for (int i = 0; i < (*toFree)->sizes[0]; i++) freePolynomial(&(*toFree)->listOfPolynomials[i]);
            free((*toFree)->listOfPolynomials); (*toFree)->listOfPolynomials = NULL;
        }
        if ((*toFree)->listOfMatrices) {
            for (int i = 0; i < (*toFree)->sizes[1]; i++) freeMatrix(&(*toFree)->listOfMatrices[i]);
            free((*toFree)->listOfMatrices); (*toFree)->listOfMatrices = NULL;
        }
        if ((*toFree)->listOfVariables) {
            for (int i = 0; i < (*toFree)->sizes[2]; i++) freeVariable(&(*toFree)->listOfVariables[i]);
            free((*toFree)->listOfVariables); (*toFree)->listOfVariables = NULL;
        }
        free(*toFree); *toFree = NULL;
    }
}

void freeRegisterContent(Register *aRegister) {
    if (aRegister) {
        for (int i = 0; i < aRegister->sizes[0]; i++) freePolynomial(&aRegister->listOfPolynomials[i]);
        free(aRegister->listOfPolynomials); aRegister->listOfPolynomials = NULL;
        aRegister->sizes[0] = 0;
        for (int i = 0; i < aRegister->sizes[1]; i++) freeMatrix(&aRegister->listOfMatrices[i]);
        free(aRegister->listOfMatrices); aRegister->listOfMatrices = NULL;
        aRegister->sizes[1] = 0;
        for (int i = 0; i < aRegister->sizes[2]; i++) freeVariable(&aRegister->listOfVariables[i]);
        free(aRegister->listOfVariables); aRegister->listOfVariables = NULL;
        aRegister->sizes[2] = 0;
    }
}

Object *newObject() {
    Object *new = malloc(sizeof(Object));
    new->matrix = NULL; new->polynomial = NULL; new->variable = NULL;
    return new;
}

void freeObject(Object **toFree) {
    if (*toFree) {
        freeMatrix(&(*toFree)->matrix);
        freePolynomial(&(*toFree)->polynomial);
        *toFree = NULL;
    }
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
    if (aRegister->listOfVariables) {
        for (int i = 0; i < aRegister->sizes[2]; i++) {
            if (!shorterString(aRegister->listOfVariables[i]->name, firstWord(name))) {
                result->variable = aRegister->listOfVariables[i]; return result;
            }
        }
    }
    return NULL;
}

void deleteFromRegister(Register *aRegister, Object *toDelete) {
    if (toDelete->polynomial) {
        for (int i = 0; i < aRegister->sizes[0]; i++) {
            if (aRegister->listOfPolynomials[i] == toDelete->polynomial) {
                freePolynomial(&aRegister->listOfPolynomials[i]);
                for (int j = i; j < aRegister->sizes[0] - 1; j++) aRegister->listOfPolynomials[j] = aRegister->listOfPolynomials[j + 1];
                if (--aRegister->sizes[0] == 0) aRegister->listOfPolynomials = NULL;
                break;
            }
        }
    }
    if (toDelete->matrix) {
        for (int i = 0; i < aRegister->sizes[1]; i++) {
            if (aRegister->listOfMatrices[i] == toDelete->matrix) {
                freeMatrix(&aRegister->listOfMatrices[i]);
                for (int j = i; j < aRegister->sizes[1] - 1; j++) aRegister->listOfMatrices[j] = aRegister->listOfMatrices[j + 1];
                if (--aRegister->sizes[1] == 0) aRegister->listOfMatrices = NULL;
                break;
            }
        }
    }
    if (toDelete->variable) {
        for (int i = 0; i < aRegister->sizes[2]; i++) {
            if (aRegister->listOfVariables[i] == toDelete->variable) {
                freeVariable(&aRegister->listOfVariables[i]);
                for (int j = i; j < aRegister->sizes[2] - 1; j++) aRegister->listOfVariables[j] = aRegister->listOfVariables[j + 1];
                if (--aRegister->sizes[2] == 0) aRegister->listOfVariables = NULL;
                break;
            }
        }
    }
}

void addToRegister(Register *aRegister, Object *newObject) {
    if (newObject->polynomial && newObject->polynomial->name) {
        Object *found = searchObject(aRegister, newObject->polynomial->name);
        if (found && found->polynomial) { //Overwriting current polynomial
            for (int i = 0; i < aRegister->sizes[0]; i++) {
                if (!shorterString(aRegister->listOfPolynomials[i]->name, newObject->polynomial->name)) {
                    aRegister->listOfPolynomials[i] = newObject->polynomial; break;
                }
            }
            printf("Overwrote polynomial %s\n", newObject->polynomial->name);
        } else { //Adding new polynomial (and suppressing object with the same name if there is one)
            if (found && (found->matrix || found->variable)) {
                deleteFromRegister(aRegister, found);
                printf("Overwrote object %s\n", newObject->polynomial->name);
            } else printf("New polynomial %s added\n", newObject->polynomial->name);
            aRegister->listOfPolynomials = realloc(aRegister->listOfPolynomials, ++aRegister->sizes[0] * sizeof(Polynomial*));
            aRegister->listOfPolynomials[aRegister->sizes[0] - 1] = newObject->polynomial;
        }
    }
    if (newObject->matrix && newObject->matrix->name) {
        Object *found = searchObject(aRegister, newObject->matrix->name);
        if (found && found->matrix) { //Overwriting current matrix
            for (int i = 0; i < aRegister->sizes[1]; i++) {
                if (!shorterString(aRegister->listOfMatrices[i]->name, newObject->matrix->name)) {
                    aRegister->listOfMatrices[i] = newObject->matrix; break;
                }
            }
            printf("Overwrote matrix %s\n", newObject->matrix->name);
        } else { //Adding new matrix (and suppressing object with the same name if there is one)
            if (found && (found->polynomial || found->variable)) {
                deleteFromRegister(aRegister, found);
                printf("Overwrote object %s\n", newObject->matrix->name);
            } else printf("New matrix %s added\n", newObject->matrix->name);
            aRegister->listOfMatrices = realloc(aRegister->listOfMatrices, ++aRegister->sizes[1] * sizeof(Matrix*));
            aRegister->listOfMatrices[aRegister->sizes[1] - 1] = newObject->matrix;
        }
    }
    if (newObject->variable && newObject->variable->name) {
        Object *found = searchObject(aRegister, newObject->variable->name);
        if (found && found->variable) { //Overwriting current variable
            for (int i = 0; i < aRegister->sizes[2]; i++) {
                if (!shorterString(aRegister->listOfVariables[i]->name, newObject->variable->name)) {
                    aRegister->listOfVariables[i] = newObject->variable; break;
                }
            }
            printf("Overwrote variable %s\n", newObject->variable->name);
        } else { //Adding new variable (and suppressing object with the same name if there is one)
            if (found && (found->polynomial || found->matrix)) {
                deleteFromRegister(aRegister, found);
                printf("Overwrote object %s\n", newObject->variable->name);
            } else printf("New variable %s added\n", newObject->variable->name);
            aRegister->listOfVariables = realloc(aRegister->listOfVariables, ++aRegister->sizes[2] * sizeof(Variable*));
            aRegister->listOfVariables[aRegister->sizes[2] - 1] = newObject->variable;
        }
    }
}

void printRegister(Register *aRegister) {
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
    if (aRegister->listOfVariables) {
        printf("=================Variables=================\n");
        for (int i = 0; i < aRegister->sizes[2]; i++) {
            printVariable(aRegister->listOfVariables[i]); printf("\n");
        }
    }
    if (!aRegister->listOfPolynomials && !aRegister->listOfMatrices && !aRegister->listOfVariables) printf("The register is empty\n");
    else printf("==========================================\n");
}