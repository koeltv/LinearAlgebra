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

void addToRegister(Register *MainRegister, Polynomial *newPolynomial, Matrix *newMatrix){
    if (newPolynomial != NULL) {
        MainRegister->listOfPolynomials = realloc(MainRegister->listOfPolynomials, ++MainRegister->sizes[0]);
        MainRegister->listOfPolynomials[MainRegister->sizes[0]] = newPolynomial;
    }
    if (newMatrix != NULL) {
        MainRegister->listOfMatrices = realloc(MainRegister->listOfMatrices, ++MainRegister->sizes[1]);
        MainRegister->listOfMatrices[MainRegister->sizes[1] - 1] = newMatrix;
    }
}

Matrix *searchMatrix(Register *MainRegister, const char *name){
    for (int i = 0; i < MainRegister->sizes[1]; i++) {
        if (shorterString(MainRegister->listOfMatrices[i]->name, name) == 0) return MainRegister->listOfMatrices[i];
    }
    return NULL;
}

Polynomial *searchPolynomial(Register *MainRegister, const char *name){
    for (int i = 0; i < MainRegister->sizes[0]; i++) {
        if (shorterString(MainRegister->listOfPolynomials[i]->name, name) == 0) return MainRegister->listOfPolynomials[i];
    }
    return NULL;
}

/**
 * Main function
 * the main function is the first function to launch upon starting the program, it makes the link between all the components of the program
 * @return an integer confirming the success or failure of program end
 */
int main() {
//    char F[100] = "2X^2 + 4X + 1";
//    printf("F(X) = %s is of degree %d\n", F, degreeOfString(F, 0, length(F)+1));
//    Polynomial *P = stringToPolynomial(F, 0, length(F) + 1);
//    printf("Polynomial form : "); printPolynomial(P);
//    printSolutions(solve(P));

//    Matrix *chat = newMatrix(2, 2, 1);
//    chat->values[0][0] = 3; chat->values[1][1] = 3;
//    printMatrix(chat);
//    printMatrix(triangularise(chat));

//    Matrix *M = newMatrix(4, 3, 0);
//    M->values[0][0] = 4; M->values[0][1] = 3; M->values[0][2] = 1;
//    M->values[1][0] = 1; M->values[1][1] = 2; M->values[1][2] = 2;
//    M->values[3][0] = 5; M->values[3][1] = 5; M->values[3][2] = 2;
//    solveAugmentedMatrix(M);

//    Matrix *M = newMatrix(2, 2, 0);
//    M->values[0][0] = 1; M->values[0][1] = 2;
//    M->values[1][0] = 0; M->values[1][1] = 3;
//    //solveForVectors(solveAugmentedMatrix(M));
//    char *temp = detOfStringMatrix(changeToPLambdaForm(toStringMatrix(M)));
//    printPolynomial(stringToPolynomial(temp, 0, length(temp) + 1));
//    M->values[0][0] -= 1; M->values[1][1] -= 1;
//    printMatrix(solveAugmentedMatrix(M));
//    printMatrix(eigenVectors(M));

//    Matrix *M = newMatrix(3, 3, 0);
//    M->values[0][0] = 1; M->values[0][1] = 2; M->values[0][2] = 3;
//    M->values[1][0] = 0; M->values[1][1] = 0; M->values[1][2] = 0;
//    M->values[2][0] = 0; M->values[2][1] = 0; M->values[2][2] = 0;
//    printMatrix(M);
//    solveForVectors(solveAugmentedMatrix(M));
//    char *temp = detOfStringMatrix(changeToPLambdaForm(toStringMatrix(M)));
//    printPolynomial(stringToPolynomial(temp, 0, length(temp) + 1));
//    printMatrix(solveAugmentedMatrix(M));
//    printMatrix(eigenVectors(M));
//    printMatrix(eigenVectors(M));
//    printMatrix(triangularise(M));

    Register *MainRegister = newRegister();
    char *command = NULL;

    do {
        //Re-Initialise the command string
        if (command != NULL) free(command);
        printf("Please enter a command\n");
        command = readString(stdin);

        //Apply requested command
        if (containString(command, "help") == 1) { //Help file
            printf("Printing help file...\n");
        } else if (containCharInOrder(command, "=[]") == 1) { //Create matrix
            Matrix *M = readMatrixInString(command);
            printMatrix(M);
            addToRegister(MainRegister, NULL, M);
            printf("New matrix added\n");
        } else if (containCharInOrder(command, "=X") == 1) { //Create polynomial
            Polynomial *P = stringToPolynomial(command, 0, length(command) + 1); //TODO Implement names for polynomials
            addToRegister(MainRegister, P, NULL);
        } else if (containCharInOrder(command, "eig()") == 1) { //Eigen values
            char *name = extractBetweenChar(command, '(', ')');
            Matrix *toUse = searchMatrix(MainRegister, name);
            if (toUse != NULL) printSolutions(eigenValues(toUse));
            else printf("The matrix %s doesn't exist\n", name);
        }
    } while (containString(command, "exit") == 0);
    return EXIT_SUCCESS;
}