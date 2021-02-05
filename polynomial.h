/**
 * @file polynomial.h Header file of polynomial.c
 * @author Valentin Koeltgen
 */

#ifndef LINEARALGEBRA_POLYNOMIAL_H
#define LINEARALGEBRA_POLYNOMIAL_H

#include "stringInteractions.h"

#define IMAGINARY 12345.54321

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Structures
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * @struct Polynomial
 * Structure representing a polynomial of any degree
 */
typedef struct {
    char *name; ///Name of the polynomial
    double *coefficient; ///Coefficients of the polynomial
    int highestDegree; ///Highest degree of the polynomial
} Polynomial;

/**
 * @struct Solutions
 * Structure representing solutions linked to an equation
 */
typedef struct {
    double *values; ///Values contained in array
    int size; ///Number of values contained
} Solutions;

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Construction functions
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * Convert a string to a polynomial
 * This function return a polynomial created from a string
 * @note there can be more than one coefficient for the same power of X
 * @attention The coefficient must follow the format "<value>X^<power>" for <power> = 1, we can write "<value>X" and for <power> = 0 we can write "<value>"
 * @param string - The given string
 * @return polynomial created from the string
 */
Polynomial *stringToPolynomial(const char *string, int start, int end);

/**
 * Free an existing polynomial
 * This function free an existing polynomial and change its pointer to NULL if it worked successfully
 * @param F - The polynomial to free
 */
void freePolynomial(Polynomial *F);

/**
 * Copy a polynomial
 * This function return a copy of a given polynomial
 * @param F - The polynomial to copy
 * @return copy created
 */
Polynomial *copyPolynomial(Polynomial *F);

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Basic operator functions
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
int degreeOfString(const char *string, int start, int end);

Polynomial *pAdd(Polynomial *F, Polynomial *G);

Polynomial *pMinus(Polynomial *F, Polynomial *G);

Polynomial *pMultiply(Polynomial *F, Polynomial *G);

/**
 * Apply the polynomial for a given value
 * This function apply the given polynomial with the given value
 * @param F - the given polynomial
 * @param x - the value to use
 * @return F(x)
 */
double apply(Polynomial *F, double x);

/**
 * Derive a polynomial
 * This function return the derivative of a given polynomial
 * @warning the degree of the polynomial must be superior to 0
 * @param F - the polynomial to derive
 * @return F'
 */
Polynomial *derive(Polynomial *F);

/**
 * Print a polynomial
 * Print a given polynomial in the terminal
 * @param F - The given polynomial
 */
void printPolynomial(Polynomial *F);

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Advanced operator functions
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * Synthetic division of a polynomial
 * This function return a polynomial with 1 degree less than the given polynomial by dividing it by "X - <root>"
 * @param F - polynomial to divide
 * @param root - root by which to divide
 * @return divided polynomial
 */
Polynomial *syntheticDivision(Polynomial *F, double root);

/**
 * Application of the Newton Method
 * This function apply the Newton Method to find a root of the given polynomial
 * @param F - The given polynomial
 * @return approximation of a root of the given polynomial
 */
double newtonMethod(Polynomial *F);

/**
 * Find the roots of a polynomial
 * This function return the roots of a given polynomial of any degree
 * @warning the roots must be reals
 * @param F - The polynomial to solve
 * @return roots of the polynomial
 */
Solutions *solve(Polynomial *F);

void printSolutions(Solutions *x);

#endif //LINEARALGEBRA_POLYNOMIAL_H