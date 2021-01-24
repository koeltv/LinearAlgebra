/**
 * @file matrix.h Header file of matrix.c
 * @author Valentin Koeltgen
 */

#ifndef LINEARALGEBRA_MATRIX_H
#define LINEARALGEBRA_MATRIX_H

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Structures
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * @struct Matrix
 * Structure representing a matrix of any size
 */
typedef struct {
    double **values; ///Elements of the matrix
    int rows; ///Number of rows
    int columns; ///Number of columns
} Matrix;

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Construction functions
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * Read a double in a file stream
 * The function readDoubleInFile read a double of any size in a filestream, update the pointer and returns the value
 * @param currentFile - The file stream where the double will be read
 * @param temp - the pointer
 * @return double read
 */
double readDoubleInFile(FILE *currentFile, char *temp);

/**
 * (WIP) Read a matrix in a file
 * This function read a matrix in a file given a specific format and return a Matrix structure made from it
 * @return matrix formed from the file
 */
Matrix *readMatrixWIP();

/**
 * Read a string in a file stream
 * This function read a string of any size in a given file stream. It will get all visible characters in the ASCII table
 * @param current - The file stream where the string will be read
 * @return string formed from the file stream
 */
char *readString(FILE *current);

/**
 * Current read a matrix in file
 * This function read a matrix in a file given a specific format and return a Matrix structure made from it
 * @return matrix formed from the file
 */
Matrix *readMatrixInFile();

/**
 * Create a simple matrix
 * This function create a simple matrix with the given number of columns and rows and with it's values initialized with the given value
 * @param nbRows - number of rows of the matrix to create
 * @param nbColumns - number of columns of the matrix to create
 * @param initialValue - initial value for each element of the matrix
 * @return initialized matrix
 */
Matrix *newMatrix(int nbRows, int nbColumns, double initialValue);

/**
 * Free an existing matrix
 * This function free an existing matrix and change its pointer to NULL if it worked successfully
 * @param matrix - The matrix to free
 */
void freeMatrix(Matrix *matrix);

/**
 * Remove a row in a matrix
 * This function return a matrix created by removing a row of a given matrix
 * @param matrix - The original matrix
 * @param rowIndex - The index of the row to remove
 * @return matrix created by removing a row
 */
Matrix *removeRow(Matrix *matrix, int rowIndex);

/**
 * Remove a column in a matrix
 * This function return a matrix created by removing a column of a given matrix
 * @param matrix - The original matrix
 * @param columnIndex - The index of the column to remove
 * @return matrix created by removing a column
 */
Matrix *removeColumn(Matrix *matrix, int columnIndex);

/**
 * Add a column to a matrix
 * This function return a matrix created by adding a column in the last position of a given matrix
 * @param matrix - The original matrix
 * @return matrix created by adding a column
 */
Matrix *addColumn(Matrix *matrix);

/**
 * Create a subMatrix
 * This function create a subMatrix of a given matrix
 * @param matrix - The original matrix
 * @param r1 - The index of the first row to take
 * @param r2 - The index of the last row to take
 * @param c1 - The index of the first column to take
 * @param c2 - The index of the last column to take
 * @return subMatrix created
 */
Matrix *subMat(Matrix* matrix, int r1, int r2, int c1, int c2);

/**
 * Copy a matrix
 * This function creates an exact copy of a given matrix
 * @param matrix - The matrix to copy
 * @return copy created
 */
Matrix* copy(Matrix *matrix);

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
Matrix* sum(Matrix *A, Matrix *B);

/**
 * Subtract 2 matrices
 * This functions subtracts 2 matrices
 * @warning They must be of identical dimensions
 * @param A - first matrix
 * @param B - second matrix
 * @return subtraction of the matrices
 */
Matrix* minus(Matrix *A, Matrix *B);

/**
 * Inner multiplication of 2 matrices
 * This function multiply the elements of 2 matrices
 * @note This operation is different from a standard matrix multiplication, it only multiplies element by element
 * @warning They must be of identical dimensions
 * @param A - first matrix
 * @param B - second matrix
 * @return inner multiplication of the matrices
 */
Matrix* innerMultiply(Matrix *A, Matrix *B);

/**
 * Multiply a matrix by a scalar
 * This function multiply a matrix by a real scalar
 * @param A - matrix
 * @param scalar - real scalar
 * @return matrix multiplied by the scalar
 */
Matrix* scalarMultiply(Matrix* A, double scalar);

/**
 * Standard matrix multiplication
 * This function does a standard multiplication of 2 matrices
 * @warning The number of columns of the first matrix must equal the number of rows of the second
 * @param A - the first matrix
 * @param B - the second matrix
 * @return multiplication of the matrices
 */
Matrix* multiply(Matrix *A, Matrix *B);

/**
 * Transpose of a matrix
 * This function return the transpose of a given matrix
 * @param matrix - The original matrix
 * @return matrix^T
 */
Matrix *transpose(Matrix *matrix);

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Advanced operator functions
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * Trace of a matrix
 * This function return the trace of a given matrix
 * @param matrix - the given matrix
 * @return trace(matrix)
 */
double trace(Matrix *matrix);

/**
 * Determinant of a matrix
 * This function return the determinant of a given matrix
 * @param M - the given matrix
 * @return det(M)
 */
double det(Matrix *M);

/**
 * Adjugate of a matrix
 * This function return the adjugate of a given matrix
 * @param matrix - the given matrix
 * @return adj(M)
 */
Matrix *adjugate(Matrix *matrix);

/**
 * Inverse of a matrix
 * This function return the inverse matrix of a given matrix if it exists
 * @warning If the given matrix is not reversible, the function return NULL
 * @param matrix - the given matrix
 * @return matrix^-1
 */
Matrix *inverse(Matrix *matrix);

/**
 * (WIP) Resolution of an augmented matrix
 * This function return the given augmented matrix in a solvable format
 * @param M - the given augmented matrix
 * @return solvable augmented matrix
 */
Matrix *solveAugmentedMatrix(Matrix *M);

/**
 * Triangularise a matrix
 * This function triangularise (or diagonalise) a given matrix
 * @param matrix - the given matrix
 * @return triangularised matrix
 */
Matrix *triangularise(Matrix *matrix);

/**
 * print matrix
 * This function print a given matrix in the terminal
 * @param matrix - the given matrix
 */
void printMatrix(Matrix *matrix);

#endif //LINEARALGEBRA_MATRIX_H