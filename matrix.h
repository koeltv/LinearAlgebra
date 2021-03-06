/**
 * @file matrix.h Header file of matrix.c
 * @author Valentin Koeltgen
 */

#ifndef LINEARALGEBRA_MATRIX_H
#define LINEARALGEBRA_MATRIX_H

#include "polynomial.h"

#define nullMatrix (Matrix) {NULL, NULL, 0, 0} ///New null matrix

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Structures
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * @struct Matrix
 * Structure representing a matrix of any size
 */
typedef struct {
    char *name;
    double **values; ///Elements of the matrix contained in a 2 dimensional array
    int rows; ///Number of rows of the matrix
    int columns; ///Number of columns matrix
} Matrix;

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Construction functions
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * Create a simple matrix
 * This function create a simple matrix with the given number of columns and rows
 * @param nbRows - number of rows of the matrix to create
 * @param nbColumns - number of columns of the matrix to create
 * @return New matrix
 */
Matrix newMatrix(int nbRows, int nbColumns);

/**
 * Free an existing matrix
 * This function free an existing matrix and change its pointer to NULL if it worked successfully
 * @param M - The matrix to free
 */
void freeMatrix(Matrix *M);

/**
 * Remove a row in a matrix
 * This function return a matrix created by removing a row of a given matrix
 * @param M - The original matrix
 * @param rowIndex - The index of the row to remove
 * @return matrix created by removing a row
 */
Matrix removeRow(Matrix M, int rowIndex);

/**
 * Remove a column in a matrix
 * This function return a matrix created by removing a column of a given matrix
 * @param M - The original matrix
 * @param columnIndex - The index of the column to remove
 * @return matrix created by removing a column
 */
Matrix removeColumn(Matrix M, int columnIndex);

/**
 * Add a column to a matrix
 * This function return a matrix created by adding a column in the last position of a given matrix
 * @param M - The original matrix
 * @return matrix created by adding a column
 */
Matrix addColumn(Matrix M);

/**
 * Create a subMatrix
 * This function create a subMatrix of a given matrix
 * @param M - The original matrix
 * @param r1 - The index of the first row to take
 * @param r2 - The index of the last row to take
 * @param c1 - The index of the first column to take
 * @param c2 - The index of the last column to take
 * @return subMatrix created
 */
Matrix subMat(Matrix M, int r1, int r2, int c1, int c2);

/**
 * Transform a matrix to a string matrix
 * This function take a given matrix and create a matrix with value in string format to enable the use of variables
 * @param M
 * @return
 */
StringMatrix toStringMatrix(Matrix M);

/**
 * Copy a matrix
 * This function returns a copy of a given matrix
 * @param M - The matrix to copy
 * @return The copy of the given matrix
 */
Matrix copyMatrix(Matrix M);

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Basic operator functions
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * Sum 2 matrices
 * This functions sums 2 matrices
 * @warning They must be of identical dimensions
 * @param A - first matrix
 * @param B - second matrix
 * @return sum of the matrices
 */
Matrix sum(Matrix A, Matrix B);

/**
 * Subtract 2 matrices
 * This functions subtracts 2 matrices
 * @warning They must be of identical dimensions
 * @param A - first matrix
 * @param B - second matrix
 * @return subtraction of the matrices
 */
Matrix minus(Matrix A, Matrix B);

/**
 * Multiply a matrix by a scalar
 * This function multiply a matrix by a real scalar
 * @param M - matrix
 * @param scalar - real scalar
 * @return matrix multiplied by the scalar
 */
Matrix scalarMultiply(Matrix M, double scalar);

/**
 * Standard matrix multiplication
 * This function does a standard multiplication of 2 matrices
 * @warning The number of columns of the first matrix must equal the number of rows of the second
 * @param A - the first matrix
 * @param B - the second matrix
 * @return multiplication of the matrices
 */
Matrix multiply(Matrix A, Matrix B);

/**
 * Transpose of a matrix
 * This function return the transpose of a given matrix
 * @param M - The original matrix
 * @return M^T
 */
Matrix transpose(Matrix M);

/**
 * print matrix
 * This function print a given matrix in the terminal
 * @param M - the given matrix
 */
void printMatrix(Matrix M);

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Advanced operator functions
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * Trace of a matrix
 * This function return the trace of a given matrix
 * @param M - the given matrix
 * @return trace(M)
 */
double trace(Matrix M);

/**
 * Determinant of a matrix
 * This function return the determinant of a given matrix
 * @param M - the given matrix
 * @return det(M)
 */
double det(Matrix M);

/**
 * Adjugate of a matrix
 * This function return the adjugate of a given matrix
 * @param M - the given matrix
 * @return adj(M)
 */
Matrix adjugate(Matrix M);

/**
 * Inverse of a matrix
 * This function return the inverse M of a given M if it exists
 * @warning If the given matrix is not reversible, the function return NULL
 * @param M - the given matrix
 * @return M^-1
 */
Matrix inverse(Matrix M);

/**
 * Resolution of an augmented matrix
 * This function return the given augmented matrix in a solvable format
 * @param M - the given augmented matrix
 * @return Solvable augmented matrix
 */
Matrix solveAugmentedMatrix(Matrix M);

/**
 * Find eigen vectors of a given matrix
 * This function returns a basis of eigen vectors of a given matrix in matrix form
 * @param M - The given matrix
 * @return Basis of eigen vectors
 */
Matrix solveForVectors(Matrix M);

/**
 * Complete a basis of vector
 * This function complete a matrix containing vectors so that all vectors are orthogonal
 * @param M - The basis of vector (in matrix form)
 * @return Completed basis
 */
Matrix completeOrthogonal(Matrix M);

#endif //LINEARALGEBRA_MATRIX_H