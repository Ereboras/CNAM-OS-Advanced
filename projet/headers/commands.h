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

void doCd(node* element, char *args[20]);

void doExit(node *element);

void doEcho(node *element, char *args[20], char *buffer);

void doPwd(node* element, char *buffer, int size);

void doExternCommand(node* element, char *args[20]);

#endif