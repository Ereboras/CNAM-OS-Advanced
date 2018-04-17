#ifndef FILE_COMMANDS_HEADER
#define FILE_COMMANDS_HEADER

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../headers/linked_list.h"

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
 * \fn doCd(node* element, char *args[20])
 * \brief Execute cd built-in command
 * 
 * \param element The node of the command
 * \return void
 */
void doCd(node* element, char *args[20]);

/**
 * \fn doExit(node* element)
 * \brief Execute exit built-in command
 * 
 * \param element The node of the command
 * \return void
 */
void doExit(node *element);

/**
 * \fn doEcho(node* element, char *args[20], char *buffer)
 * \brief Execute echo built-in command
 * 
 * \param element The node of the command
 * \param args The arguments of the command
 * \param buffer Contains the display string
 * \return void
 */
void doEcho(node *element, char *args[20], char *buffer);

/**
 * \fn doPwd(node* element, char *buffer, int size)
 * \brief Execute pwd built-in command
 * 
 * \param element The node of the command
 * \param buffer Contains the display string
 * \param size Size of the string
 * \return void
 */
void doPwd(node* element, char *buffer, int size);

/**
 * \fn doExternCommand(node* element, char *args[20])
 * \brief Execute external command
 * 
 * \param element The node of the command
 * \param args The arguments of the command
 * \return void
 */
void doExternCommand(node* element, char *args[20]);

#endif