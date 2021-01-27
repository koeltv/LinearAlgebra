#ifndef LINEARALGEBRA_STRINGINTERACTIONS_H
#define LINEARALGEBRA_STRINGINTERACTIONS_H

#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Structures
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * @struct StringMatrix
 * Structure representing a matrix of any size but with values in string format
 */
typedef struct {
    char ***values; ///Elements of the matrix contained in a 2 dimensional array
    int rows; ///Number of rows of the matrix
    int columns; ///Number of columns matrix
} StringMatrix;

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Construction functions
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

/**
 * Read a string in a file stream
 * This function read a string of any size in a given file stream. It will get all visible characters in the ASCII table
 * @param current - The file stream where the string will be read
 * @return string formed from the file stream
 */
char *readString(FILE *current);

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Basic operator functions
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

/**
 * String length
 * This function output the length of a string
 * @note the \0 is not included in the count
 * @param string
 * @return
 */
int length(const char *string);

/**
 * Read a double in a string
 * This function return a double read from a string
 * @param string - The given string
 * @param position - The position of the string where we start to read
 * @return double read
 */
double readDoubleInString(const char *string, int *position);

/**
 * Read a double in a file stream
 * The function readDoubleInFile read a double of any size in a filestream, update the pointer and returns the value
 * @param currentFile - The file stream where the double will be read
 * @param temp - the pointer
 * @return double read
 */
double readDoubleInFile(FILE *currentFile, char *temp);

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Matrix interactions
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

/**
 * (WIP) Read a matrix in a file
 * This function read a matrix in a file given a specific format and return a Matrix structure made from it
 * @return matrix formed from the file
 */
Matrix *readMatrixWIP();

/**
 * Current read a matrix in file
 * This function read a matrix in a file given a specific format and return a Matrix structure made from it
 * @return matrix formed from the file
 */
Matrix *readMatrixInFile();

/**
 * Transform a matrix to a string matrix
 * This function take a given matrix and create a matrix with value in string format to enable the use of variables
 * @param M
 * @return
 */
StringMatrix *toStringMatrix(Matrix *M);

/**
 * Implement a variable
 * This function add the variable in the diagonal to be used for P(lambda)
 * @param M
 * @return
 */
StringMatrix *detPForm(StringMatrix *M);

/**
 * Determinant in string format
 * This function output the determinant of a given matrix in string format
 * @param M
 * @return
 */
char *detToString(StringMatrix *M);

#endif //LINEARALGEBRA_STRINGINTERACTIONS_H