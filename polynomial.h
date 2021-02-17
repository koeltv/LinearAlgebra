/**
 * @file polynomial.h Header file of polynomial.c
 * @author Valentin Koeltgen
 */

#ifndef LINEARALGEBRA_POLYNOMIAL_H
#define LINEARALGEBRA_POLYNOMIAL_H

#include "stringInteractions.h"
#include "variable.h"

#define highestCoefficient(F) F.coefficient[F.highestDegree]

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
    int size; ///Number of values contained
    double *values; ///Values contained in array
} Solutions;

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Construction functions
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * Create an empty polynomial of the given degree
 * @param degree - Degree of the polynomial to create
 * @return created polynomial
 */
Polynomial newPolynomial(int degree);

/**
 * Convert a string to a polynomial
 * This function return a polynomial created from a string
 * @note there can be more than one coefficient for the same power of X
 * @attention The coefficient must follow the format "<value>X^<power>" for <power> = 1, we can write "<value>X" and for <power> = 0 we can write "<value>"
 * @param string - The given string
 * @return polynomial created from the string
 */
Polynomial stringToPolynomial(const char *string, int start, int end);

/**
 * Free an existing polynomial
 * This function free an existing polynomial and change its pointer to NULL if it worked successfully
 * @param F - The polynomial to free
 */
void freePolynomial(Polynomial *F);

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Basic operator functions
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * Degree of a polynomial in string format
 * This function return the degree of a polynomial in string format
 * @param string - The polynomial in string format
 * @param start - The starting index of the string
 * @param end - The end index of the string
 * @return degree of the polynomial
 */
int degreeOfString(const char *string, int start, int end);

/**
 * Sum polynomials
 * This function return the sum of 2 polynomials
 * @param F - first polynomial
 * @param G - second polynomial
 * @return F + G
 */
Polynomial pAdd(Polynomial F, Polynomial G);

/**
 * Subtract polynomials
 * This function return the difference of the first polynomial by the second
 * @param F - first polynomial
 * @param G - second polynomial
 * @return F - G
 */
Polynomial pMinus(Polynomial F, Polynomial G);

/**
 * Multiply polynomials
 * This function return the product of 2 polynomials
 * @param F - first polynomial
 * @param G - second polynomial
 * @return F * G
 */
Polynomial pMultiply(Polynomial F, Polynomial G);

/**
 * Divide polynomials
 * This function return the division of 2 polynomials using the long division method
 * @note If there is a remainder, it will be printed in the terminal but it won't be integrated in the result
 * @param F - first polynomial
 * @param G - second polynomial
 * @return F / G
 */
Polynomial pLongDivide(Polynomial numerator, Polynomial denominator);

/**
 * Apply the polynomial for a given value
 * This function apply the given polynomial with the given value
 * @param F - the given polynomial
 * @param x - the value to use
 * @return F(x)
 */
double apply(Polynomial F, double x);

/**
 * Derive a polynomial
 * This function return the derivative of a given polynomial
 * @warning the degree of the polynomial must be superior to 0
 * @param F - the polynomial to derive
 * @return F'
 */
Polynomial derive(Polynomial F);

/**
 * Print a polynomial
 * Print a given polynomial in the terminal
 * @param F - The given polynomial
 */
void printPolynomial(Polynomial F);

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
Polynomial syntheticDivision(Polynomial F, double root);

/**
 * Application of the Newton Method
 * This function apply the Newton Method to find a root of the given polynomial
 * @param F - The given polynomial
 * @return approximation of a root of the given polynomial
 */
double newtonMethod(Polynomial F);

/**
 * Find the roots of a polynomial
 * This function return the roots of a given polynomial of any degree
 * @warning the roots must be reals
 * @param F - The polynomial to solve
 * @return roots of the polynomial
 */
Solutions *solve(Polynomial F);

/**
 * Print a group of solutions
 * This function print a group of solutions in the terminal
 * @param x - Solutions to print
 */
void printSolutions(Solutions *x);

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Variables interactions
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * Transform a variable into a polynomial
 * This function returns a polynomial of degree 0 with the variable as coefficient, it is used to make interactions between variables and polynomial easier
 * @param variable - The variable to convert
 * @return corresponding polynomial
 */
Polynomial variableToPolynomial(Variable variable);

#endif //LINEARALGEBRA_POLYNOMIAL_H