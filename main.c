/**
 * @file main.c Main file of the program
 * @author Valentin Koeltgen
 *
 * This is the main file of the program, it takes care of the interactions with the user
 */

#include "main.h"

Object *applyOperation(Register *mainRegister, char *operation, char operator) {
    Object *result = newObject();
    Object *rightPart = recursiveCommandDecomposition(mainRegister, extractBetweenIndexes(operation, nextOperator(operation) + 1, length(operation) + 1));
    if (rightPart) {
        Object *leftPart = recursiveCommandDecomposition(mainRegister, extractUpToIndex(operation, nextOperator(operation)));
        if (leftPart) {
            if (rightPart->polynomial && leftPart->polynomial) {
                if (operator == '+') result->polynomial = pAdd(leftPart->polynomial, rightPart->polynomial);
                else if (operator == '-') result->polynomial = pMinus(leftPart->polynomial, rightPart->polynomial);
                else if (operator == '*') result->polynomial = pMultiply(leftPart->polynomial, rightPart->polynomial);
            } else if (rightPart->matrix && leftPart->matrix) {
                if (operator == '+') result->matrix = sum(leftPart->matrix, rightPart->matrix);
                else if (operator == '-') result->matrix = minus(leftPart->matrix, rightPart->matrix);
                else if (operator == '*') result->matrix = multiply(leftPart->matrix, rightPart->matrix);
            }
        }
    }
    return result;
}

Object *extractObject(Register *mainRegister, char *command) {
    if (containCharInOrder(command, "[]")) { //Create matrix
        Object *result = newObject();
        result->matrix = readMatrixInString(command);
        printf("New matrix added\n");
        return result;
    } else if (containCharInOrder(command, "X")) { //Create polynomial
        Object *result = newObject();
        result->polynomial = stringToPolynomial(command, 0, length(command) + 1);
        printf("New Polynomial added\n");
        return result;
    } else { //Search for existing object
        return searchObject(mainRegister, command);
    }
}

Object *recursiveCommandDecomposition(Register *mainRegister, char *command) {
    if (containString(command, "=")) {
        Object *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '=', '\0'));
        if (result) {
            if (result->polynomial) {
                result->polynomial = copyPolynomial(result->polynomial);
                result->polynomial->name = firstWord(command);
            } else if (result->matrix) {
                result->matrix = copy(result->matrix);
                result->matrix->name = firstWord(command);
            }
        }
        return result;
    } else if (!containString(command, "X") && (command[nextOperator(command)] == '+' || command[nextOperator(command)] == '-' || command[nextOperator(command)] == '*')) {
        return applyOperation(mainRegister, command, command[nextOperator(command)]);
    //Put special cases after this, we need to verify everything else before
    } else if (containCharInOrder(command, "trans()")) {
        Object *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result && result->matrix) result->matrix = transpose(result->matrix);
        return result;
    }
    else return extractObject(mainRegister, command);
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
    } else if (containCharInOrder(command, "display()")) {
        Object *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result) {
            if (result->polynomial) printPolynomial(result->polynomial);
            else if (result->matrix) printMatrix(result->matrix);
        } else printf("Couldn't calculate %s\n", command);
    } else if (containCharInOrder(command, "eig()")) { //Eigen values
        Object *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result && result->matrix) printSolutions(eigenValues(result->matrix));
    } else if (containCharInOrder(command, "trace()")) { //Trace of the matrix
        Object *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result && result->matrix) printf("%1.2lf\n", trace(result->matrix));
    } else if (containCharInOrder(command, "det()")) { //Determinant of the matrix
        Object *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result && result->matrix) printf("%1.2lf\n", det(result->matrix));
    } else if (containCharInOrder(command, "solve()")) { //Solve polynomial or matrix
        Object *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result) {
            if (result->polynomial) printSolutions(solve(result->polynomial));
            else if (result->matrix) printMatrix(solveAugmentedMatrix(result->matrix));
        }
    } else { //If no simple command, search for a composed one
        Object *result = recursiveCommandDecomposition(mainRegister, command);
        //Print to the console or save the result
        if (result) addToRegister(mainRegister, result);
        else printf("Failed to do this operation, please verify it and try again\n");
    }
}

void readScriptFile(Register *mainRegister, char *link) { //TODO Solve problem with stopping before end of file
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