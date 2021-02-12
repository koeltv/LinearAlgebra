/**
 * @file variable.h Header file of variable.c
 * @author Valentin Koeltgen
 */

#ifndef LINEARALGEBRA_VARIABLE_H
#define LINEARALGEBRA_VARIABLE_H

#include "polynomial.h"

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Structures
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * @struct Variable
 * Structure representing a variable
 */
typedef struct {
    char *name; ///Name of the variable
    double value; ///Value of the variable
} Variable;

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Construction functions
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * Create a variable with the given value
 * @param value - The value to initialise the variable
 * @return created variable
 */
Variable *newVariable(double value);

/**
 * Free a variable
 * @param toFree - The variable to free
 */
void freeVariable(Variable **toFree);

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Basic operator functions
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * Print a variable
 * Print a given variable in the terminal
 * @param variable - Variable to print
 */
void printVariable(Variable *variable);

/**
 * Copy a variable
 * This function returns a copy of the given variable
 * @param variable - The variable to copy
 * @return copy of the variable
 */
Variable *copyVariable(Variable *variable);

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Polynomial interactions
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * Transform a variable into a polynomial
 * This function returns a polynomial of degree 0 with the variable as coefficient, it is used to make interactions between variables and polynomial easier
 * @param variable - The variable to convert
 * @return corresponding polynomial
 */
Polynomial *variableToPolynomial(Variable *variable);

#endif //LINEARALGEBRA_VARIABLE_H