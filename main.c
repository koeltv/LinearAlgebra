/**
 * @file main.c Main file of the program
 * @author Valentin Koeltgen
 *
 * This is the main file of the program, it takes care of the interactions with the user
 */

#include "matrix.h"

/**
 * Main function
 * the main function is the first function to launch upon starting the program, it makes the link between all the components of the program
 * @return an integer confirming the success or failure of program end
 */
int main() {
//    char F[100] = "(3 - 1X) * (3 - 1X) - 1";
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

    Matrix *M = newMatrix(2, 2, 0);
    M->values[0][0] = 1; M->values[0][1] = 2;
    M->values[1][0] = 0; M->values[1][1] = 3;
    //solveForVectors(solveAugmentedMatrix(M));
    char *temp = detOfStringMatrix(changeToPLambdaForm(toStringMatrix(M)));
    printPolynomial(stringToPolynomial(temp, 0, length(temp) + 1));
    printMatrix(eigenVectors(M));



//    char *command = NULL;
//    do {
//        if (command != NULL) free(command);
//        printf("Please enter a command\n");
//        command = readString(stdin);
//        if (containString(command, "help") == 1) {
//            printf("Printing help file...\n");
//            //return help file
//        } else if (containString(command, "=[]")) { //TODO Create contain character in good order
//            //create matrix
//        }
//    } while (containString(command, "exit") == 0);
    return EXIT_SUCCESS;
}