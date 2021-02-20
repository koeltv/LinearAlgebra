/**
 * @file stringInteractions.c Functions on strings
 * @author Valentin Koeltgen
 *
 * This file contain all operations on strings
 */

#include "stringInteractions.h"

char *readString(FILE *current) {
    int i = 0;
    char temp, *string = NULL;
    fscanf(current, " %c", &temp);
    while (!feof(current) && temp >= ' ' && temp <= '~'){
        string = realloc (string, (i + 2) * sizeof(char));
        string[i] = temp;
        fscanf(current, "%c", &temp);
        i++;
    } string[i] = '\0';
    return string;
}

int length(const char *string) {
    int length = 0;
    while (string[length] != '\0') length++;
    return length;
}

char *firstWord(const char *string) {
    int firstLetterIndex = 0, k = 0;
    while (string[firstLetterIndex] == ' ') firstLetterIndex++;
    char *word = NULL;
    for (int i = firstLetterIndex; string[i] && string[i] != ' ' && string[i] != '='; i++, k++) {
        word = realloc(word, (i - firstLetterIndex + 1) * sizeof(char));
        word[k] = string[i];
    } word[k] = '\0';
    return word;
}

char shorterString(const char *string1, const char *string2) {
    if (string1 && string2) {
        int i;
        for (i = 0; string1[i] && string2[i]; i++) {
            if (string1[i] < string2[i]) return 1;
            else if (string1[i] > string2[i]) return 2;
        }
        if (!string1[i] && !string2[i]) return 0;
        else if (!string1[i]) return 1;
        else return 2;
    } else return -1;
}

char containString(const char *mainString, const char *toSearch) {
    int i = 0, j;
    while (mainString[i] && mainString[i] != toSearch[0]) i++;
    if (mainString[i]){
        for (j = 0; mainString[i+j] && toSearch[j]; j++) {
            if (mainString[i+j] != toSearch[j]) return 0;
        }
        if (!toSearch[j]) return 1;
    }
    return 0;
}

char containCharInOrder(const char *string, const char *charToSearch) {
    int totalOfCharToFind = length(charToSearch);
    for (int i = 0, nbOfCharFound = 0; string[i]; i++) {
        if (string[i] == charToSearch[nbOfCharFound]) nbOfCharFound++;
        if (nbOfCharFound == totalOfCharToFind) return 1;
    }
    return 0;
}

char *extractBetweenChar(const char *string, char first, char last) {
    int firstIndex = 0; char *extracted = NULL;
    while (string[firstIndex] && string[firstIndex] != first) firstIndex++;
    if (string[firstIndex]) {
        int j = 0; firstIndex++;
        for (int nbOfParenthesis = 0; string[firstIndex] && (string[firstIndex] != last || nbOfParenthesis > 0); firstIndex++) {
            if (string[firstIndex] == '(') nbOfParenthesis++;
            else if (string[firstIndex] == ')') nbOfParenthesis--;
            extracted = realloc(extracted, ++j * sizeof(char));
            extracted[j - 1] = string[firstIndex];
        } extracted[j] = '\0';
    }
    return extracted;
}

char *extractUpToIndex(const char *string, int last) {
    char *extracted = NULL; int k = 0;
    for (int j = 0; string[j] && j < last; j++) {
        extracted = realloc(extracted, (j + 1) * sizeof(char));
        extracted[k++] = string[j];
    } extracted[k] = '\0';
    return extracted;
}

char *extractBetweenIndexes(const char *string, int first, int last) {
    if (string && length(string) >= last - first) {
        char *result = calloc(last - first + 2, sizeof(char));
        for (int i = first; string[i] && i < last; i++) result[i - first] = string[i];
        result[last - first] = string[last];
        return result;
    } else return NULL;
}

char onlyContainValue(const char *string) {
    for (int i = 0; string[i]; i++) {
        if (string[i] != '+' && string[i] != '-' && string[i] != '.' && string[i] != ' ' &&
            (string[i] < '0' || string[i] > '9'))
            return 0;
    }
    return 1;
}

char isValidPolynomial(const char *string) {
    int index = 0;
    while (string[index] && string[index] != 'X') index++;
    if (string[index] == 'X') {
        if ((string[index + 1] == '^' && string[index + 2] >= '0' && string[index + 2] <= '9') ||
            (!string[index + 1] || string[index + 1] == ' ')) {
            return 1;
        }
    }
    return 0;
}

char everythingIsBetweenParenthesis(const char *string) {
    for (int i = 0, nbOfParenthesis = 0; string[i]; i++) {
        if (string[i] == '(') nbOfParenthesis++;
        else if (string[i] == ')') nbOfParenthesis--;
        else if (string[i] != ' ' && nbOfParenthesis < 1) return 0;
    }
    return 1;
}

char operatorWithoutDepth(const char *string) {
    int i = 0;
    while (string[i] == ' ' || string[i] == '+' || string[i] == '-') i++;
    for (int nbOfParenthesis = 0; string[i]; i++) {
        if (string[i] == '(' || string[i] == '[') nbOfParenthesis++;
        else if (string[i] == ')' || string[i] == ']') nbOfParenthesis--;
        else if (nbOfParenthesis < 1 && (string[i] == '+' || string[i] == '-' || string[i] == '*' || string[i] == '/')) return 1;
    }
    return 0;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "LoopDoesntUseConditionVariableInspection"
void nextOperator(const char *string, int *firstIndex, int *secondIndex) {
    if (string[*firstIndex] && string[*firstIndex] != '(') (*firstIndex)++;
    int temp = 0, firstNonWhiteSpace = 0;
    while (string[firstNonWhiteSpace] && string[firstNonWhiteSpace] == ' ') firstNonWhiteSpace++;
    for (int nbOfParenthesis = 0; string[*firstIndex]; (*firstIndex)++) {
        if (string[*firstIndex] == '(') nbOfParenthesis++;
        else if (string[*firstIndex] == ')') nbOfParenthesis--;
        else if (nbOfParenthesis < 1 && *firstIndex > firstNonWhiteSpace) {
            if (string[*firstIndex] == '*' || string[*firstIndex] == '/') temp = *firstIndex;
            else if (string[*firstIndex] == '+' || string[*firstIndex] == '-') break;
        }
    }
    if (!string[*firstIndex] && temp) *firstIndex = temp;

    int i = *firstIndex + 1;
    for (int nbOfParenthesis = 0; string[*firstIndex] && string[i]; i++) {
        if (string[i] == '(') nbOfParenthesis++;
        else if (string[i] == ')') nbOfParenthesis--;
        else if (nbOfParenthesis < 1 && (string[i] == '+' || string[i] == '-')) {
            *secondIndex = i - 1; break;
        }
    }
    if (!string[i]) *secondIndex = i;
}

double readDoubleInString(const char *string, int *position) {
    int temp = 0;
    if (!position) position = &temp;
    //Search for the sign
    while (*position >= 0 && string[*position] == ' ') (*position)--;
    while (string[*position] && (string[*position] < '0' || string[*position] > '9') && string[*position] != '-') (*position)++;
    //Read the sign
    int sign = 1;
    if (string[*position] == '-') sign = -1;
    while (string[*position] == ' ' || string[*position] == '+' || string[*position] == '-') (*position)++;
    //Read the value
    double result = 0;
    do {
        if (string[*position] >= '0' && string[*position] <= '9') result = result * 10 + string[(*position)++] - '0';
        else if (string[(*position)++] == '.') {
            double decimal = 0.1;
            do {
                result += (string[(*position)++] - '0') * decimal;
                decimal /= 10;
            } while (string[(*position)] >= '0' && string[(*position)] <= '9');
        }
    } while (string[*position] == '.' || (string[*position] >= '0' && string[*position] <= '9'));
    return result * sign;
}
#pragma clang diagnostic pop

void printFileContent(const char *link, FILE *output) {
    FILE *input = fopen(link, "rb");
    if (input) {
        while (!feof(input)) fprintf(output, "%c", getc(input));
        fclose(input);
    } else fprintf(stderr, "File was not found at %s", link);
}

StringMatrix newStringMatrix(int nbRows, int nbColumns) {
    StringMatrix M = {NULL, malloc(nbRows * sizeof(char**)), nbRows, nbColumns};
    for (int i = 0; i < M.rows; i++) {
        M.values[i] = malloc(M.columns * sizeof(char*));
        for (int j = 0; j < M.columns; j++) M.values[i][j] = NULL;
    }
    return M;
}

StringMatrix removeSRow(StringMatrix M, int rowIndex) {
    StringMatrix smallerM = newStringMatrix(M.rows - 1, M.columns);
    for (int i = 0; i < M.rows; i++) {
        for (int j = 0; j < M.columns; j++) {
            if (i != rowIndex) smallerM.values[i > rowIndex ? i - 1 : i][j] = M.values[i][j];
        }
    }
    return smallerM;
}

StringMatrix removeSColumn(StringMatrix M, int columnIndex) {
    StringMatrix smallerM = newStringMatrix(M.rows, M.columns - 1);
    for (int i = 0; i < M.rows; i++) {
        for (int j = 0; j < M.columns; j++) {
            if (j != columnIndex) smallerM.values[i][j > columnIndex ? j - 1 : j] = M.values[i][j];
        }
    }
    return smallerM;
}

StringMatrix changeToPLambdaForm(StringMatrix M) {
    for (int i = 0; i < M.rows; i++) {
        snprintf(M.values[i][i], (length(M.values[i][i]) + 6) * sizeof(char), "%s - 1X", M.values[i][i]);
    }
    return M;
}

char *detOfStringMatrix(StringMatrix M) {
    if (M.rows == 1 && M.columns == 1) return M.values[0][0];
    else {
        char *result = NULL;
        for (int i = 0, sign = 1; i < M.rows; i++, sign *= -1) {
            char *detOfSubDet = detOfStringMatrix(removeSRow(removeSColumn(M, 0), i));
            int totalSize = length(M.values[i][0]) + length(detOfSubDet);
            
            if (!result) {
                if (sign == 1) {
                    totalSize += 8;
                    result = realloc(result, totalSize * sizeof(char));
                    snprintf(result, totalSize * sizeof(char), "(%s) * (%s)", M.values[i][0], detOfSubDet);
                } else {
                    totalSize += 14;
                    result = realloc(result, totalSize * sizeof(char));
                    snprintf(result, totalSize * sizeof(char), "(%s) * (-1) * (%s)", M.values[i][0], detOfSubDet);
                }
            } else {
                if (sign == 1) {
                    totalSize += 14 + length(result);
                    result = realloc(result, totalSize * sizeof(char));
                    snprintf(result, totalSize * sizeof(char), "%s + (%s) * (%s)", result, M.values[i][0], detOfSubDet);
                } else {
                    totalSize += 21 + length(result);
                    result = realloc(result, totalSize * sizeof(char));
                    snprintf(result, totalSize * sizeof(char), "%s + (%s) * (-1) * (%s)", result, M.values[i][0], detOfSubDet);
                }
            }
        }
        return result;
    }
}