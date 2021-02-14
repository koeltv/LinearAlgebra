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

Object extractObject(Register *mainRegister, char *command) {
    Object result = newObject();
    if (containCharInOrder(command, "X")) { //Create polynomial
        result.type = POLYNOMIAL;
        result.any.polynomial = stringToPolynomial(command, 0, length(command));
    } else if (containCharInOrder(command, "[]")) { //Create matrix
        result.type = MATRIX;
        result.any.matrix = readMatrixInString(command);
    } else if (containValue(command)) { //Create variable
        result.type = VARIABLE; int temp = 0;
        result.any.variable = newVariable(readDoubleInString(command, &temp));
    } else { //Search for existing object
        result = searchObject(mainRegister, command);
    }
    return result;
}

Object applyOperation(Object leftOperand, char operator, Object rightOperand) {
    Object result = newObject();
    if (rightOperand.type == POLYNOMIAL && leftOperand.type == POLYNOMIAL) { //F(X) +/-/* G(X)
        result.type = POLYNOMIAL;
        if (operator == '+') result.any.polynomial = pAdd(leftOperand.any.polynomial, rightOperand.any.polynomial);
        else if (operator == '-') result.any.polynomial = pMinus(leftOperand.any.polynomial, rightOperand.any.polynomial);
        else if (operator == '*') result.any.polynomial = pMultiply(leftOperand.any.polynomial, rightOperand.any.polynomial);
    } else if (rightOperand.type == 1 && leftOperand.type == 1) { //M +/-/* N
        result.type = MATRIX;
        if (operator == '+') result.any.matrix = sum(leftOperand.any.matrix, rightOperand.any.matrix);
        else if (operator == '-') result.any.matrix = minus(leftOperand.any.matrix, rightOperand.any.matrix);
        else if (operator == '*') result.any.matrix = multiply(leftOperand.any.matrix, rightOperand.any.matrix);
    } else if (rightOperand.type == VARIABLE && leftOperand.type == VARIABLE) { //x +/-/* y
        result.type = VARIABLE;
        if (operator == '+') result.any.variable = newVariable(leftOperand.any.variable.value + rightOperand.any.variable.value);
        else if (operator == '-') result.any.variable = newVariable(leftOperand.any.variable.value - rightOperand.any.variable.value);
        else if (operator == '*') result.any.variable = newVariable(leftOperand.any.variable.value * rightOperand.any.variable.value);
    } else if (operator == '*' && ((rightOperand.type == MATRIX && leftOperand.type == VARIABLE) || (rightOperand.type == VARIABLE && leftOperand.type == MATRIX))) { //M * x
        result.type = MATRIX;
        if (leftOperand.type == VARIABLE) result.any.matrix = scalarMultiply(rightOperand.any.matrix, leftOperand.any.variable.value);
        else result.any.matrix = scalarMultiply(leftOperand.any.matrix, rightOperand.any.variable.value);
    } else if ((rightOperand.type == POLYNOMIAL && leftOperand.type == VARIABLE) || (rightOperand.type == VARIABLE && leftOperand.type == POLYNOMIAL)) { //P(X) +/-/* x
        result.type = POLYNOMIAL;
        Polynomial *polynomial; Variable *variable;
        if (rightOperand.type == POLYNOMIAL) {
            polynomial = &rightOperand.any.polynomial;
            variable = &leftOperand.any.variable;
        } else {
            polynomial = &leftOperand.any.polynomial;
            variable = &rightOperand.any.variable;
        }
        if (operator == '+') result.any.polynomial = pAdd(*polynomial, variableToPolynomial(*variable));
        else if (operator == '*') result.any.polynomial = pMultiply(*polynomial, variableToPolynomial(*variable));
        else if (operator == '-') {
            if (rightOperand.type == VARIABLE) result.any.polynomial = pMinus(*polynomial, variableToPolynomial(*variable));
            else result.any.polynomial = pMinus(variableToPolynomial(*variable), *polynomial);
        }
    }
    return result;
}

Object recursiveCommandDecomposition(Register *mainRegister, char *command) {
    //If we have global parenthesis, we get rid of them
    if (everythingIsBetweenParenthesis(command)) command = extractBetweenChar(command, '(', ')');

    if (containString(command, "=")) {
        Object result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '=', '\0'));
        if (result.type != UNUSED) {
            if (result.type == POLYNOMIAL) result.any.polynomial.name = firstWord(command);
            else if (result.type == MATRIX) result.any.matrix.name = firstWord(command);
            else if (result.type == VARIABLE) result.any.variable.name = firstWord(command);
            addToRegister(mainRegister, result);
            return result;
        }
    } else if (operatorWithoutDepth(command)) {
        int firstIndex = 0, secondIndex = 0;
        //We change the indexes to surround the next operand
        nextOperator(command, &firstIndex, &secondIndex);
        //We calculate the left part (before the indexes
        Object result = recursiveCommandDecomposition(mainRegister, extractUpToIndex(command, firstIndex));
        //We add all other operand one by one
        while (result.type != UNUSED && command[firstIndex]) {
            Object rightPart = recursiveCommandDecomposition(mainRegister, extractBetweenIndexes(command, firstIndex + 1, secondIndex));
            if (rightPart.type != UNUSED) result = applyOperation(result, command[firstIndex], rightPart);
            firstIndex = secondIndex;
            nextOperator(command, &firstIndex, &secondIndex);
        }
        return result;
    //From here the operations are on matrices
    } else if (containCharInOrder(command, "trans()")) {
        Object result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result.type == MATRIX) return (Object) {MATRIX, .any.matrix = transpose(result.any.matrix)};
    } else if (containCharInOrder(command, "adj()")) {
        Object result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result.type == MATRIX) return (Object) {MATRIX, .any.matrix = adjugate(result.any.matrix)};
    } else if (containCharInOrder(command, "inv()")) {
        Object result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result.type == MATRIX) return (Object) {MATRIX, .any.matrix = inverse(result.any.matrix)};
    } else if (containCharInOrder(command, "eigVectors()")) {
        Object result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result.type == MATRIX) return (Object) {MATRIX, .any.matrix = eigenVectors(result.any.matrix)};
    } else if (containCharInOrder(command, "triangularise()")) {
        Object result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result.type == MATRIX) return (Object) {MATRIX, .any.matrix = triangularise(result.any.matrix)};
    } else if (containCharInOrder(command, "PLambda()")) {
        Object result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result.type == MATRIX) {
            char *stringForm = detOfStringMatrix(changeToPLambdaForm(toStringMatrix(result.any.matrix)));
            return (Object) {MATRIX, .any.polynomial = stringToPolynomial(stringForm, 0, length(stringForm))};
        }
    //From here the operations are on polynomials
    } else if (containCharInOrder(command, "derive()")) {
        Object result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result.type == POLYNOMIAL || result.type == VARIABLE) {
            if (result.type == POLYNOMIAL) return (Object) {POLYNOMIAL, .any.polynomial = derive(result.any.polynomial)};
            else if (result.type == VARIABLE) return (Object) {POLYNOMIAL, .any.polynomial = derive(variableToPolynomial(result.any.variable))};
        }
    //From here the operations results are variables
    } else if (containCharInOrder(command, "trace()")) { //Trace of the matrix
        Object result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result.type == MATRIX) return (Object) {VARIABLE, .any.variable = newVariable(trace(result.any.matrix))};
    } else if (containCharInOrder(command, "det()")) { //Determinant of the matrix
        Object result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result.type == MATRIX) return (Object) {VARIABLE, .any.variable = newVariable(det(result.any.matrix))};
    }
    else return extractObject(mainRegister, command);
    return newObject();
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
        Object result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result.type == UNUSED) fprintf(stderr, "Couldn't calculate %s\n", command);
        else if (result.type == POLYNOMIAL) printPolynomial(result.any.polynomial);
        else if (result.type == MATRIX) printMatrix(result.any.matrix);
        else if (result.type == VARIABLE) printVariable(result.any.variable);
    } else if (containCharInOrder(command, "eig()")) { //Eigen values
        Object result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result.type == MATRIX) printSolutions(eigenValues(result.any.matrix));
    } else if (containCharInOrder(command, "solve()")) { //Solve polynomial or matrix
        Object result = recursiveCommandDecomposition(mainRegister, extractBetweenChar(command, '(', ')'));
        if (result.type == POLYNOMIAL) printSolutions(solve(result.any.polynomial));
        else if (result.type == MATRIX) printMatrix(solveAugmentedMatrix(result.any.matrix));
    } else { //If no simple command, search for a composed one
        Object result = recursiveCommandDecomposition(mainRegister, command);
        //Print an error if no object was created (no command recognized)
        if (result.type == UNUSED) fprintf(stderr, "Failed to do this operation, please verify it and try again\n");
    }
}

/**
 * Main function
 * the main function is the first function to launch upon starting the program, it makes the link between all the components of the program
 * @return an integer confirming the success or failure of program end
 */
int main() {
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
    
    return EXIT_SUCCESS;
}