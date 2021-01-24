/**
 * @file polynomial.h Header file of polynomial.c
 * @author Valentin Koeltgen
 */

#ifndef LINEARALGEBRA_POLYNOMIAL_H
#define LINEARALGEBRA_POLYNOMIAL_H

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Structures
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * @struct Boolean
 * Represent a boolean value
 */
typedef enum{false, true} Boolean;

/**
 * @struct Polynomial
 * Structure representing a polynomial of any degree
 */
typedef struct {
    double *coefficient; ///Coefficients of the polynomial
    int highestDegree; ///Highest degree of the polynomial
} Polynomial;

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Construction functions
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * Read a double in a string
 * This function return a double read from a string
 * @param string - The given string
 * @param position - The position of the string where we start to read
 * @return double read
 */
double readDoubleInString(const char *string, int *position);

/**
 * Print a polynomial
 * Print a given polynomial in the terminal
 * @param polynomial - The given polynomial
 */
void printPolynomial(Polynomial *polynomial);

/**
 * Free an existing polynomial
 * This function free an existing polynomial and change its pointer to NULL if it worked successfully
 * @param polynomial - The polynomial to free
 */
void freePolynomial(Polynomial *polynomial);

/**
 * Convert a string to a polynomial
 * This function return a polynomial created from a string
 * @attention The coefficient must follow the format "<value>X^<power>" for <power> = 1, we can write "<value>X" and for <power> = 0 we can write "<value>"
 * @param string - The given string
 * @return polynomial created from the string
 */
Polynomial *stringToPolynomial(char *string);

/**
 * Copy a polynomial
 * This function return a copy of a given polynomial
 * @param polynomial - The polynomial to copy
 * @return copy created
 */
Polynomial *copyPolynomial(Polynomial *polynomial);

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Basic operator functions
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * Apply the function for a given value
 * This function apply the given function with the given value
 * @param polynomial - the given polynomial
 * @param x - the value to use
 * @return polynomial(x)
 */
double apply(Polynomial *polynomial, double x);

/**
 * Derive a polynomial
 * This function return the derivative of a given polynomial
 * @warning the degree of the polynomial must be superior to 0
 * @param polynomial - the polynomial to derive
 * @return polynomial'
 */
Polynomial *derive(Polynomial *polynomial);

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Advanced operator functions
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/**
 * Synthetic division of a polynomial
 * This function return a polynomial with 1 degree less than the given polynomial by dividing it by "X - <root>"
 * @param polynomial - polynomial to divide
 * @param root - root by which to divide
 * @return divide polynomial
 */
Polynomial *syntheticDivision(Polynomial *polynomial, double root);

/**
 * Application of the Newton Method
 * This function apply the Newton Method to find a root of the given polynomial
 * @param polynomial - The given polynomial
 * @return an approximation of the root of the given polynomial
 */
double newtonMethod(Polynomial *polynomial);

/**
 * Find the roots of a polynomial
 * This function return the roots of a given polynomial of any degree
 * @warning the roots must be reals
 * @param polynomial - The polynomial to solve
 * @return roots of the polynomial
 */
double *solve(Polynomial *polynomial);

#endif //LINEARALGEBRA_POLYNOMIAL_H