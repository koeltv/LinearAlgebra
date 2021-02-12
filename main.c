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

Object *extractObject(Register *mainRegister, char *command) {
    Object *result = newObject();
    if (containCharInOrder(command, "[]")) { //Create matrix
        result->matrix = readMatrixInString(command);
    } else if (containCharInOrder(command, "X")) { //Create polynomial
        result->polynomial = stringToPolynomial(command, 0, length(command));
    } else if (containValue(command)) { //Create variable
        int temp = 0;
        result->variable = newVariable(readDoubleInString(command, &temp));
    } else { //Search for existing object
        result = searchObject(mainRegister, command);
    }
    return result;
}

Object *applyOperation(Object *leftOperand, char operator, Object *rightOperand) {
    Object *result = newObject();
    if (rightOperand->polynomial && leftOperand->polynomial) { //F(X) +/-/* G(X)
        if (operator == '+') result->polynomial = pAdd(leftOperand->polynomial, rightOperand->polynomial);
        else if (operator == '-') result->polynomial = pMinus(leftOperand->polynomial, rightOperand->polynomial);
        else if (operator == '*') result->polynomial = pMultiply(leftOperand->polynomial, rightOperand->polynomial);
    } else if (rightOperand->matrix && leftOperand->matrix) { //M +/-/* N
        if (operator == '+') result->matrix = sum(leftOperand->matrix, rightOperand->matrix);
        else if (operator == '-') result->matrix = minus(leftOperand->matrix, rightOperand->matrix);
        else if (operator == '*') result->matrix = multiply(leftOperand->matrix, rightOperand->matrix);
    } else if (rightOperand->variable && leftOperand->variable) { //x +/-/* y
        if (operator == '+') result->variable = newVariable(leftOperand->variable->value + rightOperand->variable->value);
        else if (operator == '-') result->variable = newVariable(leftOperand->variable->value - rightOperand->variable->value);
        else if (operator == '*') result->variable = newVariable(leftOperand->variable->value * rightOperand->variable->value);
    } else if (operator == '*' && ((rightOperand->matrix && leftOperand->variable) || (rightOperand->variable && leftOperand->matrix))) { //M * x
        if (leftOperand->variable) result->matrix = scalarMultiply(rightOperand->matrix, leftOperand->variable->value);
        else result->matrix = scalarMultiply(leftOperand->matrix, rightOperand->variable->value);
    } else if ((rightOperand->polynomial && leftOperand->variable) || (rightOperand->variable && leftOperand->polynomial)) { //P(X) +/-/* x
        Polynomial *polynomial; Variable *variable;
        if (rightOperand->polynomial) {
            polynomial = rightOperand->polynomial; variable = leftOperand->variable;
        } else {
            polynomial = leftOperand->polynomial; variable = rightOperand->variable;
        }
        if (operator == '+') result->polynomial = pAdd(polynomial, variableToPolynomial(variable));
        else if (operator == '*') result->polynomial = pMultiply(polynomial, variableToPolynomial(variable));
        else if (operator == '-') {
            if (rightOperand->variable) result->polynomial = pMinus(polynomial, variableToPolynomial(variable));
            else result->polynomial = pMinus(variableToPolynomial(variable), polynomial);
        }
    }
    return result;
}

Object *recursiveCommandDecomposition(Register *mainRegister, char *command) {
    //If we have global parenthesis, we get rid of them
    if (everythingIsBetweenParenthesis(command)) command = extractBetweenChar(command, '(', ')');
    Object *result = NULL;

    if (containString(command, "=")) {
        result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '=', '\0'));
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
    } else if (operatorWithoutDepth(command)) {
        int firstIndex = 0, secondIndex = 0;
        //We change the indexes to surround the next operand
        nextOperator(command, &firstIndex, &secondIndex);
        //We calculate the left part (before the indexes
        result = recursiveCommandDecomposition(mainRegister, extractUpToIndex(command, firstIndex));
        //We add all other operand one by one
        while (result && command[firstIndex]) {
            Object *rightPart = recursiveCommandDecomposition(mainRegister, extractBetweenIndexes(command, firstIndex + 1, secondIndex));
            if (rightPart) result = applyOperation(result, command[firstIndex], rightPart);
            firstIndex = secondIndex;
            nextOperator(command, &firstIndex, &secondIndex);
        }
    //From here the operations are on matrices
    } else if (containCharInOrder(command, "trans()")) {
        result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result && result->matrix) result->matrix = transpose(result->matrix);
        else return NULL;
    } else if (containCharInOrder(command, "adj()")) {
        result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result && result->matrix) result->matrix = adjugate(result->matrix);
        else return NULL;
    } else if (containCharInOrder(command, "inv()")) {
        result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result && result->matrix) result->matrix = inverse(result->matrix);
        else return NULL;
    } else if (containCharInOrder(command, "eigVectors()")) {
        result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result && result->matrix) result->matrix = eigenVectors(result->matrix);
        else return NULL;
    } else if (containCharInOrder(command, "triangularise()")) {
        result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result && result->matrix) result->matrix = triangularise(result->matrix);
        else return NULL;
    } else if (containCharInOrder(command, "PLambda()")) {
        result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result && result->matrix) {
            char *stringForm = detOfStringMatrix(changeToPLambdaForm(toStringMatrix(result->matrix)));
            result->matrix = NULL;
            result->polynomial = stringToPolynomial(stringForm, 0, length(stringForm));
        } else return NULL;
    //From here the operations are on polynomials
    } else if (containCharInOrder(command, "derive()")) {
        result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result && !result->matrix) {
            if (result->polynomial) result->polynomial = derive(result->polynomial);
            else if (result->variable) {
                result->polynomial = derive(variableToPolynomial(result->variable));
                result->variable = NULL;
            }
        } else return NULL;
    //From here the operations results are variables
    } else if (containCharInOrder(command, "trace()")) { //Trace of the matrix
        result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result && result->matrix) {
            result->variable = newVariable(trace(result->matrix));
            result->matrix = NULL;
        } else return NULL;
    } else if (containCharInOrder(command, "det()")) { //Determinant of the matrix
        result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result && result->matrix) {
            result->variable = newVariable(det(result->matrix));
            result->matrix = NULL;
        } else return NULL;
    }
    else result = extractObject(mainRegister, command);

    return result;
}

void executeCommand(Register *mainRegister, char *command) {
    //Apply simple command that doesn't need processing
    if (containString(command, "help")) { //Display help file
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
        freeObject(&result);
    } else if (containCharInOrder(command, "solve()")) { //Solve polynomial or matrix
        Object *result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result) {
            if (result->polynomial) printSolutions(solve(result->polynomial));
            else if (result->matrix) printMatrix(solveAugmentedMatrix(result->matrix));
        }
        freeObject(&result);
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
int main() { //TODO Correct excessive use of pointers, see test project
    Register mainRegister = newRegister();
    printf("Please enter a command or help to see the possibilities\n");
    char *command = readString(stdin);

    do {
        //If no known operation were detected, nothing should happen
        executeCommand(&mainRegister, command);
        //Re-Initialise the command string
        if (command) free(command);
        command = readString(stdin);
    } while (command && !containString(command, "exit"));

    //freeRegister(&mainRegister);
    return EXIT_SUCCESS;
}