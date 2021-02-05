/**
 * @file main.c Main file of the program
 * @author Valentin Koeltgen
 *
 * This is the main file of the program, it takes care of the interactions with the user
 */

#include "main.h"

Register *applyOperation(Register *mainRegister, char *operation, char operator){
    Register *result = newRegister();
    Register *rightPart = recursiveCommandDecomposition(mainRegister, extractBetweenIndexes(operation, nextOperator(operation) + 1, length(operation) + 1));
    if (rightPart != NULL) {
        Register *leftPart = recursiveCommandDecomposition(mainRegister, extractUpToIndex(operation, nextOperator(operation)));
        if (leftPart != NULL) {
            if (rightPart->listOfPolynomials != NULL && leftPart->listOfPolynomials != NULL) {
                Polynomial *polynomial = NULL;
                if (operator == '+') polynomial = pAdd(leftPart->listOfPolynomials[0], rightPart->listOfPolynomials[0]);
                else if (operator == '-') polynomial = pMinus(leftPart->listOfPolynomials[0], rightPart->listOfPolynomials[0]);
                else if (operator == '*') polynomial = pMultiply(leftPart->listOfPolynomials[0], rightPart->listOfPolynomials[0]);
                addToRegister(result, polynomial, NULL);
            } else if (rightPart->listOfMatrices != NULL && leftPart->listOfMatrices != NULL) {
                Matrix *matrix = NULL;
                if (operator == '+') matrix = sum(leftPart->listOfMatrices[0], rightPart->listOfMatrices[0]);
                else if (operator == '-') matrix = minus(leftPart->listOfMatrices[0], rightPart->listOfMatrices[0]);
                else if (operator == '*') matrix = multiply(leftPart->listOfMatrices[0], rightPart->listOfMatrices[0]);
                addToRegister(result, NULL, matrix);
            }
        }
    }
    return result;
}

Register *nextCommand(Register *mainRegister, char *command){
    if (containCharInOrder(command, "[]") == 1) { //Create matrix
        Matrix *M = readMatrixInString(command);
        Register *MResult = newRegister();
        addToRegister(MResult, NULL, M);
        printf("New matrix added\n");
        return MResult;
    } else if (containCharInOrder(command, "X") == 1) { //Create polynomial
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
    if (containCharInOrder(command, "display()") == 1) {
        Register *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result->listOfPolynomials != NULL) printPolynomial(result->listOfPolynomials[0]);
        else if (result->listOfMatrices != NULL) printMatrix(result->listOfMatrices[0]);
        else printf("Couldn't calculate %s", command);
        return NULL;
    } else if (containCharInOrder(command, "eig()") == 1) { //Eigen values
        Register *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result != NULL && result->listOfMatrices != NULL) printSolutions(eigenValues(result->listOfMatrices[0]));
        return NULL;
    } else if (containCharInOrder(command, "trace()") == 1) { //Trace of the matrix
        Register *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result != NULL && result->listOfMatrices != NULL) printf("%1.2lf\n", trace(result->listOfMatrices[0]));
        return NULL;
    } else if (containCharInOrder(command, "det()") == 1) { //Determinant of the matrix
        Register *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result != NULL && result->listOfMatrices != NULL) printf("%1.2lf\n", det(result->listOfMatrices[0]));
        return NULL;
    }
    else if (containCharInOrder(command, "solve()") == 1) { //Solve polynomial or matrix
        Register *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result != NULL && result->listOfPolynomials != NULL) {
            printSolutions(solve(result->listOfPolynomials[0]));
        } else if (result != NULL && result->listOfMatrices != NULL){
            printMatrix(solveAugmentedMatrix(result->listOfMatrices[0]));
        }
        return NULL;
    }
    else if (containString(command, "=") == 1) {
        Register *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '=', '\0'));
        if (result != NULL && result->listOfPolynomials != NULL) {
            result->listOfPolynomials[0]->name = firstWord(command);
        } else if (result != NULL && result->listOfMatrices != NULL) {
            result->listOfMatrices[0]->name = firstWord(command);
        }
        return result;
    } else if (command[nextOperator(command)] == '+' || command[nextOperator(command)] == '-' || command[nextOperator(command)] == '*') {
        return applyOperation(mainRegister, command, command[nextOperator(command)]);
    //Put special cases after this, we need to verify everything else before
    } else if (containCharInOrder(command, "trans()") == 1) {
        Register *temp = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (temp->listOfMatrices != NULL) {
            Register *result = newRegister();
            addToRegister(result, NULL, transpose(temp->listOfMatrices[0]));
            return result;
        }
    }
    else return nextCommand(mainRegister, command);
    return NULL;
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
    }
}

void printFileContent(char *link, FILE *output){
    FILE *input = NULL;
    if ((input = fopen(link, "rb")) != NULL) {
        char temp;
        while (!feof(input)) {
            fscanf(input, "%c", &temp);
            fprintf(output, "%c", temp);
        }
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
        if (containString(command, "help") == 1) { //Help file
            printFileContent("../readme.md", stdout);
            printf("\n\n");
        }
        else if (containString(command, "displayAll") == 1) {

        }
        else if (containString(command, "clear") == 1) {
            freeRegisterContent(mainRegister);
            printf("The register was cleared\n");
        } else { //If no simple command, search for a composed one
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