/**
 * @file register.h Header file of register.c
 * @author Valentin Koeltgen
 */

#ifndef LINEARALGEBRA_REGISTER_H
#define LINEARALGEBRA_REGISTER_H

#include "matrix.h"

#define newRegister {{0, 0, 0}, NULL, NULL, NULL} ///New empty register
#define newObject (Object) {-1} ///New empty object

#define UNUSED -1 ///Index used to initialise objects and say that no object were returned
#define POLYNOMIAL 0 ///Index for polynomials
#define MATRIX 1 ///Index for matrices
#define VARIABLE 2 ///Index for variables

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Structures
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * @struct Register
 * Structure representing a container of objects used in this program
 */
typedef struct {
    int sizes[3]; ///Sizes of the list of objects
    Polynomial *listOfPolynomials; ///List of polynomials
    Matrix *listOfMatrices; ///List of matrices
    Variable *listOfVariables; ///List of variables
} Register;

/**
 * @union Any
 * Structure that can contain any object
 */
typedef union {
    Polynomial polynomial;
    Matrix matrix;
    Variable variable;
} Any;

/**
 * @struct Object
 * Structure representing any type of object
 */
typedef struct {
    char type; ///Current used type of object
    Any any; ///Object used
} Object;

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Construction functions
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
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
 * Verify if an object respects its format
 * This function verifies if an object respect its format and return a null object if it doesn't
 * @param input - Object to check
 * @return Result of checking
 */
Object checkObject(Object input);

/**
 * Search for an object in a register
 * This function returns a corresponding object if it exist in the register and it's name matches. It returns NULL otherwise
 * @param aRegister - The register to analyze
 * @param name - The name of the object to search
 * @return found object
 */
Object searchObject(Register *aRegister, const char *name);

/**
 * Delete from a register
 * This function delete an object from an existing register
 * @param aRegister - The register to scan
 * @param toDelete - The object to delete
 */
void deleteFromRegister(Register *aRegister, Object toDelete);

/**
 * Add to a register
 * This function add an object to an existing register
 * @note When we only need to add 1 and not the other, we can put NULL
 * @param aRegister - The destination register
 * @param toAdd - The objects to add
 */
void addToRegister(Register *aRegister, Object toAdd);

/**
 * Print the content of a register
 * This function prints the whole content of a register
 * @note If the register is empty, this function will print "The register is empty"
 * @param aRegister - The register to print
 */
void printRegister(Register *aRegister);

#endif //LINEARALGEBRA_REGISTER_H