/**
 * @file polynomial.c Functions on polynomial
 * @author Valentin Koeltgen
 *
 * This file contain all operations on polynomial
 */

#include "polynomial.h"

Polynomial *newPolynomial(int degree) {
    Polynomial *new = malloc(sizeof(Polynomial));
    new->name = NULL;
    new->highestDegree = degree;
    new->coefficient = calloc(degree + 1, sizeof(double));
    return new;
}

void freePolynomial(Polynomial *F) {
    if (F){
        free(F->coefficient); free(F);
        F = NULL;
    }
}

Polynomial *copyPolynomial(Polynomial *F) {
    if (F) {
        Polynomial *copy = newPolynomial(F->highestDegree);
        for (int i = 0; i <= F->highestDegree; i++) copy->coefficient[i] = F->coefficient[i];
        return copy;
    } else return NULL;
}

double apply(Polynomial *F, double x) {
    if (F) {
        double result = F->coefficient[0];
        for (int i = 1; i <= F->highestDegree; i++) {
            result += F->coefficient[i] * x;
            x *= x;
        }
        return result;
    } else exit(EXIT_FAILURE);
}

Polynomial *derive(Polynomial *F) {
    if (F) {
        Polynomial *FPrime;
        if (F->highestDegree == 0) {
            FPrime = newPolynomial(F->highestDegree);
            FPrime->coefficient[0] = 0;
        } else {
            FPrime = newPolynomial(F->highestDegree - 1);
            for (int i = 0; i < F->highestDegree; i++) FPrime->coefficient[i] = F->coefficient[i + 1] * (i + 1);
        }
        return FPrime;
    } else return NULL;
}

void printPolynomial(Polynomial *F) {
    if (F) {
        if (F->name) printf("%s(X) = ", F->name);
        if (F->highestDegree == 0 && F->coefficient[0] == 0) printf("%1.1lf", F->coefficient[0]);
        else {
            for (int i = F->highestDegree; i >= 0; i--) {
                if (F->coefficient[i]) {
                    //Choose the sign
                    int sign = 1;
                    if (i != F->highestDegree) {
                        if (F->coefficient[i] < 0) {
                            printf(" - ");
                            sign = -1;
                        } else printf(" + ");
                    }
                    //Print the value with the power of X
                    if (i == 0) printf("%1.1lf", F->coefficient[i] * sign);
                    else if (i == 1) printf("%1.1lfX", F->coefficient[i] * sign);
                    else printf("%1.1lfX^%d", F->coefficient[i] * sign, i);
                }
            }
        }
        printf("\n");
    } else printf("No F\n");
}

int degreeOfString(const char *string, int start, int end) {
    int degree = 0;
    for (int i = start; string[i] != '\0' && i <= end; i++) {
        //Count in parenthesis
        if (string[i] == '(') {
            //Search for end of parenthesis
            int partialEnd = i, counter = 1;
            do {
                partialEnd++;
                if (string[partialEnd] == '(') counter++;
                else if (string[partialEnd] == ')') counter--;
            } while (counter > 0);
            //degree of the parenthesis
            int localDegree = degreeOfString(string, i + 1, partialEnd - 1);
            if (localDegree > degree) degree = localDegree;
            i = partialEnd;
        } else if (string[i] == '*'){
            //Case of multiplication
            while (string[i] != '(') i++;
            //Search for end of parenthesis
            int partialEnd = i, counter = 1;
            do {
                partialEnd++;
                if (string[partialEnd] == '(') counter++;
                else if (string[partialEnd] == ')') counter--;
            } while (counter > 0);
            //degree of the parenthesis
            degree += degreeOfString(string, i + 1, partialEnd - 1);
            i = partialEnd;
        } else {
            //If power > actual change for the new
            if (string[i] == 'X'){
                if (string[i+1] == '^') {
                    if (string[i+2] >= '1' && string[i+2] <= '9' && string[i+2] - '0' > degree) {
                        degree = string[i+2] - '0'; i += 2;
                    }
                } else if (degree < 1) {
                    degree = 1; i++;
                }
            }
        }
    }
    return degree;
}

Polynomial *pAdd(Polynomial *F, Polynomial *G) {
    Polynomial *lowerPolynomial, *higherPolynomial;
    if (F->highestDegree < G->highestDegree) {
        lowerPolynomial = F;
        higherPolynomial = G;
    } else {
        lowerPolynomial = G;
        higherPolynomial = F;
    }
    Polynomial *output = newPolynomial(higherPolynomial->highestDegree);

    for (int i = 0; i <= lowerPolynomial->highestDegree; i++) {
        output->coefficient[i] = lowerPolynomial->coefficient[i] + higherPolynomial->coefficient[i];
    }
    for (int i = lowerPolynomial->highestDegree + 1; i <= higherPolynomial->highestDegree; i++) {
        output->coefficient[i] = higherPolynomial->coefficient[i];
    }
    return output;
}

Polynomial *pMinus(Polynomial *F, Polynomial *G) {
    int highestDegree = F->highestDegree > G->highestDegree ? F->highestDegree : G->highestDegree;
    Polynomial *output = newPolynomial(highestDegree);

    for (int i = 0; i <= F->highestDegree && i <= G->highestDegree; i++) output->coefficient[i] = F->coefficient[i] - G->coefficient[i];

    if (highestDegree == F->highestDegree) {
        for (int i = G->highestDegree + 1; i <= F->highestDegree; i++) output->coefficient[i] = F->coefficient[i];
    } else {
        for (int i = F->highestDegree + 1; i <= G->highestDegree; i++) output->coefficient[i] = -G->coefficient[i];
    }
    return output;
}

Polynomial *pMultiply(Polynomial *F, Polynomial *G) {
    Polynomial *output = newPolynomial(F->highestDegree + G->highestDegree);
    for (int i = 0; i <= F->highestDegree; i++) {
        for (int j = 0; j <= G->highestDegree; j++) output->coefficient[i + j] += F->coefficient[i] * G->coefficient[j];
    }
    return output;
}

Polynomial *next(const char *string, int *start) {
    int firstPosition = *start, nbOfParenthesis = 0;
    while (string[*start] != '\0' && nbOfParenthesis >= 0 && ((string[*start] != '-' && string[*start] != '+') || nbOfParenthesis >= 1)) {
        if (string[*start] == '(') nbOfParenthesis++;
        else if (string[*start] == ')') nbOfParenthesis--;
        (*start)++;
    }
    return stringToPolynomial(string, firstPosition, *start - 1);
}

Polynomial *readFirstCoefficient(const char *string, int *start) {
    double value = readDoubleInString(string, start);
    int index = -1;
    //If there is no X after the value
    if (string[*start] != 'X') index = 0;
    else {
        //If there is no power after the X
        if (string[*start + 1] != '^') {
            index = 1; (*start)++;
        }
        //If there is a power after the X
        else if (string[*start + 1] == '^' && string[*start + 2] >= '0' && string[*start + 2] <= '9') {
            index = (int) readDoubleInString(string, start);
        }
    }
    Polynomial *output = newPolynomial(index);
    output->coefficient[index] += value;
    return output;
}

Polynomial *stringToPolynomial(const char *string, int start, int end) {
    if (string) {
        //Recuperation of the first coefficient
        Polynomial *F = readFirstCoefficient(string, &start);
        //Recuperation of the rest
        for (int currentSign = start; string[currentSign] != '\0' && currentSign < end;) {
            while (string[currentSign] != '\0' && string[currentSign] != '-' && string[currentSign] != '+' && string[currentSign] != '*') currentSign++;
            currentSign++;
            if (string[currentSign-1] != '\0' && string[currentSign-1] != ')') {
                if (string[currentSign-1] == '-' || string[currentSign-1] == '+') {
                    F = pAdd(F, next(string, &currentSign));
                } else {
                    F = pMultiply(F, next(string, &currentSign));
                }
            }
        }
        return F;
    } else return NULL;
}

Polynomial *syntheticDivision(Polynomial *F, double root) {
    if (F) {
        Polynomial *quotient = newPolynomial(F->highestDegree - 1);

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
    if (F && F->highestDegree > 0) {
        //If the maximum degree is 1, the root is easy to find
        if (F->highestDegree == 1) return -F->coefficient[0] / F->coefficient[1];
        else {
            //Initialisation
            double x0 = 1, x1, tolerance = 1e-9, epsilon = 1e-15;
            Polynomial *fPrime = derive(F);
            int maxIterations = 100; char solutionFound = 0;
            //Application of the method until precision or number of iterations is reached
            for (int i = 1; i < maxIterations; i++) {
                double y = apply(F, x0), yPrime = apply(fPrime, x0);
                if ((yPrime < 0 ? -yPrime : yPrime) < epsilon) break;
                x1 = x0 - y / yPrime;
                if ((x1 - x0 < 0 ? -(x1 - x0) : x1 - x0) <= tolerance) {
                    solutionFound = 1; break;
                }
                x0 = x1;
            }
            //End of the method, free temporary values and return output
            freePolynomial(fPrime);
            if (!solutionFound) return IMAGINARY;
            return x1;
        }
    } else exit(EXIT_FAILURE);
}

int roundDouble(double value) {
    int intPart = (int) value;
    if (value >= intPart + 0.5) return intPart + 1;
    else return intPart;
}

double roundPreciseDouble(double value) {
    double difference = (roundDouble(value)) - value;
    if (difference < 0) difference *= -1;
    if (difference < 1e-9) return roundDouble(value);
    else return value;
}

Solutions *solve(Polynomial *F) {
    if (F) {
        Solutions *x = malloc(sizeof(Solutions));
        x->size = F->highestDegree;
        x->values = malloc(x->size * sizeof(double));
        Polynomial *temp = copyPolynomial(F);
        for (int i = 0; i < x->size; i++) {
            double root = roundPreciseDouble(newtonMethod(temp));
            if (root == IMAGINARY) return NULL;
            x->values[i] = root;
            temp = syntheticDivision(temp, x->values[i]);
        }
        return x;
    } else return NULL;
}

void printSolutions(Solutions *x) {
    if (x && x->size > 0) {
        printf("{%1.2lf", x->values[0]);
        for (int i = 1; i < x->size; i++) printf(", %1.2lf", x->values[1]);
        printf("}\n");
    } else printf("No solutions or some are complex numbers\n");
}