/**
 * @file polynomial.c Functions on polynomial
 * @author Valentin Koeltgen
 *
 * This file contain all operations on polynomial
 */

#include <stdlib.h>
#include <stdio.h>
#include "polynomial.h"

double readDoubleInString(const char *string, int *position){
    double result = 0;
    while (string[*position] == ' ') (*position)++;

    //Read the sign
    int sign = 1;
    if (string[*position] == '-') sign = -1;
    while (string[*position] == ' ' || string[*position] == '+' || string[*position] == '-') (*position)++;

    //Read the value
    do {
        if (string[*position] >= '0' && string[*position] <= '9') {
            result = result * 10 + string[*position] - '0';
            (*position)++;
        }
        else if (string[*position] == '.'){
            double decimal = 0.1;
            (*position)++;
            do {
                result += (string[*position] - '0') * decimal;
                decimal /= 10;
                (*position)++;
            } while (string[*position] >= '0' && string[*position] <= '9');
        }
    } while (string[*position] == '.' || (string[*position] >= '0' && string[*position] <= '9'));
    return result * sign;
}

void printPolynomial(Polynomial *polynomial){
    printf("F(X) = %1.1lfX^%d", polynomial->coefficient[polynomial->highestDegree], polynomial->highestDegree);
    for (int i = polynomial->highestDegree-1; i >= 0; i--) {
        if (polynomial->coefficient[i] != 0) {
            int sign = 1;
            if (polynomial->coefficient[i] < 0) {
                printf(" - ");
                sign = -1;
            } else printf(" + ");
            if (i == 1) printf("%1.1lfX", polynomial->coefficient[i] * sign);
            else if (i == 0) printf("%1.1lf", polynomial->coefficient[i] * sign);
            else printf("%1.1lfX^%d", polynomial->coefficient[i] * sign, i);
        }
    } printf("\n");
}

void freePolynomial(Polynomial *polynomial){
    if (polynomial != NULL){
        free(polynomial->coefficient);
        free(polynomial);
        polynomial = NULL;
    }
}

Polynomial *stringToPolynomial(char *string){
    //char *string = readString(stdin);
    Polynomial *equation = malloc(sizeof(Polynomial));
    equation->highestDegree = 0;

    //Research of the highest degree
    for (int i = 0; string[i] != '\0'; i++) {
        if (string[i] == '^'){
            if (string[i+1] >= '1' && string[i+1] <= '9' && string[i+1] - '0' > equation->highestDegree) equation->highestDegree = string[i + 1] - '0';
        }
    }

    //Recuperation of the coefficients
    equation->coefficient = calloc(equation->highestDegree + 1, sizeof(double));
    for (int i = 0; string[i] != '\0'; i++) {
        double temp = readDoubleInString(string, &i);
        do {
            //If there is no X after the value
            if (string[i] != 'X'){
                equation->coefficient[0] += temp;
            } else {
                //If there is no power after the X
                if (string[i + 1] == ' ' || string[i + 1] == '+' || string[i + 1] == '-'){
                    equation->coefficient[1] += temp;
                    i++;
                    //If there is a power after the X
                } else if (string[i + 1] == '^' && string[i + 2] >= '0' && string[i + 2] <= '9'){
                    i += 2;
                    int index = (int)readDoubleInString(string, &i);
                    equation->coefficient[index] += temp;
                }
            }
        } while (string[i] != '+' && string[i] != '-' && string[i] != ' ' && string[i] != '\0');
    }
    return equation;
}

double apply(Polynomial *polynomial, double x){
    double result = polynomial->coefficient[0];
    for (int i = 1; i <= polynomial->highestDegree; i++) {
        result += polynomial->coefficient[i] * x;
        x *= x;
    }
    return result;
}

Polynomial *derive(Polynomial *polynomial){
    if (polynomial->highestDegree > 0) {
        Polynomial *derivative = malloc(sizeof(Polynomial));
        derivative->coefficient = malloc(polynomial->highestDegree * sizeof(double));
        derivative->highestDegree = polynomial->highestDegree - 1;
        for (int i = 0; i < polynomial->highestDegree; i++) {
            derivative->coefficient[i] = polynomial->coefficient[i + 1] * (i + 1);
        }
        return derivative;
    } else return NULL;
}

Polynomial *syntheticDivision(Polynomial *polynomial, double root){
    Polynomial *quotient = malloc(sizeof(Polynomial));
    quotient->coefficient = malloc((polynomial->highestDegree) * sizeof(double));
    quotient->highestDegree = polynomial->highestDegree-1;

    double val = polynomial->coefficient[polynomial->highestDegree];

    for (int i = 0, j = 0; i < polynomial->highestDegree; i++) {
        if (i == 0) quotient->coefficient[quotient->highestDegree - j++] = polynomial->coefficient[polynomial->highestDegree];
        else {
            val = val * root + polynomial->coefficient[polynomial->highestDegree - i];
            quotient->coefficient[quotient->highestDegree - j++] = val;
        }
    }
    return quotient;
}

double newtonMethod(Polynomial *polynomial) {
    //Newton method, !!! f'(x) must be != 0
    double x0 = 1, x1;
    Polynomial *fPrime = derive(polynomial);
    double tolerance = 1e-7, epsilon = 1e-14;
    int maxIterations = 20;
    Boolean solutionFound = false;

    for (int i = 1; i < maxIterations; i++) {
        double y = apply(polynomial, x0), yPrime = apply(fPrime, x0);

        if ((yPrime < 0 ? -yPrime : yPrime) < epsilon) break;

        x1 = x0 - y/yPrime;

        if ((x1 - x0 < 0 ? -(x1-x0) : x1-x0) <= tolerance){
            solutionFound = true;
            break;
        }

        x0 = x1;
    }
    if (solutionFound == false)printf("Didn't converge\n");
    return x1;
}

Polynomial *copyPolynomial(Polynomial *polynomial) {
    Polynomial *copy = malloc(sizeof(Polynomial));
    copy->highestDegree = polynomial->highestDegree;
    copy->coefficient = malloc((polynomial->highestDegree + 1) * sizeof(double));
    for (int i = 0; i <= polynomial->highestDegree; i++) {
        copy->coefficient[i] = polynomial->coefficient[i];
    }
    return copy;
}

double *solve(Polynomial *polynomial) {
    double *root = malloc(polynomial->highestDegree * sizeof(double));
    Polynomial *temp = copyPolynomial(polynomial);

    for (int i = 0; i < polynomial->highestDegree; i++) {
        root[i] = newtonMethod(temp);
        temp = syntheticDivision(temp, root[i]);
    }
    return root;
}