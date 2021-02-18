/**
 * @file polynomial.c Functions on polynomial
 * @author Valentin Koeltgen
 *
 * This file contain all operations on polynomial
 */

#include "polynomial.h"

Polynomial newPolynomial(int degree) {
    if (degree < 0) return (Polynomial) {NULL, NULL, degree};
    else return (Polynomial) {NULL, calloc(degree + 1, sizeof(double)), degree};
}

void freePolynomial(Polynomial *F) {
    if (F) free(F->coefficient);
}

double apply(Polynomial F, double x) {
    double result = F.coefficient[0];
    for (int i = 1; i <= F.highestDegree; i++) {
        result += F.coefficient[i] * x;
        x *= x;
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
    if (F.highestDegree == 0 && F.coefficient[0] == 0) printf("%1.1lf", F.coefficient[0]);
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
                else if (i == 1) printf("%1.1lfX", F.coefficient[i] * sign);
                else printf("%1.1lfX^%d", F.coefficient[i] * sign, i);
            }
        }
    }
    printf("\n");
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

Polynomial next(const char *string, int *start) {
    int firstPosition = *start, nbOfParenthesis = 0;
    while (string[*start] != '\0' && nbOfParenthesis >= 0 && ((string[*start] != '-' && string[*start] != '+') || nbOfParenthesis >= 1)) {
        if (string[*start] == '(') nbOfParenthesis++;
        else if (string[*start] == ')') nbOfParenthesis--;
        (*start)++;
    }
    return stringToPolynomial(string, firstPosition, *start - 1);
}

Polynomial readFirstCoefficient(const char *string, int *start) {
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
    Polynomial output = newPolynomial(index);
    output.coefficient[index] += value;
    return output;
}

Polynomial stringToPolynomial(const char *string, int start, int end) {
    //Recuperation of the first coefficient
    Polynomial F = readFirstCoefficient(string, &start);
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

Solutions *solve(Polynomial F) {
    if (F.highestDegree > 0) {
        Solutions *x = malloc(sizeof(Solutions));
        *x = (Solutions) {F.highestDegree, malloc(F.highestDegree * sizeof(double))};
        Polynomial temp = F;
        for (int i = 0; i < x->size; i++) {
            double root, delta;
            if (i > 0 && absolute(apply(temp, x->values[i-1])) < 1e-5) { //If there is at least 2 times the same root
                root = x->values[i-1];
            } else if (temp.highestDegree == 1) { //x0 = -c/b
                root = -temp.coefficient[0] / temp.coefficient[1];
            } else if (temp.highestDegree == 2) { //x = (-b^2 +/- sqrt(delta))/(2a)
                delta = temp.coefficient[1] * temp.coefficient[1] - 4 * temp.coefficient[2] * temp.coefficient[0];
                if (delta < 0) return NULL;
                else if (delta == 0) root = (-temp.coefficient[1])/(2 * temp.coefficient[2]);
                else {
                    double squareRoot= roundPreciseDouble(sqrt(delta));
                    x->values[i] = (-temp.coefficient[1] - squareRoot) / (2 * temp.coefficient[2]);
                    root = (-temp.coefficient[1] + squareRoot) / (2 * temp.coefficient[2]);
                    temp = syntheticDivision(temp, x->values[i]); i++;
                }
            } else { //Degree higher than 2
                root = newtonMethod(temp);
                if (root == IMAGINARY) return NULL;
            }
            x->values[i] = roundPreciseDouble(root);
            temp = syntheticDivision(temp, root);
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