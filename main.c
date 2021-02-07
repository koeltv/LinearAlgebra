/**
 * @file main.c Main file of the program
 * @author Valentin Koeltgen
 *
 * This is the main file of the program, it takes care of the interactions with the user
 */

#include "main.h"

void readScriptFile(Register *mainRegister, char *link) {
    FILE *input = NULL;
    if ((input = fopen(link, "rb"))) {
        while (!feof(input)) { //Read a line, then execute it
            char *command = readString(input);
            executeCommand(mainRegister, command);
            free(command);
        }
    } else fprintf(stderr, "Script not found at %s\n", link);
}

Object *applyOperation(Register *mainRegister, char *operation, char operator) { //TODO Add priorities and depth level
    Object *result = newObject();
    Object *rightPart = recursiveCommandDecomposition(mainRegister, extractBetweenIndexes(operation, nextOperator(operation) + 1, length(operation)));
    if (rightPart) {
        Object *leftPart = recursiveCommandDecomposition(mainRegister, extractUpToIndex(operation, nextOperator(operation)));
        if (leftPart) {
            if (rightPart->polynomial && leftPart->polynomial) { //F(X) +/-/* G(X)
                if (operator == '+') result->polynomial = pAdd(leftPart->polynomial, rightPart->polynomial);
                else if (operator == '-') result->polynomial = pMinus(leftPart->polynomial, rightPart->polynomial);
                else if (operator == '*') result->polynomial = pMultiply(leftPart->polynomial, rightPart->polynomial);
            } else if (rightPart->matrix && leftPart->matrix) { //M +/-/* N
                if (operator == '+') result->matrix = sum(leftPart->matrix, rightPart->matrix);
                else if (operator == '-') result->matrix = minus(leftPart->matrix, rightPart->matrix);
                else if (operator == '*') result->matrix = multiply(leftPart->matrix, rightPart->matrix);
            } else if (rightPart->variable && leftPart->variable) { //x +/-/* y
                if (operator == '+') result->variable = newVariable(leftPart->variable->value + rightPart->variable->value);
                else if (operator == '-') result->variable = newVariable(leftPart->variable->value - rightPart->variable->value);
                else if (operator == '*') result->variable = newVariable(leftPart->variable->value * rightPart->variable->value);
            } else if (operator == '*' && ((rightPart->matrix && leftPart->variable) || (rightPart->variable && leftPart->matrix))) { //M * x
                if (leftPart->variable) result->matrix = scalarMultiply(rightPart->matrix, leftPart->variable->value);
                else result->matrix = scalarMultiply(leftPart->matrix, rightPart->variable->value);
            } else if ((rightPart->polynomial && leftPart->variable) || (rightPart->variable && leftPart->polynomial)) { //P(X) +/-/* x
                Polynomial *polynomial; Variable *variable;
                if (rightPart->polynomial) {
                    polynomial = rightPart->polynomial; variable = leftPart->variable;
                } else {
                    polynomial = leftPart->polynomial; variable = rightPart->variable;
                }
                if (operator == '+') result->polynomial = pAdd(polynomial, variableToPolynomial(variable));
                else if (operator == '*') result->polynomial = pMultiply(polynomial, variableToPolynomial(variable));
                else if (operator == '-') {
                    if (rightPart->variable) result->polynomial = pMinus(polynomial, variableToPolynomial(variable));
                    else result->polynomial = pMinus(variableToPolynomial(variable), polynomial);
                }
            }
        }
    }
    return result;
}

Object *extractObject(Register *mainRegister, char *command) {
    if (containCharInOrder(command, "[]")) { //Create matrix
        Object *result = newObject();
        result->matrix = readMatrixInString(command);
        return result;
    } else if (containCharInOrder(command, "X")) { //Create polynomial
        Object *result = newObject();
        result->polynomial = stringToPolynomial(command, 0, length(command));
        return result;
    } else if (containValue(command)) {
        Object *result = newObject();
        int temp = 0;
        result->variable = newVariable(readDoubleInString(command, &temp));
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
            } else if (result->variable) {
                result->variable = copyVariable(result->variable);
                result->variable->name = firstWord(command);
            }
        }
        return result;
    } else if ((command[nextOperator(command)] == '+' || command[nextOperator(command)] == '-' || command[nextOperator(command)] == '*')) {
        return applyOperation(mainRegister, command, command[nextOperator(command)]);
    //Put special cases after this, we need to verify everything else before
    } //From here the operations are on matrices
    else if (containCharInOrder(command, "trans()")) {
        Object *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result && result->matrix) {
            result->matrix = transpose(result->matrix);
            return result;
        } else return NULL;
    } else if (containCharInOrder(command, "adj()")) {
        Object *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result && result->matrix) {
            result->matrix = adjugate(result->matrix);
            return result;
        } else return NULL;
    } else if (containCharInOrder(command, "inv()")) {
        Object *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result && result->matrix) {
            result->matrix = inverse(result->matrix);
            return result;
        } else return NULL;
    } else if (containCharInOrder(command, "eigVectors()")) {
        Object *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result && result->matrix) {
            result->matrix = eigenVectors(result->matrix);
            return result;
        } else return NULL;
    } else if (containCharInOrder(command, "triangularise()")) {
        Object *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result && result->matrix) {
            result->matrix = triangularise(result->matrix);
            return result;
        } else return NULL;
    } else if (containCharInOrder(command, "PLambda()")) {
        Object *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result && result->matrix) {
            char *stringForm = detOfStringMatrix(changeToPLambdaForm(toStringMatrix(result->matrix)));
            result->matrix = NULL;
            result->polynomial = stringToPolynomial(stringForm, 0, length(stringForm));
            return result;
        } else return NULL;
    } //From here the operations are on polynomials
    else if (containCharInOrder(command, "derive()")) {
        Object *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result && !result->matrix) {
            if (result->polynomial) result->polynomial = derive(result->polynomial);
            else if (result->variable) {
                result->polynomial = derive(variableToPolynomial(result->variable));
                result->variable = NULL;
            }
            return result;
        } else return NULL;
    }
    else return extractObject(mainRegister, command);
}

void executeCommand(Register *mainRegister, char *command) {
    //Apply simple command that doesn't need processing
    if (containString(command, "help")) { //Help file
        printFileContent("../help.txt", stdout); printf("\n\n");
    } else if (containString(command, "displayAll")) {
        printRegister(mainRegister);
    } else if (containString(command, "clear")) {
        freeRegisterContent(mainRegister);
        printf("The register was cleared\n");
    } else if (containCharInOrder(command, "readScript()")) {
        char *fileLink = extractBetweenChar(command, '(', ')');
        readScriptFile(mainRegister, fileLink);
        free(fileLink);
    } else if (containCharInOrder(command, "display()")) {
        Object *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result) {
            if (result->polynomial) printPolynomial(result->polynomial);
            else if (result->matrix) printMatrix(result->matrix);
            else if (result->variable) printVariable(result->variable);
        } else printf("Couldn't calculate %s\n", command);
    } else if (containCharInOrder(command, "eig()")) { //Eigen values
        Object *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result && result->matrix) printSolutions(eigenValues(result->matrix));
        freeObject(result);
    } else if (containCharInOrder(command, "trace()")) { //Trace of the matrix
        Object *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result && result->matrix) printf("%1.2lf\n", trace(result->matrix));
        freeObject(result);
    } else if (containCharInOrder(command, "det()")) { //Determinant of the matrix
        Object *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result && result->matrix) printf("%1.2lf\n", det(result->matrix));
        freeObject(result);
    } else if (containCharInOrder(command, "solve()")) { //Solve polynomial or matrix
        Object *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result) {
            if (result->polynomial) printSolutions(solve(result->polynomial));
            else if (result->matrix) printMatrix(solveAugmentedMatrix(result->matrix));
        }
        freeObject(result);
    } else { //If no simple command, search for a composed one
        Object *result = recursiveCommandDecomposition(mainRegister, command);
        //Print to the console or save the result
        if (result) addToRegister(mainRegister, result);
        else fprintf(stderr, "Failed to do this operation, please verify it and try again\n");
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
        //If no known operation were detected, nothing should happen
        executeCommand(mainRegister, command);
        //Re-Initialise the command string
        if (command) free(command);
        command = readString(stdin);
    } while (!containString(command, "exit"));

    freeRegister(&mainRegister);
    return EXIT_SUCCESS;
}