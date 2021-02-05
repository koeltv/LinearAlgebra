/**
 * @file main.h Header file of main.c
 * @author Valentin Koeltgen
 */

#ifndef LINEARALGEBRA_MAIN_H
#define LINEARALGEBRA_MAIN_H

#include "register.h"

Register *applyOperation(Register *mainRegister, char *operation, char operator);

Register *recursiveCommandDecomposition(Register *mainRegister, char *command);

#endif //LINEARALGEBRA_MAIN_H