/**
 * @file main.h Header file of register.c
 * @author Valentin Koeltgen
 */

#ifndef LINEARALGEBRA_REGISTER_H
#define LINEARALGEBRA_REGISTER_H

#include "matrix.h"

/**
 * @struct Register
 * Structure representing a global container of objects used in this program
 */
typedef struct {
    int sizes[2]; ///Sizes of the list of objects
    Polynomial **listOfPolynomials; ///List of polynomials
    Matrix **listOfMatrices; ///List of matrices
} Register;

/**
 * Create a new register
 * @return created register
 */
Register *newRegister();

/**
 * Search for a matrix in a register
 * This function returns a corresponding matrix if it exist in the register and it's name matches or NULL otherwise
 * @param aRegister - The register to analyze
 * @param name - The name of the matrix to search
 * @return found matrix
 */
Matrix *searchMatrix(Register *aRegister, const char *name);

/**
 * Search for a polynomial in a register
 * This function returns a corresponding polynomial if it exist in the register and it's name matches or NULL otherwise
 * @param aRegister - The register to analyze
 * @param name - The name of the polynomial to search
 * @return found polynomial
 */
Polynomial *searchPolynomial(Register *aRegister, const char *name);

/**
 * Add to a register
 * This function add an object to an existing register
 * @note When we only need to add 1 and not the other, we can put NULL
 * @param aRegister - The destination register
 * @param newPolynomial - The polynomial to add
 * @param newMatrix - The matrix to add
 */
void addToRegister(Register *aRegister, Polynomial *newPolynomial, Matrix *newMatrix);

/**
 * Empty a register
 * This function free the content of a register but doesn't free it
 * @param aRegister - The register to empty
 */
void freeRegisterContent(Register *aRegister);

/**
 * Print the content of a register
 * This function prints the whole content of a register
 * @note If the register is empty, this function will print "The register is empty"
 * @param aRegister - The register to print
 */
void printRegister(Register *aRegister);

#endif //LINEARALGEBRA_REGISTER_H