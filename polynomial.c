/**
 * @file polynomial.c Functions on polynomial
 * @author Valentin Koeltgen
 *
 * This file contain all operations on polynomial
 */

#include "polynomial.h"

void printPolynomial(Polynomial *F){
    if (F != NULL) {
        printf("F(X) = %1.1lfX^%d", F->coefficient[F->highestDegree], F->highestDegree);
        for (int i = F->highestDegree - 1; i >= 0; i--) {
            if (F->coefficient[i] != 0) {
                //Choose the sign
                int sign = 1;
                if (F->coefficient[i] < 0) {
                    printf(" - ");
                    sign = -1;
                } else printf(" + ");
                //Print the value with the power of X
                if (i == 1) printf("%1.1lfX", F->coefficient[i] * sign);
                else if (i == 0) printf("%1.1lf", F->coefficient[i] * sign);
                else printf("%1.1lfX^%d", F->coefficient[i] * sign, i);
            }
        }
        printf("\n");
    } else printf("No F\n");
}

void freePolynomial(Polynomial *F){
    if (F != NULL){
        free(F->coefficient); free(F);
        F = NULL;
    }
}

Polynomial *stringToPolynomial(char *string){
    if (string != NULL) {
        Polynomial *F = malloc(sizeof(Polynomial));

        //Research of the highest degree
        for (int i = 0; string[i] != '\0'; i++) {
            if (string[i] == '^') {
                if (string[i + 1] >= '1' && string[i + 1] <= '9' && string[i + 1] - '0' > F->highestDegree) {
                    F->highestDegree = string[i + 1] - '0';
                }
            }
        }

        //Recuperation of the coefficients
        F->coefficient = calloc(F->highestDegree + 1, sizeof(double));
        for (int i = 0; string[i] != '\0'; i++) {
            double temp = readDoubleInString(string, &i);
            do {
                //If there is no X after the value
                if (string[i] != 'X') {
                    F->coefficient[0] += temp;
                } else {
                    //If there is no power after the X
                    if (string[i + 1] == ' ' || string[i + 1] == '+' || string[i + 1] == '-') {
                        F->coefficient[1] += temp;
                        i++;
                        //If there is a power after the X
                    } else if (string[i + 1] == '^' && string[i + 2] >= '0' && string[i + 2] <= '9') {
                        i += 2;
                        int index = (int) readDoubleInString(string, &i);
                        F->coefficient[index] += temp;
                    }
                }
            } while (string[i] != '+' && string[i] != '-' && string[i] != ' ' && string[i] != '\0');
        }
        return F;
    } else return NULL;
}

double apply(Polynomial *F, double x){
    if (F != NULL) {
        double result = F->coefficient[0];
        for (int i = 1; i <= F->highestDegree; i++) {
            result += F->coefficient[i] * x;
            x *= x;
        }
        return result;
    } else exit(EXIT_FAILURE);
}

Polynomial *derive(Polynomial *F){
    if (F != NULL && F->highestDegree > 0) {
        Polynomial *FPrime = malloc(sizeof(Polynomial));
        FPrime->coefficient = malloc(F->highestDegree * sizeof(double));
        FPrime->highestDegree = F->highestDegree - 1;
        for (int i = 0; i < F->highestDegree; i++) FPrime->coefficient[i] = F->coefficient[i + 1] * (i + 1);
        return FPrime;
    } else return NULL;
}

Polynomial *syntheticDivision(Polynomial *F, double root){
    if (F != NULL) {
        Polynomial *quotient = malloc(sizeof(Polynomial));
        quotient->coefficient = malloc((F->highestDegree) * sizeof(double));
        quotient->highestDegree = F->highestDegree - 1;

        double temp = F->coefficient[F->highestDegree];

        for (int i = 0, j = 0; i < F->highestDegree; i++) {
            if (i == 0) quotient->coefficient[quotient->highestDegree - j++] = F->coefficient[F->highestDegree];
            else {
                temp = temp * root + F->coefficient[F->highestDegree - i];
                quotient->coefficient[quotient->highestDegree - j++] = temp;
            }
        }
        return quotient;
    } else return NULL;
}

double newtonMethod(Polynomial *F) {
    if (F != NULL && F->highestDegree > 0) {
        //If the maximum degree is 1, the root is easy to find
        if (F->highestDegree == 1){
            return F->coefficient[0] / F->coefficient[1];
        } else {
            //Initialisation
            double x0 = 1, x1;
            Polynomial *fPrime = derive(F);
            double tolerance = 1e-7, epsilon = 1e-14;
            int maxIterations = 100;
            Boolean solutionFound = false;
            //Application of the method until precision or number of iterations is reached
            for (int i = 1; i < maxIterations; i++) {
                double y = apply(F, x0), yPrime = apply(fPrime, x0);
                if ((yPrime < 0 ? -yPrime : yPrime) < epsilon) break;
                x1 = x0 - y / yPrime;
                if ((x1 - x0 < 0 ? -(x1 - x0) : x1 - x0) <= tolerance) {
                    solutionFound = true; break;
                }
                x0 = x1;
            }
            //End of the method, free temporary values and return output
            freePolynomial(fPrime);
            if (solutionFound == false) printf("Didn't converge\n");
            return x1;
        }
    } else exit(EXIT_FAILURE);
}

Polynomial *copyPolynomial(Polynomial *F) {
    if (F != NULL) {
        Polynomial *copy = malloc(sizeof(Polynomial));
        copy->highestDegree = F->highestDegree;
        copy->coefficient = malloc((F->highestDegree + 1) * sizeof(double));
        for (int i = 0; i <= F->highestDegree; i++) copy->coefficient[i] = F->coefficient[i];
        return copy;
    } else return NULL;
}

double *solve(Polynomial *F) {
    if (F != NULL) {
        double *root = malloc(F->highestDegree * sizeof(double));
        Polynomial *temp = copyPolynomial(F);
        for (int i = 0; i < F->highestDegree; i++) {
            root[i] = newtonMethod(temp);
            temp = syntheticDivision(temp, root[i]);
        }
        return root;
    } else return NULL;
}