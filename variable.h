/**
 * @file variable.h Header file of variable.c
 * @author Valentin Koeltgen
 */

#ifndef LINEARALGEBRA_VARIABLE_H
#define LINEARALGEBRA_VARIABLE_H

#include <stdlib.h>
#include <stdio.h>

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
Variable newVariable(double value);

/**
 * Free a variable
 * @param toFree - The variable to free
 */
void freeVariable(Variable *toFree);

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Basic operator functions
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * Print a variable
 * Print a given variable in the terminal
 * @param variable - Variable to print
 */
void printVariable(Variable variable);

#endif //LINEARALGEBRA_VARIABLE_H