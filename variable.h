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

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Basic mathematical operators
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * Absolute value
 * @param x - Input value
 * @return |x|
 */
double absolute(double x);

/**
 * Square root
 * @param x - Input value
 * @return Square root of x
 */
double sqrt(double x);

/**
 * Round double to integer
 * @param value - Value to round
 * @return rounded value
 */
int roundDouble(double value);

/**
 * Reduce precision of a double
 * This function take a double and round it up to a lower precision (1e-9)
 * @param value - Value to round
 * @return rounded double
 */
double roundPreciseDouble(double value);

#endif //LINEARALGEBRA_VARIABLE_H