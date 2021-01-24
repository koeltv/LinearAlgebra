/**
 * @file matrix.c Functions on matrix
 * @author Valentin Koeltgen
 *
 * This file contain all operations on matrix
 */

#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

double readDoubleInFile(FILE *currentFile, char *temp){
    double result = 0;
    fscanf(currentFile, " %c", temp);
    if (*temp == '-'){
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

Matrix *newMatrix(int nbRows, int nbColumns, double initialValue){
    Matrix* M = (Matrix*) malloc(sizeof(Matrix));
    M->rows = nbRows; M->columns = nbColumns;
    M->values = (double**) malloc(M->rows * sizeof(double*));
    for (int i = 0; i < M->rows; i++) {
        M->values[i] = (double*) malloc(M->columns * sizeof(double));
        for (int j = 0; j < M->columns; j++) M->values[i][j] = initialValue;
    }
    return M;
}

void freeMatrix(Matrix *matrix){
    if (matrix != NULL) {
        for (int i = 0; i < matrix->rows; i++) free(matrix->values[i]);
        free(matrix->values);
        free(matrix);
        matrix = NULL;
    }
}

Matrix *removeRow(Matrix *matrix, int rowIndex){
    Matrix *M = newMatrix(matrix->rows-1, matrix->columns, 0);
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->columns; j++) {
            if (i != rowIndex) M->values[i > rowIndex ? i-1 : i][j] = matrix->values[i][j];
        }
    }
    return M;
}

Matrix *removeColumn(Matrix *matrix, int columnIndex){
    Matrix *M = newMatrix(matrix->rows, matrix->columns-1, 0);
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->columns; j++) {
            if (j != columnIndex) M->values[i][j > columnIndex ? j-1 : j] = matrix->values[i][j];
        }
    }
    return M;
}

Matrix *addColumn(Matrix *matrix){
    Matrix *M = newMatrix(matrix->rows, matrix->columns+1, 0);
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->columns; j++) {
            M->values[i][j] = matrix->values[i][j];
        }
        M->values[i][matrix->columns] = 0;
    }
    return M;
}

Matrix *subMat(Matrix* matrix, int r1, int r2, int c1, int c2){
    Matrix *M = newMatrix(r2 - r1, c2 - c1, 0);
    for (int i = r1; i < r2; i++) {
        for (int j = c1; j < c2; j++) {
            if (i >= r1 && j >= c1) M->values[i-r1][j-c1] = matrix->values[i][j];
        }
    }
    return M;
}

Matrix* copy(Matrix *matrix){
    Matrix *M = newMatrix(matrix->rows, matrix->columns, 0);
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->columns; j++) M->values[i][j] = matrix->values[i][j];
    }
    return M;
}

Matrix* sum(Matrix *A, Matrix *B){
    if (A->columns == B->columns && A->rows == B->rows) {
        Matrix *C = newMatrix(A->rows, A->columns, 0);
        for (int i = 0; i < A->rows; i++) {
            for (int j = 0; j < A->columns; j++) C->values[i][j] = A->values[i][j] + B->values[i][j];
        }
        return C;
    } else return NULL;
}

Matrix* minus(Matrix *A, Matrix *B){
    if (A->columns == B->columns && A->rows == B->rows) {
        Matrix *C = newMatrix(A->rows, A->columns, 0);
        for (int i = 0; i < A->rows; i++) {
            for (int j = 0; j < A->columns; j++) C->values[i][j] = A->values[i][j] - B->values[i][j];
        }
        return C;
    } else return NULL;
}

Matrix* innerMultiply(Matrix *A, Matrix *B){
    if (A->columns == B->columns && A->rows == B->rows) {
        Matrix *C = newMatrix(A->rows, A->columns, 0);
        for (int i = 0; i < A->rows; i++) {
            for (int j = 0; j < A->columns; j++) C->values[i][j] = A->values[i][j] * B->values[i][j];
        }
        return C;
    } else return NULL;
}

Matrix* scalarMultiply(Matrix* A, double scalar){
    Matrix *C = newMatrix(A->rows, A->columns, 0);
    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) C->values[i][j] = A->values[i][j] * scalar;
    }
    return C;
}

Matrix* multiply(Matrix *A, Matrix *B){
    if (A->columns == B->rows) {
        Matrix *C = newMatrix(A->rows, A->columns, 0);
        for (int i = 0; i < A->rows; i++) {
            for (int j = 0; j < A->columns; j++) {
                for (int k = 0; k < A->columns; k++) C->values[i][j] += A->values[i][k] * B->values[k][j];
            }
        }
        return C;
    } else return NULL;
}

Matrix *transpose(Matrix *matrix){
    Matrix *transpose = newMatrix(matrix->columns, matrix->rows, 0);
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->columns; j++) transpose->values[i][j] = matrix->values[j][i];
    } return transpose;
}

double trace(Matrix *matrix){
    double result = 0;
    for (int i = 0; i < (matrix->columns < matrix->rows ? matrix->columns : matrix->rows); i++) {
        result += matrix->values[i][i];
    }
    return result;
}

double det(Matrix *M){
    if (M->columns == M->rows){
        if (M->columns == 1) return M->values[0][0];
        else {
            double result = 0;
            for (int i = 0, sign = 1; i < M->rows; i++, sign *= -1) {
                Matrix *D1 = removeColumn(M, 0);
                D1 = removeRow(D1, i);
                result += M->values[i][0] * det(D1) * sign;
                freeMatrix(D1);
            }
            return result;
        }
    } else exit(EXIT_FAILURE);
}

Matrix *adjugate(Matrix *matrix){
    Matrix *M = newMatrix(matrix->rows, matrix->columns, 0);
    for (int i = 0, sign = 1; i < matrix->rows; i++, sign *= -1) {
        for (int j = 0; j < matrix->columns; j++, sign *= -1) {
            Matrix *A1 = removeColumn(matrix, j);
            A1 = removeRow(A1, i);
            M->values[i][j] = sign * det(A1);
            freeMatrix(A1);
        }
    }
    return M;
}

Matrix *inverse(Matrix *matrix){
    double determinant = det(matrix);
    if (determinant != 0 && matrix->rows == matrix->columns){
        return scalarMultiply(transpose(adjugate(matrix)), 1 / det(matrix));
    } else return NULL;
}

Matrix *solveAugmentedMatrix(Matrix *M){
    //Normalise first row
    Matrix *augmented = addColumn(M);
    for (int i = 0; i < augmented->columns; i++) augmented->values[0][i] /= augmented->values[0][0]; //TODO Case = 0

    //Change matrix to echelon form TODO Adapt to different forms (not square)
    for (int i = 0; i < augmented->columns - 1; i++) { //Column by column for echelon form
        for (int j = i+1; j < augmented->rows; j++) { //Row by row for first = 0
            double coefficient = augmented->values[j][0] >= 0 ? -1 * augmented->values[0][0] * augmented->values[j][0] : augmented->values[0][0] * augmented->values[j][0];
            for (int k = 0; k < augmented->columns; k++) { //Rest of the row
                augmented->values[j][k] += coefficient * augmented->values[0][k];
            }
        }
    }
    augmented->values[M->rows-1][M->columns] /= augmented->values[M->rows-1][M->columns-1];
    if (augmented->values[M->rows-1][M->columns] == -0) augmented->values[M->rows-1][M->columns] = 0;

    augmented->values[M->rows-1][M->columns-1] /= augmented->values[M->rows-1][M->columns-1];

    //Solve the linear system
    double *solution = malloc(M->rows * sizeof(double));
    for (int i = M->rows-1; i >= 0; i--) {
        solution[M->rows - i-1] = augmented->values[i][M->columns];
        for (int j = i; j < M->rows-1; j++) {
            solution[M->rows - i-1] -= augmented->values[i][j] * solution[M->rows - j-1];
        }
    }

    printf("Solutions :\n");
    for (int i = M->rows-1; i >= 0; i--) printf("x%d = %1.2lf\n", M->rows-1 - i, solution[i]);

    return augmented;
}

Matrix *triangularise(Matrix *matrix){ //TODO Verifications
    Matrix *P = solveAugmentedMatrix(matrix);
    Matrix *PInverse = inverse(P);
    return multiply(multiply(PInverse, matrix), P);
}

void printMatrix(Matrix *matrix){
    printf("=====================================\n");
    if (matrix != NULL) {
        for (int i = 0; i < matrix->rows; i++) {
            for (int j = 0; j < matrix->columns; j++) {
                printf("%1.1lf\t", matrix->values[i][j]);
            }
            printf("\n");
        }
    } else printf("No matrix\n");
    printf("=====================================\n");
}