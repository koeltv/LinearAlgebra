/**
 * @file polynomial.c Functions on polynomial
 * @author Valentin Koeltgen
 *
 * This file contain all operations on polynomial
 */

#include "polynomial.h"

Polynomial newPolynomial(int degree) {
    if (degree < 0) return nullPolynomial;
    else return (Polynomial) {NULL, calloc(degree + 1, sizeof(double)), degree};
}

Polynomial stringToPolynomial(const char *string) {
    Polynomial output; int index = 0; double coefficient = 1;
    while (string[index] && string[index] == ' ') index++;
    //If there is a numerical coefficient read it
    if (string[index] && string[index] != 'X') coefficient = readDoubleInString(string, &index);
    if (string[index] && string[index == 'X']) {
        if (string[++index] != '^') {
            output = newPolynomial(1);
            output.coefficient[1] = coefficient;
        } else {
            int degree = (int) readDoubleInString(string, &index);
            if (degree < 0) return newPolynomial(-1);
            output = newPolynomial(degree);
            output.coefficient[degree] = coefficient;
        }
        return output;
    } else return newPolynomial(-1);
}

Polynomial copyPolynomial(Polynomial F) {
    Polynomial copy = newPolynomial(F.highestDegree);
    for (int i = 0; i <= F.highestDegree; i++) copy.coefficient[i] = F.coefficient[i];
    return copy;
}

void freePolynomial(Polynomial *F) {
    if (F) free(F->coefficient);
}

double apply(Polynomial F, double x) {
    double result = F.coefficient[0], powerOfX = x;
    for (int i = 1; i <= F.highestDegree; i++) {
        result += F.coefficient[i] * powerOfX;
        powerOfX *= x;
    }
    return result;
}

Polynomial derive(Polynomial F) {
    Polynomial FPrime;
    if (F.highestDegree == 0) {
        FPrime = newPolynomial(F.highestDegree);
        FPrime.coefficient[0] = 0;
    } else {
        FPrime = newPolynomial(F.highestDegree - 1);
        for (int i = 0; i < F.highestDegree; i++) FPrime.coefficient[i] = F.coefficient[i + 1] * (i + 1);
    }
    return FPrime;
}

void printPolynomial(Polynomial F) {
    if (F.name) printf("%s(X) = ", F.name);
    if (F.highestDegree == 0 && F.coefficient[0] == 0) printf("0");
    else {
        for (int i = F.highestDegree; i >= 0; i--) {
            if (F.coefficient[i]) {
                //Choose the sign
                int sign = 1;
                if (i != F.highestDegree) {
                    if (F.coefficient[i] < 0) {
                        printf(" - ");
                        sign = -1;
                    } else printf(" + ");
                }
                //Print the value with the power of X
                if (i == 0) printf("%1.1lf", F.coefficient[i] * sign);
                else if (i == 1) {
                    if (F.coefficient[i] == 1) printf("X");
                    else printf("%1.1lfX", F.coefficient[i] * sign);
                } else {
                    if (F.coefficient[i] == 1) printf("X^%d", i);
                    else printf("%1.1lfX^%d", F.coefficient[i] * sign, i);
                }
            }
        }
    }
    printf("\n");
}

void eliminateNullCoefficients(Polynomial *F) {
    for (int i = F->highestDegree; i > 0; i++) {
        if (F->coefficient[i] == 0) {
            Polynomial temp = newPolynomial(F->highestDegree - 1);
            for (int j = 0; j <= temp.highestDegree; j++) temp.coefficient[j] = F->coefficient[j];
            *F = temp;
        } else break;
    }
}

Polynomial pAdd(Polynomial F, Polynomial G) {
    Polynomial lowerPolynomial, higherPolynomial;
    if (F.highestDegree < G.highestDegree) {
        lowerPolynomial = F;
        higherPolynomial = G;
    } else {
        lowerPolynomial = G;
        higherPolynomial = F;
    }
    Polynomial output = newPolynomial(higherPolynomial.highestDegree);

    for (int i = 0; i <= lowerPolynomial.highestDegree; i++) {
        output.coefficient[i] = lowerPolynomial.coefficient[i] + higherPolynomial.coefficient[i];
    }
    for (int i = lowerPolynomial.highestDegree + 1; i <= higherPolynomial.highestDegree; i++) {
        output.coefficient[i] = higherPolynomial.coefficient[i];
    }
    
    eliminateNullCoefficients(&output);
    return output;
}

Polynomial pMinus(Polynomial F, Polynomial G) {
    int highestDegree = F.highestDegree > G.highestDegree ? F.highestDegree : G.highestDegree;
    Polynomial output = newPolynomial(highestDegree);

    for (int i = 0; i <= F.highestDegree && i <= G.highestDegree; i++) output.coefficient[i] = F.coefficient[i] - G.coefficient[i];

    if (highestDegree == F.highestDegree) {
        for (int i = G.highestDegree + 1; i <= F.highestDegree; i++) output.coefficient[i] = F.coefficient[i];
    } else {
        for (int i = F.highestDegree + 1; i <= G.highestDegree; i++) output.coefficient[i] = -G.coefficient[i];
    }
    
    eliminateNullCoefficients(&output);
    return output;
}

Polynomial pMultiply(Polynomial F, Polynomial G) {
    Polynomial output = newPolynomial(F.highestDegree + G.highestDegree);
    for (int i = 0; i <= F.highestDegree; i++) {
        for (int j = 0; j <= G.highestDegree; j++) output.coefficient[i + j] += F.coefficient[i] * G.coefficient[j];
    }
    eliminateNullCoefficients(&output);
    return output;
}

char isPolynomialNull(Polynomial F) {
    char nonNullValue = 0;
    for (int i = 0; i <= F.highestDegree; i++) {
        if (F.coefficient[i]) nonNullValue = 1;
    }
    if (nonNullValue) return 0;
    else return 1;
}

Polynomial pLongDivide(Polynomial numerator, Polynomial denominator) {
    if (!isPolynomialNull(denominator)) {
        Polynomial quotient = newPolynomial(0), remainder = numerator;
        while (!isPolynomialNull(remainder) && remainder.highestDegree >= denominator.highestDegree) {
            Polynomial temp = newPolynomial(remainder.highestDegree - denominator.highestDegree);
            highestCoefficient(temp) = highestCoefficient(remainder) / highestCoefficient(denominator);
            quotient = pAdd(quotient, temp);
            remainder = pMinus(remainder, pMultiply(temp, denominator));
        }
        if (!isPolynomialNull(remainder)) {
            printf("There is a remainder in the long division : ");
            printPolynomial(remainder);
        }
        return quotient;
    } else return newPolynomial(-1);
}

Polynomial syntheticDivision(Polynomial F, double root) {
    Polynomial quotient = newPolynomial(F.highestDegree - 1);
    double temp = highestCoefficient(F);
    for (int i = 0, j = 0; i < F.highestDegree; i++) {
        if (i == 0) quotient.coefficient[quotient.highestDegree - j++] = highestCoefficient(F);
        else {
            temp = temp * root + F.coefficient[F.highestDegree - i];
            quotient.coefficient[quotient.highestDegree - j++] = temp;
        }
    }
    return quotient;
}

double searchNull(Polynomial F) {
    double leftIndex = 0, rightIndex = 0, temp;
    Polynomial fPrime = derive(F);
    for (int i = 0; absolute(apply(fPrime, leftIndex)) < 1000 && absolute(apply(fPrime, rightIndex)) < 1000; i++) {
        //Search on the left side
        temp = leftIndex;
        //Move the index to the left
        if (apply(fPrime, leftIndex) < 0) leftIndex += apply(fPrime, leftIndex);
        else leftIndex--;
        //Verify if 0 is in the interval
        if (apply(F, leftIndex) == 0) return leftIndex;
        else if (apply(F, temp) == 0) return temp;
        else if (apply(F, leftIndex) * apply(F, temp) < 0) return (leftIndex - temp) / 2;
        //Search on the right side
        temp = rightIndex;
        //Move the index to the right
        if (apply(fPrime, rightIndex) > 0) leftIndex += apply(fPrime, rightIndex);
        else rightIndex++;
        //Verify if 0 is in the interval
        if (apply(F, rightIndex) == 0) return rightIndex;
        else if (apply(F, temp) == 0) return temp;
        else if (apply(F, rightIndex) * apply(F, temp) < 0) return (rightIndex - temp) / 2;
    }
    return 1;
}

double newtonMethod(Polynomial F) {
    if (F.highestDegree > 0) {
        //Initialisation
        double x0 = searchNull(F), x1, tolerance = 1e-9, epsilon = 1e-15;
        Polynomial fPrime = derive(F);
        char maxIterations = 50, solutionFound = 0;
        //Application of the method until precision or number of iterations is reached
        for (int i = 1; i < maxIterations; i++) {
            double y = apply(F, x0), yPrime = apply(fPrime, x0);
            if (absolute(yPrime) < epsilon) break;
            x1 = x0 - y / yPrime;
            if (absolute(x1 - x0) <= tolerance) {
                solutionFound = 1; break;
            }
            x0 = x1;
        }
        //End of the method, free temporary values and return output
        freePolynomial(&fPrime);
        if (solutionFound) return x1;
    }
    return IMAGINARY;
}

Polynomial depress(Polynomial F) {
    if (F.highestDegree == 4) {
        Polynomial depressedForm = copyPolynomial(F);
        double A = depressedForm.coefficient[4], B = depressedForm.coefficient[3], C = depressedForm.coefficient[2], D = depressedForm.coefficient[1], E = depressedForm.coefficient[0];
        depressedForm.coefficient[4] = 1;
        depressedForm.coefficient[3] = 0;
        depressedForm.coefficient[2] = (-3 * power(B, 2)) / (8 * power(A, 2)) + C / A;
        depressedForm.coefficient[1] = power(B, 3) / (8 * power(A, 3)) - (B * C) / (2 * power(A, 2)) + D / A;
        depressedForm.coefficient[0] = (-3 * power(B, 4)) / (256 * power(A, 4)) + (C * power(B, 2)) / (16 * power(A, 3)) - (B * D) / (4 * power(A, 2)) + E / A;
        return depressedForm;
    } else return F;
}

Solutions *solve(Polynomial F) {
    if (F.highestDegree > 0) {
        Solutions *x = malloc(sizeof(Solutions));
        *x = (Solutions) {F.highestDegree, malloc(F.highestDegree * sizeof(double))};
        Polynomial temp = F;
        for (int i = 0; i < x->size; i++) {
            double root = IMAGINARY, delta;
            if (temp.coefficient[0] == 0) { //If the lowest coefficient is 0, it is a root of the polynomial
                root = 0;
            } else if (i > 0 && absolute(apply(temp, x->values[i-1])) < 1e-5) { //If a root as a multiplicity > 1
                root = x->values[i-1];
            } else if (temp.highestDegree == 1) { //x = -c/b
                root = -temp.coefficient[0] / temp.coefficient[1];
            } else if (temp.highestDegree == 2) { //x = (-b^2 +/- sqrt(delta))/(2a)
                //delta = b^2 - 4ac
                delta = power(temp.coefficient[1], 2) - 4 * temp.coefficient[2] * temp.coefficient[0];
                if (delta == 0) root = -temp.coefficient[1]/(2 * temp.coefficient[2]);
                else if (delta > 0) {
                    double rootOfDelta = roundPreciseDouble(sqrt(delta));
                    x->values[i] = (-temp.coefficient[1] - rootOfDelta) / (2 * temp.coefficient[2]);
                    root = (-temp.coefficient[1] + rootOfDelta) / (2 * temp.coefficient[2]);
                    temp = syntheticDivision(temp, x->values[i]); i++;
                }
            } else if (temp.highestDegree == 4) {
                double a0 = temp.coefficient[4], a1 = temp.coefficient[3], a2 = temp.coefficient[2], a3 = temp.coefficient[1], a4 = temp.coefficient[0];
                //Evident roots: 1 and −1 and −k
                if (a0 + a1 + a2 + a3 + a4 == 0) root = 1;
                else if (a0 + a2 + a4 == a1 + a2) root = -1;
                else if (a4 == a3 * (a1 / a0)) root = -a1 / a0;
                //Biquadratic equations
                else if (a3 == a1 == 0) {
                    Polynomial biquadraticForm = {NULL, (double[]) {a4, a2, a0}, 2};
                    Solutions *biquadraticSolutions = solve(biquadraticForm);
                    if (biquadraticSolutions) {
                        x->values[i] = biquadraticSolutions->values[0];
                        root = biquadraticSolutions->values[1];
                        temp = syntheticDivision(temp, x->values[i]); i++;
                    }
                //Converting to a depressed quartic
                } else if (a3 != 0) {
                    Solutions *depressedSolutions = solve(depress(temp));
                    if (depressedSolutions) {
                        //We use reverse substitution
                        for (int j = 0; j < depressedSolutions->size; j++) depressedSolutions->values[i] -= a1 / (4 * a0);
                        return depressedSolutions;
                    }
                } else root = newtonMethod(temp);
            } else { //Degree higher than 4
                root = newtonMethod(temp);
            }
            if (root == IMAGINARY) return NULL;
            x->values[i] = roundPreciseDouble(root);
            if (temp.highestDegree > 1) temp = syntheticDivision(temp, root);
        }
        return x;
    } else return NULL;
}

void printSolutions(Solutions *x) {
    if (x && x->size > 0) {
        printf("{%1.2lf", x->values[0]);
        for (int i = 1; i < x->size; i++) printf(", %1.2lf", x->values[i]);
        printf("}\n");
    } else printf("No solutions or some are complex numbers\n");
}

Polynomial variableToPolynomial(Variable variable) {
    Polynomial result = newPolynomial(0);
    result.coefficient[0] = variable.value;
    return result;
}