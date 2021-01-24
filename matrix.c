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

void freeMatrix(Matrix *M){
    if (M != NULL) {
        for (int i = 0; i < M->rows; i++) free(M->values[i]);
        free(M->values); free(M);
        M = NULL;
    }
}

Matrix *removeRow(Matrix *M, int rowIndex){
    if (M != NULL) {
        Matrix *smallerM = newMatrix(M->rows - 1, M->columns, 0);
        for (int i = 0; i < M->rows; i++) {
            for (int j = 0; j < M->columns; j++) {
                if (i != rowIndex) smallerM->values[i > rowIndex ? i - 1 : i][j] = M->values[i][j];
            }
        }
        return smallerM;
    } else return NULL;
}

Matrix *removeColumn(Matrix *M, int columnIndex){
    if (M != NULL) {
        Matrix *smallerM = newMatrix(M->rows, M->columns - 1, 0);
        for (int i = 0; i < M->rows; i++) {
            for (int j = 0; j < M->columns; j++) {
                if (j != columnIndex) smallerM->values[i][j > columnIndex ? j - 1 : j] = M->values[i][j];
            }
        }
        return smallerM;
    } else return NULL;
}

Matrix *addColumn(Matrix *M){
    if (M != NULL) {
        Matrix *biggerM = newMatrix(M->rows, M->columns + 1, 0);
        for (int i = 0; i < M->rows; i++) {
            for (int j = 0; j < M->columns; j++) biggerM->values[i][j] = M->values[i][j];
            biggerM->values[i][M->columns] = 0;
        }
        return biggerM;
    } else return NULL;
}

Matrix *subMat(Matrix* M, int r1, int r2, int c1, int c2){
    if (M != NULL) {
        Matrix *subM = newMatrix(r2 - r1, c2 - c1, 0);
        for (int i = r1; i < r2; i++) {
            for (int j = c1; j < c2; j++) {
                if (i >= r1 && j >= c1) subM->values[i - r1][j - c1] = M->values[i][j];
            }
        }
        return subM;
    } else return NULL;
}

Matrix *copy(Matrix *M){
    if (M != NULL) {
        Matrix *copy = newMatrix(M->rows, M->columns, 0);
        for (int i = 0; i < M->rows; i++) {
            for (int j = 0; j < M->columns; j++) copy->values[i][j] = M->values[i][j];
        }
        return copy;
    } else return NULL;
}

Matrix *sum(Matrix *A, Matrix *B){
    if (A != NULL && B != NULL && A->columns == B->columns && A->rows == B->rows) {
        Matrix *C = newMatrix(A->rows, A->columns, 0);
        for (int i = 0; i < A->rows; i++) {
            for (int j = 0; j < A->columns; j++) C->values[i][j] = A->values[i][j] + B->values[i][j];
        }
        return C;
    } else return NULL;
}

Matrix *minus(Matrix *A, Matrix *B){
    if (A != NULL && B != NULL && A->columns == B->columns && A->rows == B->rows) {
        Matrix *C = newMatrix(A->rows, A->columns, 0);
        for (int i = 0; i < A->rows; i++) {
            for (int j = 0; j < A->columns; j++) C->values[i][j] = A->values[i][j] - B->values[i][j];
        }
        return C;
    } else return NULL;
}

Matrix *innerMultiply(Matrix *A, Matrix *B){
    if (A != NULL && B != NULL && A->columns == B->columns && A->rows == B->rows) {
        Matrix *C = newMatrix(A->rows, A->columns, 0);
        for (int i = 0; i < A->rows; i++) {
            for (int j = 0; j < A->columns; j++) C->values[i][j] = A->values[i][j] * B->values[i][j];
        }
        return C;
    } else return NULL;
}

Matrix *scalarMultiply(Matrix *M, double scalar){
    if (M != NULL) {
        Matrix *scalarM = newMatrix(M->rows, M->columns, 0);
        for (int i = 0; i < M->rows; i++) {
            for (int j = 0; j < M->columns; j++) scalarM->values[i][j] = M->values[i][j] * scalar;
        }
        return scalarM;
    } else return NULL;
}

Matrix *multiply(Matrix *A, Matrix *B){
    if (A != NULL && B != NULL && A->columns == B->rows) {
        Matrix *C = newMatrix(A->rows, A->columns, 0);
        for (int i = 0; i < A->rows; i++) {
            for (int j = 0; j < A->columns; j++) {
                for (int k = 0; k < A->columns; k++) C->values[i][j] += A->values[i][k] * B->values[k][j];
            }
        }
        return C;
    } else return NULL;
}

Matrix *transpose(Matrix *M){
    if (M != NULL) {
        Matrix *transpose = newMatrix(M->columns, M->rows, 0);
        for (int i = 0; i < M->rows; i++) {
            for (int j = 0; j < M->columns; j++) transpose->values[i][j] = M->values[j][i];
        }
        return transpose;
    } else return NULL;
}

double trace(Matrix *M){
    if (M != NULL) {
        double trace = 0;
        int lastDiagonal = M->columns < M->rows ? M->columns : M->rows;
        for (int i = 0; i < lastDiagonal; i++) trace += M->values[i][i];
        return trace;
    } else exit(EXIT_FAILURE);
}

double det(Matrix *M){
    if (M != NULL && M->columns == M->rows){
        if (M->columns == 1) return M->values[0][0];
        else {
            double result = 0;
            for (int i = 0, sign = 1; i < M->rows; i++, sign*=-1) {
                Matrix *subDet = removeColumn(M, 0);
                subDet = removeRow(subDet, i);
                result += M->values[i][0] * det(subDet) * sign;
                freeMatrix(subDet);
            }
            return result;
        }
    } else exit(EXIT_FAILURE);
}

Matrix *adjugate(Matrix *M){
    if (M != NULL) {
        Matrix *adjM = newMatrix(M->rows, M->columns, 0);
        for (int i = 0, sign = 1; i < M->rows; i++, sign *= -1) {
            for (int j = 0; j < M->columns; j++, sign *= -1) {
                Matrix *coFactor = removeColumn(M, j);
                coFactor = removeRow(coFactor, i);
                adjM->values[i][j] = sign * det(coFactor);
                freeMatrix(coFactor);
            }
        }
        return adjM;
    } else return NULL;
}

Matrix *inverse(Matrix *M){
    if (M != NULL) {
        double determinant = det(M);
        if (determinant != 0 && M->rows == M->columns) {
            return scalarMultiply(transpose(adjugate(M)), 1 / det(M));
        }
    } return NULL;
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

Matrix *triangularise(Matrix *M){
    if (M != NULL) {
        Matrix *PInverse, *P = solveAugmentedMatrix(M); //TODO P should be eigenvectors
        if ((PInverse = inverse(P)) != NULL) return multiply(multiply(PInverse, M), P);
    }
    return NULL;
}

void printMatrix(Matrix *M){
    printf("=====================================\n");
    if (M != NULL) {
        for (int i = 0; i < M->rows; i++) {
            for (int j = 0; j < M->columns; j++) printf("%1.1lf\t", M->values[i][j]);
            printf("\n");
        }
    } else printf("No matrix\n");
    printf("=====================================\n");
}