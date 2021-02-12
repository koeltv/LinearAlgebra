/**
 * @file stringInteractions.h Header file of stringInteractions.c
 * @author Valentin Koeltgen
 */

#ifndef LINEARALGEBRA_STRINGINTERACTIONS_H
#define LINEARALGEBRA_STRINGINTERACTIONS_H

#include <stdlib.h>
#include <stdio.h>

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
 * First word of a string
 * This function return the first word of a string (surrounded by whitespaces)
 * @param string - The string where to extract the first word
 * @return first word
 */
char *firstWord(const char *string);

/**
 * Compare strings
 * This function compare 2 strings, it returns 1 if the first is shorter, 2 if the second is shorter and 0 if they are equal
 * @param string1 - first string for comparison
 * @param string2 - second string for comparison
 * @return result of comparison
 */
char shorterString(const char *string1, const char *string2);

/**
 * Search for string in a string
 * This function verify if a given string is contained in another, it returns 1 if it does and 0 otherwise
 * @param mainString - The string to check for the other string
 * @param toSearch - The string to search
 * @return result of search
 */
char containString(const char *mainString, const char *toSearch);

/**
 * Verify if the string contains the character in order
 * This function return 1 if the string contains the character in order and 0 if it doesn't
 * @param string - String to scan for the characters
 * @param charToSearch - Characters to search
 * @return result of the scan
 */
char containCharInOrder(const char *string, const char *charToSearch);

/**
 * Extract string between 2 characters
 * This function return the string between 2 given characters
 * @param string - The original string
 * @param first - The first character
 * @param last - The second character
 * @return extracted string
 */
char *extractBetweenChar(const char *string, char first, char last);

/**
 * Extract string between 2 indexes
 * This function return the string between 2 given indexes
 * @param string - The original string
 * @param first - The first index
 * @param last - The second index
 * @return extracted string
 */
char *extractBetweenIndexes(const char *string, int first, int last);

/**
 * Extract up to an index
 * This function return the string until an index
 * @param string - The original string
 * @param last - The end index
 * @return extracted string
 */
char *extractUpToIndex(const char *string, int last);

/**
 * Check if there exist an operator outside of parenthesis
 * @param string - String to scan
 * @return result of the scan
 */
char operatorWithoutDepth(const char *string);

/**
 * Find next operand in string
 * This functions update to indexes to surround the next operand in a string
 * @note If the end of the string is reached, both indexes will point to the end of the string ('\0')
 * @param string - String to scan
 * @param firstIndex - Index to place before next operand
 * @param secondIndex - Index to place after next operand
 */
void nextOperator(const char *string, int *firstIndex, int *secondIndex);

/**
 * Verify if the string contain a number
 * @param string - String to scan
 * @return result of the scan
 */
char containValue(const char *string);

/**
 * Verify if everything is between parenthesis
 * @note This function doesn't count whitespaces, if they are some outside of parenthesis it will not count them
 * @param string - String to scan
 * @return result of the scan
 */
char everythingIsBetweenParenthesis(const char *string);

/**
 * Read a double in a string
 * This function return a double read from a string
 * @param string - The given string
 * @param position - The position of the string where we start to read
 * @return double read
 */
double readDoubleInString(const char *string, int *position);

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// File interactions
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

/**
 * Print the content of a file in a buffer
 * This function print the content of the file at the given link in the output buffer without modifying it
 * @param link - Link of the file to read
 * @param output - Buffer where the content while be outputted
 */
void printFileContent(char *link, FILE *output);

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Matrix interactions
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * @struct StringMatrix
 * Structure representing a matrix of any size but with values in string format
 */
typedef struct {
    char *name;
    char ***values; ///Elements of the matrix contained in a 2 dimensional array
    int rows; ///Number of rows of the matrix
    int columns; ///Number of columns matrix
} StringMatrix;

/**
 * Create string matrix
 * This function create a matrix with strings as values, this is used to handle matrix with variables
 * @param nbRows - number of rows of the matrix
 * @param nbColumns - number of columns of the matrix
 * @param initialValue - values to initialise the element to
 * @return created string matrix
 */
StringMatrix *newStringMatrix(int nbRows, int nbColumns, char *initialValue);

/**
 * Free an existing string matrix
 * This function free an existing string matrix and change its pointer to NULL if it worked successfully
 * @param M - The string matrix to free
 */
void freeStringMatrix(StringMatrix **M);

/**
 * Remove a row in a string matrix
 * This function return a string matrix created by removing a row of a given string matrix
 * @param M - The original string matrix
 * @param rowIndex - The index of the row to remove
 * @return string matrix created by removing a row
 */
StringMatrix *removeSRow(StringMatrix *M, int rowIndex);

/**
 * Remove a column in a string matrix
 * This function return a string matrix created by removing a column of a given matrix
 * @param M - The original string matrix
 * @param columnIndex - The index of the column to remove
 * @return string matrix created by removing a column
 */
StringMatrix *removeSColumn(StringMatrix *M, int columnIndex);

/**
 * Implement a variable
 * This function add the variable in the diagonal to be used for P(lambda)
 * @param M - The given string matrix
 * @return string matrix with the variable X subtracted in the diagonal
 */
StringMatrix *changeToPLambdaForm(StringMatrix *M);

/**
 * Determinant in string format
 * This function output the determinant of a given matrix in string format
 * @param M - The given string matrix
 * @return determinant in string format
 */
char *detOfStringMatrix(StringMatrix *M);

#endif //LINEARALGEBRA_STRINGINTERACTIONS_H