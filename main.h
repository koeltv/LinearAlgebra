/**
 * @file main.h Header file of main.c
 * @author Valentin Koeltgen
 */

#ifndef LINEARALGEBRA_MAIN_H
#define LINEARALGEBRA_MAIN_H

#include "register.h"

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Commands interactions
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * Read a matrix in a string
 * This function read a matrix in a string given a specific format and return a matrix structure made from it
 * @param string - String to scan for a matrix
 * @return matrix formed from the string
 */
Matrix readMatrixInString(const char *string);

/**
 * Eigen values of a matrix
 * This function return the eigen values of a given matrix
 * @param M  - the given matrix
 * @return eigen values of M
 */
Solutions *eigenValues(Matrix M);

/**
 * Eigen vectors of a matrix
 * This function return the eigen vectors of a given matrix in a matrix form
 * @param M - the given matrix
 * @return eigen vectors in matrix form
 */
Matrix eigenVectors(Matrix M);

/**
 * Triangularise a matrix
 * This function triangularise (or diagonalise if possible) a given matrix
 * @param M - the given matrix
 * @return triangularised M
 */
Matrix triangularise(Matrix M);

/**
 * Read and apply a script file
 * This function read a given file and apply the commands in it line by line
 * @param link - link of the file in string format
 */
void readScriptFile(const char *link);

/**
 * Apply a mathematical operation
 * This function apply a simple mathematical operation (sum, difference, multiplication, division) on 2 objects (polynomial, matrix or variable)
 * @param leftOperand - Left part of the operation
 * @param rightOperand - Right part of the operation
 * @param operator - The operation to apply
 * @return object containing the result
 */
Object applyOperation(Object leftOperand, char operator, Object rightOperand);

/**
 * Get a basic object from a string
 * This function recuperate a basic object (existing or created)
 * @param command - object in string format
 * @return object containing the object
 */
Object extractObject(const char *command);

/**
 * Apply a composite command recursively
 * This function apply a command by reducing to basic object and combining them
 * @param command - command in string form
 * @return object containing the result of the command
 */
Object recursiveCommandDecomposition(const char *command);

/**
 * Apply any command
 * This function is the first called when reading a command, it can take simple and composite commands, see readme.md for more details
 * @param command - The command to execute
 */
void executeCommand(const char *command);

#endif //LINEARALGEBRA_MAIN_H