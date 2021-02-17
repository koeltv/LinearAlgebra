/**
 * @file main.h Header file of main.c
 * @author Valentin Koeltgen
 */

#ifndef LINEARALGEBRA_MAIN_H
#define LINEARALGEBRA_MAIN_H

#include "register.h"

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Commands interactions
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

/**
 * Read and apply a script file
 * This function read a given file and apply the commands in it line by line
 * @param link - link of the file in string format
 */
void readScriptFile(char *link);

/**
 * Apply a mathematical operation
 * This function apply a simple mathematical operation (sum, difference, multiplication, division) on 2 objects (polynomial, matrix or variable)
 * @param leftOperand - Left part of the operation
 * @param rightOperand - Right part of the operation
 * @param operator - The operation to apply
 * @return object containing the result
 */
Object applyOperation(Object leftOperand, char operator, Object rightOperand);

/**
 * Get a basic object from a string
 * This function recuperate a basic object (existing or created)
 * @param command - object in string format
 * @return object containing the object
 */
Object extractObject(char *command);

/**
 * Apply a composite command recursively
 * This function apply a command by reducing to basic object and combining them
 * @param command - command in string form
 * @return object containing the result of the command
 */
Object recursiveCommandDecomposition(char *command);

/**
 * Apply any command
 * This function is the first called when reading a command, it can take simple and composite commands, see readme.md for more details
 * @param command - The command to execute
 */
void executeCommand(char *command);

#endif //LINEARALGEBRA_MAIN_H