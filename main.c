/**
 * @file main.c Main file of the program
 * @author Valentin Koeltgen
 *
 * This is the main file of the program, it takes care of the interactions with the user
 */

#include "main.h"

Register *applyOperation(Register *mainRegister, char *operation, char operator) {
    Register *result = newRegister();
    Register *rightPart = recursiveCommandDecomposition(mainRegister, extractBetweenIndexes(operation, nextOperator(operation) + 1, length(operation) + 1));
    if (rightPart) {
        Register *leftPart = recursiveCommandDecomposition(mainRegister, extractUpToIndex(operation, nextOperator(operation)));
        if (leftPart) {
            if (rightPart->listOfPolynomials && leftPart->listOfPolynomials) {
                Polynomial *polynomial = NULL;
                if (operator == '+') polynomial = pAdd(leftPart->listOfPolynomials[0], rightPart->listOfPolynomials[0]);
                else if (operator == '-') polynomial = pMinus(leftPart->listOfPolynomials[0], rightPart->listOfPolynomials[0]);
                else if (operator == '*') polynomial = pMultiply(leftPart->listOfPolynomials[0], rightPart->listOfPolynomials[0]);
                addToRegister(result, polynomial, NULL);
            } else if (rightPart->listOfMatrices && leftPart->listOfMatrices) {
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

Register *extractObject(Register *mainRegister, char *command) {
    if (containCharInOrder(command, "[]")) { //Create matrix
        Matrix *M = readMatrixInString(command);
        Register *MResult = newRegister();
        addToRegister(MResult, NULL, M);
        printf("New matrix added\n");
        return MResult;
    } else if (containCharInOrder(command, "X")) { //Create polynomial
        Polynomial *P = stringToPolynomial(command, 0, length(command) + 1);
        Register *PResult = newRegister();
        addToRegister(PResult, P, NULL);
        printf("New Polynomial added\n");
        return PResult;
    } else { //Search for existing object
        for (int i = 0; i < mainRegister->sizes[0]; i++) {
            if (containString(command, mainRegister->listOfPolynomials[i]->name)) {
                Register *result = newRegister();
                addToRegister(result, mainRegister->listOfPolynomials[i], NULL);
                return result;
            }
        }
        for (int i = 0; i < mainRegister->sizes[1]; i++) {
            if (containString(command, mainRegister->listOfMatrices[i]->name)) {
                Register *result = newRegister();
                addToRegister(result, NULL, mainRegister->listOfMatrices[i]);
                return result;
            }
        }
    }
    return NULL;
}

Register *recursiveCommandDecomposition(Register *mainRegister, char *command) {
    if (containCharInOrder(command, "display()")) {
        Register *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result->listOfPolynomials) printPolynomial(result->listOfPolynomials[0]);
        else if (result->listOfMatrices) printMatrix(result->listOfMatrices[0]);
        else printf("Couldn't calculate %s", command);
        return NULL;
    } else if (containCharInOrder(command, "eig()")) { //Eigen values
        Register *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result && result->listOfMatrices) printSolutions(eigenValues(result->listOfMatrices[0]));
        return NULL;
    } else if (containCharInOrder(command, "trace()")) { //Trace of the matrix
        Register *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result && result->listOfMatrices) printf("%1.2lf\n", trace(result->listOfMatrices[0]));
        return NULL;
    } else if (containCharInOrder(command, "det()")) { //Determinant of the matrix
        Register *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result && result->listOfMatrices) printf("%1.2lf\n", det(result->listOfMatrices[0]));
        return NULL;
    } else if (containCharInOrder(command, "solve()")) { //Solve polynomial or matrix
        Register *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result && result->listOfPolynomials) {
            printSolutions(solve(result->listOfPolynomials[0]));
        } else if (result && result->listOfMatrices) {
            printMatrix(solveAugmentedMatrix(result->listOfMatrices[0]));
        }
        return NULL;
    } else if (containString(command, "=")) {
        Register *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '=', '\0'));
        if (result && result->listOfPolynomials) {
            result->listOfPolynomials[0]->name = firstWord(command);
        } else if (result && result->listOfMatrices) {
            result->listOfMatrices[0]->name = firstWord(command);
        }
        return result;
    } else if (!containString(command, "X") && (command[nextOperator(command)] == '+' || command[nextOperator(command)] == '-' || command[nextOperator(command)] == '*')) {
        return applyOperation(mainRegister, command, command[nextOperator(command)]);
    //Put special cases after this, we need to verify everything else before
    } else if (containCharInOrder(command, "trans()")) {
        Register *temp = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (temp->listOfMatrices) {
            Register *result = newRegister();
            addToRegister(result, NULL, transpose(temp->listOfMatrices[0]));
            return result;
        }
    }
    else return extractObject(mainRegister, command);
    return NULL;
}

void printFileContent(char *link, FILE *output) {
    FILE *input = NULL;
    if ((input = fopen(link, "rb"))) {
        char temp;
        while (!feof(input)) {
            fscanf(input, "%c", &temp);
            fprintf(output, "%c", temp);
        }
    }
}

void executeCommand(Register *mainRegister, char *command) {
    //Apply simple command that doesn't need processing
    if (containString(command, "help")) { //Help file
        printFileContent("../readme.md", stdout);
        printf("\n\n");
    } else if (containString(command, "displayAll")) {
        printRegister(mainRegister);
    } else if (containString(command, "clear")) {
        freeRegisterContent(mainRegister);
        printf("The register was cleared\n");
    } else if (containCharInOrder(command, "readScript()")) {
        char *fileLink = extractBetweenChar(command, '(', ')');
        readScriptFile(mainRegister, fileLink);
    } else { //If no simple command, search for a composed one
        Register *result = recursiveCommandDecomposition(mainRegister, command);
        //Print to the console or save the result
        if (result->listOfPolynomials) addToRegister(mainRegister, result->listOfPolynomials[0], NULL);
        else if (result->listOfMatrices) addToRegister(mainRegister, NULL, result->listOfMatrices[0]);
        else printf("Failed to do this operation, please verify it and try again\n");
    }
}

void readScriptFile(Register *mainRegister, char *link) {
    FILE *input = NULL;
    if ((input = fopen(link, "rb"))) {
        while (!feof(input)) { //Read a line, then execute it
            char *command = readString(input);
            executeCommand(mainRegister, command);
            free(command);
        }
    } else printf("Script not found at %s\n", link);
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
        //If no known operation were detected, nothing should happen
        executeCommand(mainRegister, command);
        //Re-Initialise the command string
        if (command) free(command);
        command = readString(stdin);
    } while (!containString(command, "exit"));

    return EXIT_SUCCESS;
}