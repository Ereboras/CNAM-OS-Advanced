#ifndef FILE_MAIN_HEADER
#define FILE_MAIN_HEADER

#include "../headers/typedef.h"
#include "../headers/utils.h"
#include "../headers/commands.h"


/**
 * \fn logAction(char* command)
 * \brief Register commands in a log file
 * 
 * \param command Command to log
 * \return void
 */
void logAction(char* command);

/**
 * \fn executeCmd(node* element)
 * \brief Execute the command in the node
 * 
 * \param element The node of the command
 * \return void
 */
void executeCmd(node* element);

/**
 * \fn void createProcessAndExecuteCmd(node* element, int input, bool lastCommand)
 * \brief Manage the I/O and process to execute commands
 * 
 * \param element The node of the command
 * \param input The input for the command
 * \param lastCommand Is it the last command of the line
 * \return void
 */
void createProcessAndExecuteCmd(node* element, int input, bool lastCommand);

/**
 * \fn liberateThem(node* root)
 * \brief Free the linked list
 * 
 * \param root The root node of the linked list
 * \return void
 */
void liberateThem(node* root);

/**
 * \fn launchInOrder(node* root)
 * \brief Manage the order to executes commands & conditions for operators
 * 
 * \param root The root node of the linked list
 * \return void
 */
bool launchInOrder(node* root);

/**
 * \fn displayChain(node* root)
 * \brief Display the linked list
 * 
 * \param root The root node of the linked list
 * \return void
 */
void displayChain(node* root);

/**
 * \fn buildChain(char* commands)
 * \brief Create the linked tree
 * 
 * \param commands The string from where the linked tree is created
 * \return int
 */
int buildChain(char* commands);

/**
 * \fn interactiveMode()
 * \brief Display the interactive mode interface
 * 
 * \return void
 */
void interactiveMode();

/**
 * \fn checkMode(int sizeCommand, char* commands[])
 * \brief Check the mode the user want to use (-c or interactive)
 * 
 * \param sizeCommand The size of the arguments
 * \param commands[] List of arguments
 * \return void
 */
void checkMode(int sizeCommand, char* commands[]);

#endif