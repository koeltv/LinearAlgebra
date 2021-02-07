/**
 * @file register.h Header file of register.c
 * @author Valentin Koeltgen
 */

#ifndef LINEARALGEBRA_REGISTER_H
#define LINEARALGEBRA_REGISTER_H

#include "matrix.h"
#include "variable.h"

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Structures
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * @struct Register
 * Structure representing a container of objects used in this program
 */
typedef struct {
    int sizes[3]; ///Sizes of the list of objects
    Polynomial **listOfPolynomials; ///List of polynomials
    Matrix **listOfMatrices; ///List of matrices
    Variable **listOfVariables; ///List of variables
} Register;

/**
 * @struct Object
 * Structure representing any type of object
 */
typedef struct {
    Polynomial *polynomial;
    Matrix *matrix;
    Variable *variable;
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
 * Free a register
 * @param toFree - Register to free
 */
void freeRegister(Register **toFree);

/**
 * Empty a register
 * This function free the content of a register but doesn't free it
 * @param aRegister - The register to empty
 */
void freeRegisterContent(Register *aRegister);

/**
 * Create a new object
 * @return created object
 */
Object *newObject();

/**
 * Free an object
 * @param toFree - The object to free
 */
void freeObject(Object *toFree);

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Basic operator functions
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * Search for an object in a register
 * This function returns a corresponding object if it exist in the register and it's name matches. It returns NULL otherwise
 * @param aRegister - The register to analyze
 * @param name - The name of the object to search
 * @return found object
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
 * @param newObject - The objects to add
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