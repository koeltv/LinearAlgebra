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
    //char F[100] = "(1 + 2X) + ((1X^2) * (1X^2) * (1X))";
    //printf("%s is of degree %d\n", F, degreeOfString(F, 0, length(F)+1));
    //printf("Polynomial form : ");
    //printPolynomial(stringToPolynomial(F, 0, length(F) + 1));

    Matrix *chat = newMatrix(2, 2, 1);
    chat->values[0][0] = 3; chat->values[1][1] = 3;
    //eigenValues(chat);
    printMatrix(eigenVectors(chat));
    printMatrix(triangularise(chat));
//    char *result = detOfStringMatrix(changeToPLambdaForm(toStringMatrix(chat)));
//    printf("Result : %s\n", result);
//    solve(stringToPolynomial(result, 0, length(result) + 1));

    //printf("%s\n", detOfStringMatrix(toStringMatrix(newMatrix(2, 2, 1))));
    char *command = NULL;
    do {
        if (command != NULL) free(command);
        printf("Please enter a command\n");
        command = readString(stdin);
        if (containString(command, "help") == 1) {
            printf("Printing help file...\n");
            //return help file
        } else if (containString(command, "=[]")) { //TODO Create contain character in good order
            //create matrix
        }
    } while (containString(command, "exit") == 0);
    return EXIT_SUCCESS;
}