/**
 * @file stringMatrix.c Functions on strings
 * @author Valentin Koeltgen
 *
 * This file contain all operations on strings
 */

#include "stringInteractions.h"

char *readString(FILE *current){
    int i = 0;
    char temp, *string = (char*) malloc(20 * sizeof(char)); //We start from a chain of 20 characters
    fscanf(current, " %c", &temp);
    while (temp >= ' ' && temp <= '~'){
        if (i % 19 == 1 && i > 19) string = (char *) realloc (string, (i + 20) * sizeof(char)); //If we go over 20, we add 20 more to the chain
        string[i] = temp;
        fscanf(current, "%c", &temp);
        i++;
    } string[i] = '\0';
    return string;
}

int length(const char *string){
    int length = 0;
    while (string[length] != '\0') length++;
    return length;
}

short shorterString(const char *string1, const char *string2){
    int i;
    for (i = 0; string1[i] != '\0' && string2[i] != '\0'; i++) {
        if (string1[i] < string2[i]) return 1;
        else if (string1[i] > string2[i]) return 2;
    }
    if (string1[i] == '\0') return 1;
    else if (string2[i] == '\0') return 2;
    else return 0;
}

short containString(const char *mainString, const char *toSearch){
    int i = 0, j;
    while (mainString[i] != '\0' && mainString[i] != toSearch[0]) i++;
    if (mainString[i] != '\0'){
        for (j = 0; mainString[i+j] != '\0' && toSearch[j] != '\0'; j++) {
            if (mainString[i+j] != toSearch[j]) return 0;
        }
        if (toSearch[j] == '\0') return 1;
    }
    return 0;
}

double readDoubleInString(const char *string, int *position){

    //Search for the sign
    while (*position >= 0 && string[*position] == ' ') (*position)--;
    while (string[*position] != '\0' && (string[*position] < '0' || string[*position] > '9') && string[*position] != '-') (*position)++;

    //Read the sign
    int sign = 1;
    if (string[*position] == '-') sign = -1;
    while (string[*position] == ' ' || string[*position] == '+' || string[*position] == '-') (*position)++;

    //Read the value
    double result = 0;
    do {
        if (string[*position] >= '0' && string[*position] <= '9') {
            result = result * 10 + string[(*position)++] - '0';
        }
        else if (string[(*position)++] == '.'){
            double decimal = 0.1;
            do {
                result += (string[(*position)++] - '0') * decimal;
                decimal /= 10;
            } while (string[(*position)] >= '0' && string[(*position)] <= '9');
        }
    } while (string[*position] == '.' || (string[*position] >= '0' && string[*position] <= '9'));
    return result * sign;
}

double readDoubleInFile(FILE *currentFile, char *temp){
    double result = 0;
    fscanf(currentFile, " %c", temp);
    if (*temp == '-') {
        fscanf(currentFile, "%c", temp);
        result -= (double)(*temp - '0');
    } else result += (double)(*temp - '0');
    do {
        fscanf(currentFile, "%c", temp);
        if (*temp >= '0' && *temp <= '9') result = result * 10 + *temp - '0';
        else if (*temp == '.') {
            double decimal = 0.1;
            fscanf(currentFile, "%c", temp);
            do {
                result += (*temp - '0') * decimal;
                decimal /= 10;
                fscanf(currentFile, "%c", temp);
            } while (*temp >= '0' && *temp <= '9');
        }
    } while (*temp == '.' || (*temp >= '0' && *temp <= '9'));
    return result;
}

StringMatrix *newStringMatrix(int nbRows, int nbColumns, char *initialValue){
    StringMatrix* M = (StringMatrix*) malloc(sizeof(StringMatrix));
    M->rows = nbRows; M->columns = nbColumns;
    M->values = (char***) malloc(M->rows * sizeof(char**));
    for (int i = 0; i < M->rows; i++) {
        M->values[i] = (char**) malloc(M->columns * sizeof(char*));
        for (int j = 0; j < M->columns; j++) M->values[i][j] = initialValue;
    }
    return M;
}

void freeStringMatrix(StringMatrix *M){
    if (M != NULL) {
        for (int i = 0; i < M->rows; i++) {
            //for (int j = 0; j < M->columns; j++) free(M->values[i][j]);
            free(M->values[i]);
        }
        free(M->values); free(M);
        M = NULL;
    }
}

StringMatrix *removeSRow(StringMatrix *M, int rowIndex){
    if (M != NULL) {
        StringMatrix *smallerM = newStringMatrix(M->rows - 1, M->columns, 0);
        for (int i = 0; i < M->rows; i++) {
            for (int j = 0; j < M->columns; j++) {
                if (i != rowIndex) smallerM->values[i > rowIndex ? i - 1 : i][j] = M->values[i][j];
            }
        }
        return smallerM;
    } else return NULL;
}

StringMatrix *removeSColumn(StringMatrix *M, int columnIndex){
    if (M != NULL) {
        StringMatrix *smallerM = newStringMatrix(M->rows, M->columns - 1, 0);
        for (int i = 0; i < M->rows; i++) {
            for (int j = 0; j < M->columns; j++) {
                if (j != columnIndex) smallerM->values[i][j > columnIndex ? j - 1 : j] = M->values[i][j];
            }
        }
        return smallerM;
    } else return NULL;
}

StringMatrix *changeToPLambdaForm(StringMatrix *M){
    for (int i = 0; i < M->columns; i++) {
        snprintf(M->values[i][i], (length(M->values[i][i]) + 6) * sizeof(char), "%s - 1X", M->values[i][i]);
    }
    return M;
}

char *detOfStringMatrix(StringMatrix *M){
    if (M->rows == 1 && M->columns == 1) return M->values[0][0];
    else {
        char *result = calloc(1, sizeof(char));
        for (int i = 0, sign = 1; i < M->rows; i++, sign*=-1) {
            StringMatrix *subDet = removeSRow(removeSColumn(M, 0), i);
            char *detOfSubDet = detOfStringMatrix(subDet);
            int totalSize = length(M->values[i][0]) + length(detOfSubDet);

            if (sign == -1) {
                if (length(result) < 1) {
                    totalSize += 15;
                    result = realloc(result, totalSize * sizeof(char));
                    snprintf(result, totalSize * sizeof(char), "(%s) * (-1) * (%s)", M->values[i][0], detOfSubDet);
                } else {
                    totalSize += 22 + length(result);
                    result = realloc(result, totalSize * sizeof(char));
                    snprintf(result, totalSize * sizeof(char), "(%s) + ((%s) * (-1) * (%s))", result, M->values[i][0], detOfSubDet);
                }
            } else {
                if (length(result) < 1) {
                    totalSize += 8;
                    result = realloc(result, totalSize * sizeof(char));
                    snprintf(result, totalSize * sizeof(char), "(%s) * (%s)", M->values[i][0], detOfSubDet);
                } else {
                    totalSize += 15 + length(result);
                    result = realloc(result, totalSize * sizeof(char));
                    snprintf(result, totalSize * sizeof(char), "(%s) + ((%s) * (%s))", result, M->values[i][0], detOfSubDet);
                }
            }
            freeStringMatrix(subDet);
        }
        return result;
    }
}