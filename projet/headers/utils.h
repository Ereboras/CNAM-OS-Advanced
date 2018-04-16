#ifndef FILE_UTILS_HEADER
#define FILE_UTILS_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/typedef.h"

/**
 * \fn inputString(FILE* fp, size_t size)
 * \brief Dynamically allocate memory for user inputs
 * 
 * \param fp File descriptor of the input of the user
 * \param size Initial size of the user input, dynamically resized
 * \return char*
 */
char* inputString(FILE* fp, size_t size);

/**
 * \fn copyFile(FILE* filesrc, FILE* filedest)
 * \brief Copy content from source file and write it to destination file
 * 
 * \param filesrc Source file
 * \param filedest Destination file
 * \return int
 */
int copyFile(FILE* filesrc, FILE* filedest);

/**
 * \fn isOperator(char* operator)
 * \brief Check if string is a linux operator
 * 
 * \param operator String to check
 * \return bool
 */
bool isOperator(char* operator);

/**
 * \fn isBuiltInCommand(char *cmd)
 * \brief Check if string is a built in command
 * 
 * \param cmd String to check
 * \return bool
 */
bool isBuiltInCommand(char *cmd);

/**
 * \fn openCommandFile(char *action)
 * \brief Open the file for commands
 * 
 * \param action Write or read file
 * \return int
 */
int openCommandFile(char *action);

/**
 * \fn printCommandError(char *command, int error)
 * \brief Open the file for commands
 * 
 * \param command Command where the error occured
 * \param error Error code
 * \return void
 */
void printCommandError(char* command, int error);

#endif