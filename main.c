/**
 * @file main.c Main file of the program
 * @author Valentin Koeltgen
 *
 * This is the main file of the program, it takes care of the interactions with the user
 */

#include "main.h"

//Global register, all objects are stored here
Register aRegister = newRegister, *mainRegister = &aRegister;

void readScriptFile(char *link) {
    FILE *input = fopen(link, "rb");
    if (input) {
        while (!feof(input)) { //Read a line, then execute it
            char *command = readString(input);
            if (command[0]) executeCommand(command);
            free(command);
        }
    } else fprintf(stderr, "Script not found at %s\n", link);
}

Object extractObject(char *command) {
    Object result = newObject;
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
    return checkObject(result);
}

Object applyOperation(Object leftOperand, char operator, Object rightOperand) {
    Object result = newObject;
    if (rightOperand.type == POLYNOMIAL && leftOperand.type == POLYNOMIAL) { //F(X) +,-,*,/ G(X)
        result.type = POLYNOMIAL;
        if (operator == '+') result.any.polynomial = pAdd(leftOperand.any.polynomial, rightOperand.any.polynomial);
        else if (operator == '-') result.any.polynomial = pMinus(leftOperand.any.polynomial, rightOperand.any.polynomial);
        else if (operator == '*') result.any.polynomial = pMultiply(leftOperand.any.polynomial, rightOperand.any.polynomial);
        else if (operator == '/') result.any.polynomial = pLongDivide(leftOperand.any.polynomial, rightOperand.any.polynomial);
    } else if (rightOperand.type == MATRIX && leftOperand.type == MATRIX) { //M +,-,* N
        result.type = MATRIX;
        if (operator == '+') result.any.matrix = sum(leftOperand.any.matrix, rightOperand.any.matrix);
        else if (operator == '-') result.any.matrix = minus(leftOperand.any.matrix, rightOperand.any.matrix);
        else if (operator == '*') result.any.matrix = multiply(leftOperand.any.matrix, rightOperand.any.matrix);
        else if (operator == '/') result.type = UNUSED;
    } else if (rightOperand.type == VARIABLE && leftOperand.type == VARIABLE) { //x +,-,*,/ y
        result.type = VARIABLE;
        if (operator == '+') result.any.variable = newVariable(leftOperand.any.variable.value + rightOperand.any.variable.value);
        else if (operator == '-') result.any.variable = newVariable(leftOperand.any.variable.value - rightOperand.any.variable.value);
        else if (operator == '*') result.any.variable = newVariable(leftOperand.any.variable.value * rightOperand.any.variable.value);
        else if (operator == '/') {
            if (rightOperand.any.variable.value == 0) return newObject;
            result.any.variable = newVariable(leftOperand.any.variable.value / rightOperand.any.variable.value);
        }
    } else if (operator == '*' && ((rightOperand.type == MATRIX && leftOperand.type == VARIABLE) || (rightOperand.type == VARIABLE && leftOperand.type == MATRIX))) { //M * x
        result.type = MATRIX;
        if (leftOperand.type == VARIABLE) result.any.matrix = scalarMultiply(rightOperand.any.matrix, leftOperand.any.variable.value);
        else result.any.matrix = scalarMultiply(leftOperand.any.matrix, rightOperand.any.variable.value);
    } else if ((rightOperand.type == POLYNOMIAL && leftOperand.type == VARIABLE) || (rightOperand.type == VARIABLE && leftOperand.type == POLYNOMIAL)) { //P(X) +,-,*,/ x
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
        } else if (operator == '/') {
            //Polynomial temp = newPolynomial(polynomial->highestDegree);
            if (rightOperand.type == VARIABLE) result.any.polynomial = pLongDivide(*polynomial, variableToPolynomial(*variable));
            else result.any.polynomial = pLongDivide(variableToPolynomial(*variable), *polynomial);
        }
    }
    return checkObject(result);
}

Object recursiveCommandDecomposition(char *command) {
    //If we have global parenthesis, we get rid of them
    if (everythingIsBetweenParenthesis(command)) command = extractBetweenChar(command, '(', ')');

    if (containString(command, "=")) { //Declaration of an object
        Object result = checkObject(recursiveCommandDecomposition(extractBetweenChar(command, '=', '\0')));
        char *name = firstWord(command);
        if (result.type != UNUSED && shorterString("X", name)) {
            if (result.type == POLYNOMIAL) result.any.polynomial.name = name;
            else if (result.type == MATRIX) result.any.matrix.name = name;
            else if (result.type == VARIABLE) result.any.variable.name = name;
            addToRegister(mainRegister, result);
            return result;
        } else if (!shorterString("X", name)) fprintf(stderr, "Error, can't use 'X' as a variable name\n");
    } else if (operatorWithoutDepth(command)) { //Mathematical operation
        int firstIndex = 0, secondIndex = 0;
        //We change the indexes to surround the next operand
        nextOperator(command, &firstIndex, &secondIndex);
        //We calculate the left part (before the indexes
        Object result = recursiveCommandDecomposition(extractUpToIndex(command, firstIndex));
        //We add all other operand one by one
        while (result.type != UNUSED && command[firstIndex]) {
            Object rightPart = recursiveCommandDecomposition(extractBetweenIndexes(command, firstIndex + 1, secondIndex));
            if (rightPart.type != UNUSED) result = applyOperation(result, command[firstIndex], rightPart);
            firstIndex = secondIndex;
            nextOperator(command, &firstIndex, &secondIndex);
        }
        return result;
    //From here the operations are on matrices
    } else if (containCharInOrder(command, "triangularise()")) {
        Object result = recursiveCommandDecomposition(extractBetweenChar(command, '(', ')'));
        if (result.type == MATRIX) return (Object) {MATRIX, .any.matrix = triangularise(result.any.matrix)};
    } else if (containCharInOrder(command, "trans()")) {
        Object result = recursiveCommandDecomposition(extractBetweenChar(command, '(', ')'));
        if (result.type == MATRIX) return (Object) {MATRIX, .any.matrix = transpose(result.any.matrix)};
    } else if (containCharInOrder(command, "adj()")) {
        Object result = recursiveCommandDecomposition(extractBetweenChar(command, '(', ')'));
        if (result.type == MATRIX) return (Object) {MATRIX, .any.matrix = adjugate(result.any.matrix)};
    } else if (containCharInOrder(command, "inv()")) {
        Object result = recursiveCommandDecomposition(extractBetweenChar(command, '(', ')'));
        if (result.type == MATRIX) return (Object) {MATRIX, .any.matrix = inverse(result.any.matrix)};
    } else if (containCharInOrder(command, "eigVectors()")) {
        Object result = recursiveCommandDecomposition(extractBetweenChar(command, '(', ')'));
        if (result.type == MATRIX) return (Object) {MATRIX, .any.matrix = eigenVectors(result.any.matrix)};
    } else if (containCharInOrder(command, "PLambda()")) {
        Object result = recursiveCommandDecomposition(extractBetweenChar(command, '(', ')'));
        if (result.type == MATRIX) {
            char *stringForm = detOfStringMatrix(changeToPLambdaForm(toStringMatrix(result.any.matrix)));
            return (Object) {POLYNOMIAL, .any.polynomial = stringToPolynomial(stringForm, 0, length(stringForm))};
        }
    //From here the operations are on polynomials
    } else if (containCharInOrder(command, "derive()")) {
        Object result = recursiveCommandDecomposition(extractBetweenChar(command, '(', ')'));
        if (result.type == POLYNOMIAL || result.type == VARIABLE) {
            if (result.type == POLYNOMIAL) return (Object) {POLYNOMIAL, .any.polynomial = derive(result.any.polynomial)};
            else return (Object) {POLYNOMIAL, .any.polynomial = derive(variableToPolynomial(result.any.variable))};
        }
    //From here the operations results are variables
    } else if (containCharInOrder(command, "trace()")) {
        Object result = recursiveCommandDecomposition(extractBetweenChar(command, '(', ')'));
        if (result.type == MATRIX) return (Object) {VARIABLE, .any.variable = newVariable(trace(result.any.matrix))};
    } else if (containCharInOrder(command, "det()")) {
        Object result = recursiveCommandDecomposition(extractBetweenChar(command, '(', ')'));
        if (result.type == MATRIX) {
            if (result.any.matrix.columns == result.any.matrix.rows) {
                return (Object) {VARIABLE, .any.variable = newVariable(det(result.any.matrix))};
            } else return newObject;
        }
    }
    else return extractObject(command);
    return newObject;
}

void executeCommand(char *command) {
    //Apply simple command that doesn't need processing
    if (containString(command, "help")) { //Display help file
        printFileContent("help.txt", stdout); printf("\n\n");
    } else if (containString(command, "displayAll")) {
        printRegister(mainRegister);
    } else if (containString(command, "clear")) {
        freeRegisterContent(mainRegister);
        printf("The register was cleared\n");
    } else if (containCharInOrder(command, "readScript()")) {
        char *fileLink = extractBetweenChar(command, '(', ')');
        readScriptFile(fileLink);
    } else if (containCharInOrder(command, "display()")) {
        Object result = checkObject(recursiveCommandDecomposition(extractBetweenChar(command, '(', ')')));
        if (result.type == UNUSED) fprintf(stderr, "Couldn't calculate %s\n", command);
        else if (result.type == POLYNOMIAL) printPolynomial(result.any.polynomial);
        else if (result.type == MATRIX) printMatrix(result.any.matrix);
        else if (result.type == VARIABLE) printVariable(result.any.variable);
    } else if (containCharInOrder(command, "eigValues()")) { //Eigen values
        Object result = recursiveCommandDecomposition(extractBetweenChar(command, '(', ')'));
        if (result.type == MATRIX) printSolutions(eigenValues(result.any.matrix));
    } else if (containCharInOrder(command, "solve()")) { //Solve polynomial or matrix
        Object result = recursiveCommandDecomposition(extractBetweenChar(command, '(', ')'));
        if (result.type == POLYNOMIAL) printSolutions(solve(result.any.polynomial));
        else if (result.type == MATRIX) printMatrix(solveAugmentedMatrix(result.any.matrix));
    } else { //If no simple command, search for a composed one
        Object result = recursiveCommandDecomposition(command);
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
    printf("Please enter a command or help to see the possibilities\n");
    char *command = readString(stdin);

    do {
        //If no known operation were detected, nothing should happen
        executeCommand(command);
        //Re-Initialise the command string
        if (command) free(command);
        command = readString(stdin);
    } while (command && !containString(command, "exit"));
    
    return EXIT_SUCCESS;
}