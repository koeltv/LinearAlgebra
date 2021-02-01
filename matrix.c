/**
 * @file matrix.c Functions on matrix
 * @author Valentin Koeltgen
 *
 * This file contain all operations on matrix
 */

#include "matrix.h"

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

Matrix *readMatrixInFile(char *link){
    FILE *currentFile;
    if ((currentFile = fopen(link, "rb")) == NULL) exit(EXIT_FAILURE);
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
        Matrix *scalarM = copy(M);
        for (int i = 0; i < M->rows; i++) {
            for (int j = 0; j < M->columns; j++) {
                scalarM->values[i][j] *= scalar;
            }
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

char isRowEmpty(Matrix *M, int index){
    int nbOfZeros = 0;
    for (int j = 0; j < M->columns; j++) if (M->values[index][j] == 0) nbOfZeros++;
    if (nbOfZeros == M->columns) return 1;
    else return 0;
}

char isColumnEmpty(Matrix *M, int index){
    int nbOfZeros = 0;
    for (int j = 0; j < M->rows; j++) if (M->values[j][index] == 0) nbOfZeros++;
    if (nbOfZeros == M->rows) return 1;
    else return 0;
}

Matrix *swapRows(Matrix *M, int firstIndex, int secondIndex){
    Matrix *swapped = copy(M);
    for (int i = 0; i < M->columns; i++) swapped->values[firstIndex][i] = M->values[secondIndex][i];
    for (int i = 0; i < M->columns; i++) swapped->values[secondIndex][i] = M->values[firstIndex][i];
    return swapped;
}

Matrix *solveAugmentedMatrix(Matrix *M){
    Matrix *toSolve = copy(M);
    //Reduce number of rows to be equal or less than the number of columns (square matrix)
    while (toSolve->rows > toSolve->columns - 1) {
        //Remove a null row if there is one, else remove the first
        int index = 0;
        for (int i = 0; i < toSolve->rows && index == 0; i++) if (isRowEmpty(M, i) == 1) index = i;
        toSolve = removeRow(toSolve, index);
    }

    //If a column only has zeros nullify a row (overdetermined)
    int nbOfEmptyColumns = 0;
    for (int i = 0; i < toSolve->columns - 1; i++) {
        if (isColumnEmpty(toSolve, i) == 1) {
            for (int j = 0; j < toSolve->rows; j++) {
                if (isRowEmpty(toSolve, j) == 1) nbOfEmptyColumns++;
                else if (j == M->rows - 1) {
                    for (int k = 0; k < toSolve->columns - 1; k++) toSolve->values[j][k] = 0;
                    nbOfEmptyColumns++;
                }
            }
        }
    }

    int nbOfEmptyRows = 0;
    for (int i = 0; i < toSolve->rows; i++) if (isRowEmpty(toSolve, i) == 1) nbOfEmptyRows++;
    
    //Swap rows to have the null ones at the bottom
    for (int i = 0, lastNullLine = toSolve->rows-1; i < toSolve->rows - 1; i++) {
        if (isRowEmpty(M, i) == 1) toSolve = swapRows(toSolve, i, lastNullLine--);
        else if (toSolve->values[i][i] == 0) {
            for (int j = i + 1; j < toSolve->rows; j++) {
                if (toSolve->values[j][i] != 0) {
                    toSolve = swapRows(toSolve, i, j); break;
                }
            }
        }
    }

    //Change matrix to echelon form
    for (int i = 0; i < toSolve->rows - nbOfEmptyColumns && i < toSolve->rows - nbOfEmptyRows; i++) {
        //Normalise the current row
        double normaliseValue = toSolve->values[i][i + nbOfEmptyColumns];
        for (int j = i + nbOfEmptyColumns; j < toSolve->columns; j++) {
            toSolve->values[i][j] /= normaliseValue;
            if (toSolve->values[i][j] == -0) toSolve->values[i][j] = 0;
        }
        //Subtract the current row to the next ones
        for (int j = i + 1; j < toSolve->rows - nbOfEmptyColumns && j < toSolve->rows - nbOfEmptyRows; j++) {
            double coefficient = -1 * toSolve->values[i][i + nbOfEmptyColumns] * toSolve->values[j][i + nbOfEmptyColumns];
            for (int k = i + nbOfEmptyColumns; k < toSolve->columns; k++) toSolve->values[j][k] += coefficient * toSolve->values[i][k];
        }
    }
    return toSolve;
}

Solutions *eigenValues(Matrix *M){
    if (M != NULL){
        char *stringForm = detOfStringMatrix(changeToPLambdaForm(toStringMatrix(M)));
        return solve(stringToPolynomial(stringForm, 0, length(stringForm) + 1));
    } else return NULL;
}

char orthogonal(Matrix *M1, Matrix *M2){
    if (M1->rows == M2->rows){
        Matrix *toSolve = M1;
        for (int i = 0; i < M2->columns; i++) {
            toSolve = addColumn(toSolve);
            for (int j = 0; j < M2->rows; j++) toSolve->values[M1->columns + i][j] = M2->values[i][j];
        }
        Matrix *result = solveAugmentedMatrix(toSolve);
        for (int i = 0; i < result->rows; i++) if (result->values[i][result->columns-1] != 0) return 0;
        return 1;
    } else return -1;
}

Matrix *completeOrthogonal(Matrix *M){
    if (M->columns == M->rows) return M;
    else {
        Matrix *completed = M;
        for (int i = M->columns; i < M->rows; i++) {
            completed = addColumn(completed);
            for (int j = 0; j < M->rows; j++) {
                //Create basic vector
                Matrix *orthogonalVector = newMatrix(M->rows, 1, 0);
                orthogonalVector->values[j][0] = 1;
                //Check if orthogonal with the other vectors
                if (orthogonal(M, orthogonalVector) == 1) {
                    for (int k = 0; k < M->rows; k++) M->values[k][M->columns] = orthogonalVector->values[k][0];
                    break;
                }
            }
        }
        return completed;
    }
}

//Solutions *solveForValues(Matrix *M);

Matrix *solveForVectors(Matrix *M){
    Matrix **v = malloc((M->columns - 1) * sizeof(Matrix*));
    Matrix *N = copy(M);

    //If we have an empty column, we move the empty row associated to the unrestricted value to its row
    for (int i = 0; i < N->columns - 1; i++) {
        if (isColumnEmpty(N, i) == 1) {
            for (int j = 0; j < N->rows; j++) { //find a null row
                if (isRowEmpty(N, j) == 1) {
                    for (int k = i; k < j; k++) N = swapRows(N, k, j); break;
                }
            }
        }
    }

//    If a row as only one value, nullify it
//    for (int i = 0, nbOfZeros = 0; i < M->rows; i++, nbOfZeros = 0) {
//        for (int j = 0; j < M->columns - 1; j++) if (N->values[i][j] == 0) nbOfZeros++;
//        if (nbOfZeros >= N->columns-2) for (int j = 0; j < N->columns - 1; j++) N->values[i][j] = 0;
//    }

    //Initialise and search for unrestricted values (only if 0 = 0)
    for (int i = 0; i < N->rows; i++) {
        v[i] = newMatrix(N->columns - 1, 1, 0);
        if (isRowEmpty(N, i) == 1) v[i]->values[i][0] = 1;
    }

    //Calculate vectors in function of the others
    for (int i = N->rows - 1; i >= 0; i--) {
        if (isRowEmpty(N, i) != 1){
            for (int j = N->columns - 2; j >= 0; j--) {
                if (j != i) v[i] = minus(v[i], scalarMultiply(v[j], N->values[i][j]));
            }
            v[i] = scalarMultiply(v[i], 1/N->values[i][i]);
        }
    }

    //Reforming the matrix by picking the rows
    Matrix *output = newMatrix(N->columns - 1, 1, 0);
    int currentIndex = 0;
    for (int i = 0; i < N->columns - 1; i++) {
        //If the i-th value of the vectors are null ignore them, else create a vector from them
        int atLeastAValueAtIndex = 0;
        for (int j = 0; j < N->columns - 1 && atLeastAValueAtIndex == 0; j++) if (v[j]->values[i][0] != 0) atLeastAValueAtIndex++;
        //If the line isn't null
        if (atLeastAValueAtIndex > 0) {
            if (currentIndex > 0) output = addColumn(output);
            //Copy the values of this line for each vector
            for (int j = 0; j < N->rows; j++) output->values[j][currentIndex] = v[j]->values[i][0];
            currentIndex++;
        }
    }
    return output;
}

Matrix *eigenVectors(Matrix *M){
    if (M->columns == M->rows) {
        Solutions *eigValues = eigenValues(M);
        Matrix *eigenMatrix = newMatrix(M->rows, 1, 0);

        int nbVectors = 0;
        for (int n = 0; n < eigValues->size; n++) {
            char sameEigenValue = 0;
            for (int i = 0; i < n; i++) if (eigValues->values[i] == eigValues->values[n]) sameEigenValue++;
            if (sameEigenValue < 1) {
                Matrix *toSolve = copy(M);
                for (int j = 0; j < M->columns; j++) toSolve->values[j][j] -= eigValues->values[n];
                Matrix *vectors = solveForVectors(solveAugmentedMatrix(addColumn(toSolve)));
                freeMatrix(toSolve);
                for (int i = 0; i < vectors->columns; i++) {
                    if (nbVectors != 0) eigenMatrix = addColumn(eigenMatrix);
                    for (int j = 0; j < vectors->rows; j++) eigenMatrix->values[j][nbVectors] = vectors->values[j][i];
                    printf("Iteration %d of vector %d (%d Columns) :\n", i, n, vectors->columns);
                    printMatrix(eigenMatrix);
                    nbVectors++;
                }
                freeMatrix(vectors);
            }
        }
        return completeOrthogonal(eigenMatrix);
    } else return NULL;
}

Matrix *triangularise(Matrix *M){
    if (M != NULL) {
        Matrix *PInverse, *P = eigenVectors(M);
        if ((PInverse = inverse(P)) != NULL) return multiply(multiply(PInverse, M), P);
    }
    return NULL;
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