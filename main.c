/**
 * @file main.c Main file of the program
 * @author Valentin Koeltgen
 *
 * This is the main file of the program, it takes care of the interactions with the user
 */

#include "matrix.h"

typedef struct {
    int sizes[2];
    Polynomial **listOfPolynomials;
    Matrix **listOfMatrices;
} Register;

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
            if (shorterString(MainRegister->listOfPolynomials[i]->name, name) == 0)
                return MainRegister->listOfPolynomials[i];
        }
    }
    return NULL;
}

void addToRegister(Register *MainRegister, Polynomial *newPolynomial, Matrix *newMatrix){ //TODO Test problem when update
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
    }
    if (newMatrix != NULL) {
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

Register *nextCommand(Register *mainRegister, char *command){
    if (containCharInOrder(command, "[]") == 1) { //Create matrix
        Matrix *M = readMatrixInString(command);
        Register *MResult = newRegister();
        addToRegister(MResult, NULL, M);
        printf("New matrix added\n");
        return MResult;
    } else if (containCharInOrder(command, "=X") == 1) { //Create polynomial
        Polynomial *P = stringToPolynomial(command, 0, length(command) + 1); //TODO Implement names for polynomials
        Register *PResult = newRegister();
        addToRegister(PResult, P, NULL);
        printf("New Polynomial added\n");
        return PResult;
    } else { //Search for existing object
        for (int i = 0; i < mainRegister->sizes[0]; i++) {
            if (containString(command, mainRegister->listOfPolynomials[i]->name) == 1) {
                Register *result = newRegister();
                addToRegister(result, mainRegister->listOfPolynomials[i], NULL);
                return result;
            }
        }
        for (int i = 0; i < mainRegister->sizes[1]; i++) {
            if (containString(command, mainRegister->listOfMatrices[i]->name) == 1) {
                Register *result = newRegister();
                addToRegister(result, NULL, mainRegister->listOfMatrices[i]);
                return result;
            }
        }
    }
    return NULL;
}

Register *recursiveCommandDecomposition(Register *mainRegister, char *command) {
    if (containString(command, "=") == 1) {
        Register *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '=', '\0'));
        if (result != NULL && result->listOfPolynomials != NULL) {
            result->listOfPolynomials[0]->name = firstWord(command);
        } else if (result != NULL && result->listOfMatrices != NULL) {
            result->listOfMatrices[0]->name = firstWord(command);
        }
        return result;
    } else if (nextOperator(command) == '+') {
        Register *result = newRegister();
        Register *rightPart = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '+', '\0'));
        if (rightPart != NULL) {
            Register *leftPart = recursiveCommandDecomposition(mainRegister, extractUpToChar(command, '+'));
            if (leftPart != NULL) {
                if (rightPart->listOfPolynomials != NULL) {
                    if (leftPart->listOfPolynomials != NULL) {
                        Polynomial *polynomial = pAdd(leftPart->listOfPolynomials[0], rightPart->listOfPolynomials[0]);
                        addToRegister(result, polynomial, NULL);
                    }
                } else if (rightPart->listOfMatrices != NULL) {
                    if (leftPart->listOfMatrices != NULL) {
                        Matrix *matrix = sum(leftPart->listOfMatrices[0], rightPart->listOfMatrices[0]);
                        addToRegister(result, NULL, matrix);
                    }
                }
            }
        }
        return result;
    } else if (nextOperator(command) == '-') {
        Register *result = newRegister();
        Register *rightPart = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '-', '\0'));
        if (rightPart != NULL) {
            Register *leftPart = recursiveCommandDecomposition(mainRegister, extractUpToChar(command, '-'));
            if (leftPart != NULL) {
                if (rightPart->listOfPolynomials != NULL) {
                    if (leftPart->listOfPolynomials != NULL) {
                        Polynomial *polynomial = pAdd(leftPart->listOfPolynomials[0], rightPart->listOfPolynomials[0]);
                        addToRegister(result, polynomial, NULL);
                    }
                } else if (rightPart->listOfMatrices != NULL) {
                    if (leftPart->listOfMatrices != NULL) {
                        Matrix *matrix = minus(leftPart->listOfMatrices[0], rightPart->listOfMatrices[0]);
                        addToRegister(result, NULL, matrix);
                    }
                }
            }
        }
        return result;
    } else if (nextOperator(command) == '*') {
        Register *result = newRegister();
        Register *rightPart = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '*', '\0'));
        if (rightPart != NULL) {
            Register *leftPart = recursiveCommandDecomposition(mainRegister, extractUpToChar(command, '*'));
            if (leftPart != NULL) {
                if (rightPart->listOfPolynomials != NULL) {
                    if (leftPart->listOfPolynomials != NULL) {
                        Polynomial *polynomial = pMultiply(leftPart->listOfPolynomials[0], rightPart->listOfPolynomials[0]);
                        addToRegister(result, polynomial, NULL);
                    }
                } else if (rightPart->listOfMatrices != NULL) {
                    if (leftPart->listOfMatrices != NULL) {
                        Matrix *matrix = multiply(leftPart->listOfMatrices[0], rightPart->listOfMatrices[0]);
                        addToRegister(result, NULL, matrix);
                    }
                }
            }
        }
        return result;
    } else {
        return nextCommand(mainRegister, command);
    }
}

void printResultOfOperation(Register **mainRegister, Register *result){
    if (result != NULL) {
        if (result->sizes[0] > 0) {
            addToRegister(*mainRegister, result->listOfPolynomials[0], NULL);
            printf("Operation on polynomial has succeeded\n");
        } else if (result->sizes[1] > 0) {
            addToRegister(*mainRegister, NULL, result->listOfMatrices[0]);
            printf("Operation on matrix has succeeded\n");
        } else printf("Failed to do this operation, please verify it and try again\n");
    } else {
        for (int i = 0; i < (*mainRegister)->sizes[0]; i++) {
            printPolynomial((*mainRegister)->listOfPolynomials[i]);
        }
        for (int i = 0; i < (*mainRegister)->sizes[1]; i++) {
            printMatrix((*mainRegister)->listOfMatrices[i]);
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

/**
 * Main function
 * the main function is the first function to launch upon starting the program, it makes the link between all the components of the program
 * @return an integer confirming the success or failure of program end
 */
int main() {
    Register *mainRegister = newRegister();
    printf("Please enter a command or help to see the possibilities\n");
    char *command = readString(stdin);

    do {
        //Apply simple command that doesn't need processing
        if (containString(command, "help") == 1) { //Help file TODO Create help file or use readme
            printf("Printing help file...\n");
        }
        else if (containString(command, "clear") == 1) {
            freeRegisterContent(mainRegister);
        }
        else if (containCharInOrder(command, "display()") == 1) {
            char *name = extractBetweenChar(command, '(', ')');
            Matrix *M = searchMatrix(mainRegister, name);
            if (M != NULL) printMatrix(M);
            else {
                Polynomial *P = searchPolynomial(mainRegister, name);
                if (P != NULL) printPolynomial(P);
                else printf("%s not found\n", name);
            }
        }
        else if (containCharInOrder(command, "eig()") == 1) { //Eigen values
            char *name = extractBetweenChar(command, '(', ')');
            Matrix *toUse = searchMatrix(mainRegister, name);
            if (toUse != NULL) printSolutions(eigenValues(toUse));
            else printf("The matrix %s doesn't exist\n", name);
        }
        else if (containCharInOrder(command, "trace()") == 1) { //Eigen values
            char *name = extractBetweenChar(command, '(', ')');
            Matrix *toUse = searchMatrix(mainRegister, name);
            if (toUse != NULL) printf("%1.2lf\n", trace(toUse));
            else printf("The matrix %s doesn't exist\n", name);
        }
        else if (containCharInOrder(command, "det()") == 1) { //Eigen values
            char *name = extractBetweenChar(command, '(', ')');
            Matrix *toUse = searchMatrix(mainRegister, name);
            if (toUse != NULL) {
                printf("%1.2lf\n", det(toUse));
                freeMatrix(toUse);
            }
            else printf("The matrix %s doesn't exist\n", name);
        }
        else if (containCharInOrder(command, "solve()") == 1) { //Eigen values
            char *name = extractBetweenChar(command, '(', ')');
            Matrix *toUse = searchMatrix(mainRegister, name);
            if (toUse != NULL) {
                printMatrix(solveAugmentedMatrix(toUse));
                freeMatrix(toUse);
            } else {
                Polynomial *toSolve = searchPolynomial(mainRegister, name);
                if (toSolve != NULL) printSolutions(solve(toSolve));
                freePolynomial(toSolve);
            }
        }
        else { //If no simple command, search for a composed one
            //Apply command recursively
            Register *result = recursiveCommandDecomposition(mainRegister, command);
            //Print to the console or save the result
            printResultOfOperation(&mainRegister, result);
        }
        //If no known operation were detected, nothing should happen

        //Re-Initialise the command string
        if (command != NULL) free(command);
        command = readString(stdin);
    } while (containString(command, "exit") == 0);

    return EXIT_SUCCESS;
}