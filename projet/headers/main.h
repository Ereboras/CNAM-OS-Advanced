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

/* @function : isOperator
 * @description: Check if string is a linux operator
 * @param : char* operator
 * @return : bool
*/
bool isOperator(char* operator);

/* @function : logAction
 * @description: Log action made by user in a log file
 * @param : char* commands
 * @return : nothing
*/
void logAction(char* command);

/* @function : currentPosition
 * @description: Get current position in tree files
 * @param : char* path
 * @param : int size
 * @return : char*
*/
char* currentPosition(char* path, int size);

/* @function : changeDir
 * @description: Change the current directory
 * @param : char *path
 * @return : nothing
*/
void changeDir(char *path);

/* @function : executeCommands
 * @description: Launch execution of commands
 * @param : node* root
 * @return : bool
*/
bool executeCommands(node* root);

/* @function : displayChain
 * @description: Display the chained list
 * @param : node* root
 * @return : nothing
*/
void displayChain(node* root);

/* @function : buildTree
 * @description: Parse string & set it in tree mode
 * @param : char* commands
 * @return : nothing
*/
void buildChain(char* commands);

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

#endif