#ifndef FILE_EXECUTION_HEADER
#define FILE_EXECUTION_HEADER

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../headers/typedef.h"
#include "../headers/utils.h"
#include "../headers/commands.h"


/**
 * \fn void forkAndRedirectCmd(node *element, char *args[20], bool lastCommand, int input)
 * \brief Manage child / parent process and input / output
 * 
 * \param element The node of the command
 * \param args Arguments of the command
 * \param lastCommand Is it the last command of the line
 * \param input Input of the command
 * \return void
 */
void forkAndRedirectCmd(node *element, char *args[20], bool lastCommand, int input);

/**
 * \fn void forkAndRedirectCmd(node *element, char *args[20], bool lastCommand, int input)
 * \brief Check if command is built-in or external command and redirect to correct function
 * 
 * \param element The node of the command
 * \param input Input of the command
 * \param lastCommand Is it the last command of the line
 * \return void
 */
void createProcessAndExecuteCmd(node* element, int input, bool lastCommand);

#endif