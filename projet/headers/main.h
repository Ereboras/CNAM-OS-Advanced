#ifndef FILE_MAIN_HEADER
#define FILE_MAIN_HEADER

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
 * \fn isBuiltInCommand(char* cmd)
 * \brief Check if string is a built in command
 * 
 * \param operator String to check
 * \return bool
 */
bool isBuiltInCommand(char* cmd);

/**
 * \fn logAction(char* command)
 * \brief Register commands in a log file
 * 
 * \param command Command to log
 * \return void
 */
void logAction(char* command);

/**
 * \fn currentPosition(char* path, int size)
 * \brief Get the current position of the user
 * 
 * \param path The string to register the path
 * \param size size of the string
 * \return int
 */
int currentPosition(char* path, int size);

/**
 * \fn changeDir(char *path)
 * \brief Change the directory of the user
 * 
 * \param path The path to where the user want to move
 * \return int
 */
int changeDir(char *path);

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