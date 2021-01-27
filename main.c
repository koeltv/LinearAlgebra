/**
 * @file main.c Main file of the program
 * @author Valentin Koeltgen
 *
 * This is the main file of the program, it takes care of the interactions with the user
 */

#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
//#include "polynomial.h"

/**
 * Main function
 * the main function is the first function to launch upon starting the program, it makes the link between all the components of the program
 * @return an integer confirming the success or failure of program end
 */
int main() {
    Matrix *matrix = readMatrixInFile();
    printMatrix(matrix);
    //printf("det(M) = %.2lf\n", det(matrix));
    //Matrix *trans = solveAugmentedMatrix(matrix);
    //printMatrix(trans);
    //printf("det(M^T) = %.2lf\n", det(trans));
    //freeMatrix(matrix); freeMatrix(trans);
    Polynomial *f = stringToPolynomial("2X^2 - 5");
    printPolynomial(f);
    double *roots = solve(f);
    for (int i = 0; i < f->highestDegree; i++) printf("x%d = %1.2lf, ", i, roots[i]); printf("\n");

    int size = 2;
    Matrix *test = newMatrix(size, size, 2);

    int k = 1;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) test->values[i][j] = k++;
    }
    printMatrix(test);
    //eigenValues(test);
    //printPolynomial(f);
    //printPolynomial(syntheticDivision(f, 1));

    printf("Associated polynomial :\n");
    printf("%s\n", detToString(detPForm(toStringMatrix(test))));
    return EXIT_SUCCESS;
}