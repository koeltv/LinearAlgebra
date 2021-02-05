/**
 * @file main.h Header file of main.c
 * @author Valentin Koeltgen
 */

#ifndef LINEARALGEBRA_MAIN_H
#define LINEARALGEBRA_MAIN_H

#include "register.h"

/**
 * (WIP) Apply a mathematical operation
 * This function apply a simple mathematical operation (sum, difference, multiplication) on 2 objects (polynomials or matrices)
 * @param mainRegister - The register to search for existing objects
 * @param operation - Operation to do in string format
 * @param operator - The operation to apply
 * @return register containing the result
 */
Register *applyOperation(Register *mainRegister, char *operation, char operator);

/**
 * Get a basic object from a string
 * This function recuperate a basic object (existing or created)
 * @param mainRegister - The register to search for existing objects
 * @param command - object in string format
 * @return register containing the object
 */
Register *extractObject(Register *mainRegister, char *command);

/**
 * Apply a composite command recursively
 * This function apply a command by reducing to basic object and combining them
 * @param mainRegister - The register to search for existing objects
 * @param command - command in string form
 * @return register containing the result of the command
 */
Register *recursiveCommandDecomposition(Register *mainRegister, char *command);

/**
 * Print the content of a file in a buffer
 * This function print the content of the file at the given link in the output buffer without modifying it
 * @param link - Link of the file to read
 * @param output - Buffer where the content while be outputted
 */
void printFileContent(char *link, FILE *output);

/**
 * Apply any command
 * This function is the first called when reading a command, it can take simple and composite commands, see readme.md for more details
 * @param mainRegister
 * @param command
 */
void executeCommand(Register *mainRegister, char *command);

/**
 * Apply a script file
 * This function read a given file and apply the commands in it line by line
 * @attention For now a blank line is needed at the end of the file
 * @param mainRegister - The register to add and search for existing objects
 * @param link - link of the file in string format
 */
void readScriptFile(Register *mainRegister, char *link);

#endif //LINEARALGEBRA_MAIN_H