/**
 * @file matrix.c Functions on matrix
 * @author Valentin Koeltgen
 *
 * This file contain all operations on matrix
 */

#include "matrix.h"

Matrix readMatrixInString(char *string) {
    //Creation of the matrix
    Matrix matrix = {NULL, NULL, 1, 1};
    for (int i = 0; string[i] != ']' && string[i] != '\0'; i++) {
        if (matrix.rows == 1 && string[i] == ',') matrix.columns++;
        else if (string[i] == ';') matrix.rows++;
    }

    //Initialisation of the matrix
    int startOfMatrix = 0;
    while (string[startOfMatrix] != '[') startOfMatrix++;

    matrix.values = (double**) malloc(matrix.rows * sizeof(double*));
    for (int k = 0; k < matrix.rows; k++) {
        matrix.values[k] = (double*) malloc(matrix.columns * sizeof(double));
        for (int j = 0; j < matrix.columns; j++) matrix.values[k][j] = readDoubleInString(string, &startOfMatrix);
    }
    return matrix;
}

Matrix newMatrix(int nbRows, int nbColumns, double initialValue) {
    if (nbRows < 1 || nbColumns < 1) return (Matrix) {NULL, NULL, nbRows, nbColumns};
    else {
        Matrix M = {NULL, malloc(nbRows * sizeof(double *)), nbRows, nbColumns};
        for (int i = 0; i < M.rows; i++) {
            M.values[i] = malloc(M.columns * sizeof(double));
            for (int j = 0; j < M.columns; j++) M.values[i][j] = initialValue;
        }
        return M;
    }
}

void freeMatrix(Matrix *M) {
    if (M) {
        for (int i = 0; i < M->rows; i++) free(M->values[i]);
        free(M->values);
    }
}

Matrix copyMatrix(Matrix M) {
    Matrix copy = newMatrix(M.rows, M.columns, 0);
    for (int i = 0; i < M.rows; i++) {
        for (int j = 0; j < M.columns; j++) {
            copy.values[i][j] = M.values[i][j];
        }
    }
    return copy;
}

Matrix removeRow(Matrix M, int rowIndex) {
    Matrix smallerM = newMatrix(M.rows - 1, M.columns, 0);
    for (int i = 0; i < M.rows; i++) {
        for (int j = 0; j < M.columns; j++) {
            if (i != rowIndex) smallerM.values[i > rowIndex ? i - 1 : i][j] = M.values[i][j];
        }
    }
    return smallerM;
}

Matrix removeColumn(Matrix M, int columnIndex) {
    Matrix smallerM = newMatrix(M.rows, M.columns - 1, 0);
    for (int i = 0; i < M.rows; i++) {
        for (int j = 0; j < M.columns; j++) {
            if (j != columnIndex) smallerM.values[i][j > columnIndex ? j - 1 : j] = M.values[i][j];
        }
    }
    return smallerM;
}

Matrix addColumn(Matrix M) {
    Matrix biggerM = newMatrix(M.rows, M.columns + 1, 0);
    for (int i = 0; i < M.rows; i++) {
        for (int j = 0; j < M.columns; j++) biggerM.values[i][j] = M.values[i][j];
        biggerM.values[i][M.columns] = 0;
    }
    return biggerM;
}

Matrix subMat(Matrix M, int r1, int r2, int c1, int c2) {
    Matrix subM = newMatrix(r2 - r1, c2 - c1, 0);
    for (int i = r1; i < r2; i++) {
        for (int j = c1; j < c2; j++) {
            if (i >= r1 && j >= c1) subM.values[i - r1][j - c1] = M.values[i][j];
        }
    }
    return subM;
}

Matrix sum(Matrix A, Matrix B) {
    if (A.columns == B.columns && A.rows == B.rows) {
        Matrix C = newMatrix(A.rows, A.columns, 0);
        for (int i = 0; i < A.rows; i++) {
            for (int j = 0; j < A.columns; j++) C.values[i][j] = A.values[i][j] + B.values[i][j];
        }
        return C;
    } else return nullMatrix;
}

Matrix minus(Matrix A, Matrix B) {
    if (A.columns == B.columns && A.rows == B.rows) {
        Matrix C = newMatrix(A.rows, A.columns, 0);
        for (int i = 0; i < A.rows; i++) {
            for (int j = 0; j < A.columns; j++) C.values[i][j] = A.values[i][j] - B.values[i][j];
        }
        return C;
    } else return nullMatrix;
}

Matrix scalarMultiply(Matrix M, double scalar) {
    Matrix copy = copyMatrix(M);
    for (int i = 0; i < copy.rows; i++) {
        for (int j = 0; j < copy.columns; j++) {
            copy.values[i][j] *= scalar;
        }
    }
    return copy;
}

Matrix multiply(Matrix A, Matrix B) {
    if (A.columns == B.rows) {
        Matrix C = newMatrix(A.rows, B.columns, 0);
        for (int i = 0; i < A.rows; i++) {
            for (int j = 0; j < B.columns; j++) {
                for (int k = 0; k < A.columns; k++) C.values[i][j] += A.values[i][k] * B.values[k][j];
            }
        }
        return C;
    } else return nullMatrix;
}

Matrix transpose(Matrix M) {
    Matrix transpose = newMatrix(M.columns, M.rows, 0);
    for (int i = 0; i < M.rows; i++) {
        for (int j = 0; j < M.columns; j++) transpose.values[j][i] = M.values[i][j];
    }
    return transpose;
}

void printMatrix(Matrix M) {
    if (M.name) printf("%s =\n", M.name);
    for (int i = 0; i < M.rows; i++) {
        printf("\t");
        for (int j = 0; j < M.columns; j++) printf("%1.1lf\t", M.values[i][j]);
        printf("\n");
    }
}

double trace(Matrix M) {
    double trace = 0;
    int lastDiagonal = M.columns < M.rows ? M.columns : M.rows;
    for (int i = 0; i < lastDiagonal; i++) trace += M.values[i][i];
    return trace;
}

double det(Matrix M) {
    if (M.columns == M.rows) {
        if (M.columns == 1) return M.values[0][0];
        else {
            double result = 0;
            for (int i = 0, sign = 1; i < M.rows; i++, sign *= -1) {
                Matrix subDet = removeColumn(M, 0);
                subDet = removeRow(subDet, i);
                result += M.values[i][0] * det(subDet) * sign;
            }
            return result;
        }
    } else return IMAGINARY;
}

Matrix adjugate(Matrix M) {
    Matrix adjM = newMatrix(M.rows, M.columns, 0);
    if (M.rows == M.columns) {
        if (M.rows == 1) adjM.values[0][0] = M.values[0][0];
        else {
            for (int i = 0, sign = 1; i < M.rows; i++, sign *= -1) {
                for (int j = 0; j < M.columns; j++, sign *= -1) {
                    Matrix coFactor = removeColumn(M, j);
                    coFactor = removeRow(coFactor, i);
                    adjM.values[i][j] = sign * det(coFactor);
                }
            }
        }
    }
    return adjM;
}

Matrix inverse(Matrix M) {
    double determinant = det(M);
    if (determinant != 0 && M.rows == M.columns) {
        return scalarMultiply(transpose(adjugate(M)), 1 / determinant);
    } else return nullMatrix;
}

char isRowEmpty(Matrix M, int index) {
    int nbOfZeros = 0;
    for (int j = 0; j < M.columns; j++) if (M.values[index][j] == 0) nbOfZeros++;
    if (nbOfZeros == M.columns) return 1;
    else return 0;
}

char isColumnEmpty(Matrix M, int index) {
    int nbOfZeros = 0;
    for (int j = 0; j < M.rows; j++) if (M.values[j][index] == 0) nbOfZeros++;
    if (nbOfZeros == M.rows) return 1;
    else return 0;
}

Matrix swapRows(Matrix M, int firstIndex, int secondIndex) {
    Matrix swapped = M;
    for (int i = 0; i < M.columns; i++) swapped.values[firstIndex][i] = M.values[secondIndex][i];
    for (int i = 0; i < M.columns; i++) swapped.values[secondIndex][i] = M.values[firstIndex][i];
    return swapped;
}

Matrix solveAugmentedMatrix(Matrix M) {
    //Reduce number of rows to be equal or less than the number of columns (square matrix)
    while (M.rows > M.columns - 1) {
        //Remove a null row if there is one, else remove the first
        int index = 0;
        for (int i = 0; i < M.rows && index == 0; i++) if (isRowEmpty(M, i) == 1) index = i;
        M = removeRow(M, index);
    }

    //If a column only has zeros nullify a row (overdetermined)
    int nbOfEmptyColumns = 0;
    for (int i = 0; i < M.columns - 1; i++) {
        if (isColumnEmpty(M, i) == 1) {
            for (int j = 0; j < M.rows; j++) {
                if (isRowEmpty(M, j) == 1) nbOfEmptyColumns++;
                else if (j == M.rows - 1) {
                    for (int k = 0; k < M.columns - 1; k++) M.values[j][k] = 0;
                    nbOfEmptyColumns++;
                }
            }
        }
    }

    int nbOfEmptyRows = 0;
    for (int i = 0; i < M.rows; i++) if (isRowEmpty(M, i) == 1) nbOfEmptyRows++;
    
    //Swap rows to have the null ones at the bottom
    for (int i = 0, lastNullLine = M.rows - 1; i < M.rows - 1; i++) {
        if (isRowEmpty(M, i) == 1) M = swapRows(M, i, lastNullLine--);
        else if (M.values[i][i] == 0) {
            for (int j = i + 1; j < M.rows; j++) {
                if (M.values[j][i] != 0) {
                    M = swapRows(M, i, j); break;
                }
            }
        }
    }

    //Change matrix to echelon form
    for (int i = 0; i < M.rows - nbOfEmptyColumns && i < M.rows - nbOfEmptyRows; i++) {
        //Normalise the current row
        double normaliseValue = M.values[i][i + nbOfEmptyColumns];
        if (normaliseValue != 0) {
            for (int j = i + nbOfEmptyColumns; j < M.columns; j++) {
                M.values[i][j] /= normaliseValue;
                if (M.values[i][j] == -0) M.values[i][j] = 0;
            }
        }
        //Subtract the current row to the next ones
        for (int j = i + 1; j < M.rows - nbOfEmptyColumns && j < M.rows - nbOfEmptyRows; j++) {
            double coefficient = -1 * M.values[i][i + nbOfEmptyColumns] * M.values[j][i + nbOfEmptyColumns];
            for (int k = i + nbOfEmptyColumns; k < M.columns; k++) {
                M.values[j][k] += coefficient * M.values[i][k];
            }
        }
    }
    return M;
}

Solutions *eigenValues(Matrix M) {
    char *stringForm = detOfStringMatrix(changeToPLambdaForm(toStringMatrix(M)));
    return solve(stringToPolynomial(stringForm, 0, length(stringForm)));
}

char orthogonal(Matrix M1, Matrix M2) {
    if (M1.rows == M2.rows){
        Matrix toSolve = M1;
        for (int i = 0; i < M2.columns; i++) {
            toSolve = addColumn(toSolve);
            for (int j = 0; j < M2.rows; j++) toSolve.values[M1.columns + i][j] = M2.values[i][j];
        }
        Matrix result = solveAugmentedMatrix(toSolve);
        for (int i = 0; i < result.rows; i++) if (result.values[i][result.columns-1] != 0) return 0;
        return 1;
    } else return -1;
}

Matrix completeOrthogonal(Matrix M) {
    if (M.columns == M.rows) return M;
    else {
        Matrix completed = M;
        for (int i = M.columns; i < M.rows; i++) {
            completed = addColumn(completed);
            for (int j = 0; j < M.rows; j++) {
                //Create basic vector
                Matrix orthogonalVector = newMatrix(M.rows, 1, 0);
                orthogonalVector.values[j][0] = 1;
                //Check if orthogonal with the other vectors
                if (orthogonal(M, orthogonalVector) == 1) {
                    for (int k = 0; k < M.rows; k++) M.values[k][M.columns] = orthogonalVector.values[k][0];
                    break;
                }
            }
        }
        return completed;
    }
}

Matrix solveForVectors(Matrix M){
    Matrix *v = malloc((M.columns - 1) * sizeof(Matrix));
    //If we have an empty column, we move the empty row associated to the unrestricted value to its row
    for (int i = 0; i < M.columns - 1; i++) {
        if (isColumnEmpty(M, i) == 1) {
            for (int j = 0; j < M.rows; j++) { //find a null row
                if (isRowEmpty(M, j) == 1) {
                    for (int k = i; k < j; k++) M = swapRows(M, k, j); break;
                }
            }
        }
    }
    //Initialise and search for unrestricted values (only if 0 = 0)
    for (int i = 0; i < M.rows; i++) {
        v[i] = newMatrix(M.columns - 1, 1, 0);
        if (isRowEmpty(M, i) == 1) v[i].values[i][0] = 1;
    }
    //Calculate vectors in function of the others
    for (int i = M.rows - 1; i >= 0; i--) {
        if (isRowEmpty(M, i) != 1){
            for (int j = M.columns - 2; j >= 0; j--) {
                if (j != i) v[i] = minus(v[i], scalarMultiply(copyMatrix(v[j]), M.values[i][j]));
            }
            v[i] = scalarMultiply(v[i], 1 / M.values[i][i]);
        }
    }
    //Reforming the matrix by picking the rows
    Matrix output = newMatrix(M.columns - 1, 1, 0);
    int currentIndex = 0;
    for (int i = 0; i < M.columns - 1; i++) {
        //If the i-th value of the vectors are null ignore them, else create a vector from them
        int atLeastAValueAtIndex = 0;
        for (int j = 0; j < M.columns - 1 && atLeastAValueAtIndex == 0; j++) if (v[j].values[i][0] != 0) atLeastAValueAtIndex++;
        //If the line isn't null
        if (atLeastAValueAtIndex > 0) {
            if (currentIndex > 0) output = addColumn(output);
            //Copy the values of this line for each vector
            for (int j = 0; j < M.rows; j++) output.values[j][currentIndex] = v[j].values[i][0];
            currentIndex++;
        }
    }
    return output;
}

Matrix eigenVectors(Matrix M) {
    if (M.columns == M.rows) {
        Solutions *eigValues = eigenValues(M);
        if (eigValues) {
            Matrix eigenMatrix = newMatrix(M.rows, 1, 0);
            int nbVectors = 0;
            for (int n = 0; n < eigValues->size; n++) {
                char sameEigenValue = 0;
                for (int i = 0; i < n; i++) if (eigValues->values[i] == eigValues->values[n]) sameEigenValue++;
                if (sameEigenValue < 1) {
                    Matrix toSolve = copyMatrix(M);
                    for (int j = 0; j < M.columns; j++) toSolve.values[j][j] -= eigValues->values[n];
                    Matrix vector = solveForVectors(solveAugmentedMatrix(addColumn(toSolve)));
                    freeMatrix(&toSolve);
                    for (int i = 0; i < vector.columns; i++) {
                        if (nbVectors != 0) eigenMatrix = addColumn(eigenMatrix);
                        for (int j = 0; j < vector.rows; j++) eigenMatrix.values[j][nbVectors] = vector.values[j][i];
                        printf("Iteration %d of vector %d (%d Columns) :\n", i, n, vector.columns);
                        printMatrix(eigenMatrix);
                        nbVectors++;
                    }
                }
            }
            return completeOrthogonal(eigenMatrix);
        }
    }
    return nullMatrix;
}

Matrix triangularise(Matrix M) {
    Matrix PInverse, P = eigenVectors(M);
    PInverse = inverse(P);
    if (PInverse.values != NULL) return multiply(multiply(PInverse, M), P);
    else return nullMatrix;
}

StringMatrix toStringMatrix(Matrix M) {
    StringMatrix toString = {NULL, malloc(M.rows * sizeof(char**)), M.rows, M.columns};
    for (int i = 0; i < M.rows; i++) {
        toString.values[i] = malloc(toString.columns * sizeof(char*));
        for (int j = 0; j < M.columns; j++) {
            toString.values[i][j] = malloc(20 * sizeof(char));
            snprintf(toString.values[i][j], 20 * sizeof(char), "%lf", M.values[i][j]);
        }
    }
    return toString;
}