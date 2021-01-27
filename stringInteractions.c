/**
 * @file stringMatrix.c Functions on strings
 * @author Valentin Koeltgen
 *
 * This file contain all operations on strings
 */

#include "stringInteractions.h"

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
        else if (*temp == '.'){
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

Matrix *readMatrixWIP(){
    FILE *currentFile;
    if ((currentFile = fopen("../testFile.txt", "rb")) == NULL) exit(EXIT_FAILURE);

    //Creation of the matrix object
    Matrix *matrix = (Matrix*) malloc(sizeof(Matrix));

    //Initialisation
    char temp = ' ';
    while (temp != '[') fscanf(currentFile, "%c", &temp);
    matrix->values = (double**) malloc(sizeof(double*));

    double first; //TODO Solve problem with values[0][0]

    //First Row
    double *holder = (double*) malloc(10 * sizeof(double));
    for (matrix->columns = 0; temp == '[' || temp == ','; matrix->columns++) {
        holder[matrix->columns] = readDoubleInFile(currentFile, &temp);
        //if (matrix->columns == 0) first = holder[0];
        if (matrix->columns > 0 && matrix->columns % 10 == 0) holder = realloc(holder, matrix->columns + 10);
    }
    matrix->values[0] = (double*) malloc(matrix->columns * sizeof(double));
    for (int i = 0; i < matrix->columns; i++) matrix->values[0][i] = holder[i];
    free(holder);

    //Other rows
    if (matrix->columns > 0 && temp == ';') {
        for (matrix->rows = 1; temp == ';'; matrix->rows++) {
            matrix->values[matrix->rows] = (double*) malloc(matrix->columns * sizeof(double));
            matrix->values[matrix->rows][0] = readDoubleInFile(currentFile, &temp);
            for (int j = 1; j < matrix->columns && temp == ','; j++) {
                matrix->values[matrix->rows][j] = readDoubleInFile(currentFile, &temp);
            }
        }
    }

    //matrix->values[0][0] = first;
    fclose(currentFile);
    return matrix;
}

char *readString(FILE *current){
    int i = 0;
    char temp, *string = (char*) malloc(20 * sizeof(char)); //On part d'une chaîne de 20 caractères
    fscanf(current, " %c", &temp);
    while (temp >= ' ' && temp <= '~'){
        if (i % 19 == 1 && i > 19) string = (char *) realloc (string, (i + 20) * sizeof(char)); //Si on dépasse 20 caractères, on ajoute un espace de 20 caractères à la chaîne
        string[i] = temp;
        fscanf(current, "%c", &temp);
        i++;
    } string[i] = '\0';
    return string;
}

Matrix *readMatrixInFile(){
    FILE *currentFile;
    if ((currentFile = fopen("../testFile.txt", "rb")) == NULL) exit(EXIT_FAILURE);
    char *firstLine = readString(currentFile);

    //Creation of the matrix
    Matrix *matrix = malloc(sizeof(Matrix));
    matrix->columns = matrix->rows = 1;
    for (int i = 0; firstLine[i] != ']' && firstLine[i] != '\0'; i++) {
        if (matrix->rows == 1 && firstLine[i] == ',') matrix->columns++;
        else if (firstLine[i] == ';') matrix->rows++;
    }

    //Initialisation of the matrix
    char temp = ' ';
    rewind(currentFile);
    while (temp != '[') fscanf(currentFile, "%c", &temp);

    matrix->values = (double**) malloc(matrix->rows * sizeof(double*));
    for (int k = 0; k < matrix->rows; k++) {
        matrix->values[k] = (double*) malloc(matrix->columns * sizeof(double));
        for (int j = 0; j < matrix->columns; j++) matrix->values[k][j] = readDoubleInFile(currentFile, &temp);
    }
    return matrix;
}

StringMatrix *toStringMatrix(Matrix *M){
    StringMatrix *toString = malloc(sizeof(StringMatrix));
    toString->columns = M->columns;
    toString->rows = M->rows;
    toString->values = malloc(toString->rows * sizeof(char***));
    for (int i = 0; i < M->rows; i++) {
        toString->values[i] = malloc(toString->columns * sizeof(char**));
        for (int j = 0; j < M->columns; j++) {
            toString->values[i][j] = malloc(20 * sizeof(char));
            snprintf(toString->values[i][j], 2000 * sizeof(char), "%lf", M->values[i][j]);
        }
    }
    return toString;
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

int length(const char *string){
    int length = 0;
    while (string[length] != '\0') length++;
    return length;
}

StringMatrix *detPForm(StringMatrix *M){
    for (int i = 0; i < M->columns; i++) {
        snprintf(M->values[i][i], (length(M->values[i][i]) + 5) * sizeof(char), "%s - X", M->values[i][i]);
    }
    return M;
}

char *detToString(StringMatrix *M){
    if (M->rows == 1 && M->columns == 1) return M->values[0][0];
    else {
        //char *result = calloc(200, sizeof(char));
        char *result = calloc(1, sizeof(char));
        for (int i = 0, sign = 1; i < M->rows; i++, sign*=-1) {
            StringMatrix *subDet = removeSRow(removeSColumn(M, 0), i);
            char *detOfSubDet = detToString(subDet);
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