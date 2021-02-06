/**
 * @file register.h Header file of register.c
 * @author Valentin Koeltgen
 */

#ifndef LINEARALGEBRA_REGISTER_H
#define LINEARALGEBRA_REGISTER_H

#include "matrix.h"

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Structures
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * @struct Register
 * Structure representing a container of objects used in this program
 */
typedef struct {
    int sizes[2]; ///Sizes of the list of objects
    Polynomial **listOfPolynomials; ///List of polynomials
    Matrix **listOfMatrices; ///List of matrices
} Register;

/**
 * @struct Object
 * Structure representing any type of object
 */
typedef struct {
    Polynomial *polynomial;
    Matrix *matrix;
} Object;

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Construction functions
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * Create a new register
 * @return created register
 */
Register *newRegister();

/**
 * Create a new object
 * @return created object
 */
Object *newObject();

/**
 * Empty a register
 * This function free the content of a register but doesn't free it
 * @param aRegister - The register to empty
 */
void freeRegisterContent(Register *aRegister);

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Basic operator functions
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * Search for an object in a register
 * This function returns a corresponding object if it exist in the register and it's name matches. It returns NULL otherwise
 * @param aRegister - The register to analyze
 * @param name - The name of the object to search
 * @return found polynomial
 */
Object *searchObject(Register *aRegister, const char *name);

/**
 * Delete from a register
 * This function delete an object from an existing register
 * @param aRegister - The register to scan
 * @param toDelete - The object to delete
 */
void deleteFromRegister(Register *aRegister, Object *toDelete);

/**
 * Add to a register
 * This function add an object to an existing register
 * @note When we only need to add 1 and not the other, we can put NULL
 * @param aRegister - The destination register
 * @param newPolynomial - The polynomial to add
 * @param newMatrix - The matrix to add
 */
void addToRegister(Register *aRegister, Object *newObject);

/**
 * Print the content of a register
 * This function prints the whole content of a register
 * @note If the register is empty, this function will print "The register is empty"
 * @param aRegister - The register to print
 */
void printRegister(Register *aRegister);

#endif //LINEARALGEBRA_REGISTER_H