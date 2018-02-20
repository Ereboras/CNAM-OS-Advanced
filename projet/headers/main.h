#ifndef FILE_MAIN_HEADER
#define FILE_MAIN_HEADER

#include "../headers/typedef.h"

/* @function : inputString
 * @description: Get user input and realloc if the command is too long
 * @param : FILE* fp
 * @param : size_t size
 * @return : nothing
*/
char* inputString(FILE* fp, size_t size);

/* @function : currentPosition
 * @description: Get current position in tree files
 * @param : char* path
 * @param : int size
 * @return : char*
*/
char* currentPosition(char* path, int size);

/* @function : buildTree
 * @description: Parse string & set it in tree mode
 * @param : char* commands
 * @return : nothing
*/
void buildTree(char* commands);

/* @function : interactiveMode
 * @description: Loop for interactive mode
 * @return : nothing
*/
void interactiveMode();

/* @function : checkMode
 * @description: Check if user want to enter interactive mode or not
 * @param : int sizeCommand
 * @param : char* commands[]
 * @return : nothing
*/
void checkMode(int sizeCommand, char* commands[]);

/* @function : logAction
 * @description: Log action made by user in a log file
 * @param : char* commands
 * @return : nothing
*/
void logAction(char* command);

#endif