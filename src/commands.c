/**
 * \file commands.c
 * \brief File for built-in and external commands
 * \author Emilie AM et Mohamed A
 */
#include "../headers/commands.h"

int currentPosition(char* cwd, int size) {
    if(getcwd(cwd, size) == 0)  {
        return errno;
    }
    return 0;
}

int changeDir(char *path) {
    if(chdir(path) != 0) {
        return errno;
    }
    return 0;
}

void doCd(node* element, char *args[20]) {
    element->success = changeDir(args[1]);
}

void doExit(node *element) {
    element->success = 0;
    exit(0);
}

void doEcho(node *element, char *args[20], char *buffer) {
    int i = 2;
    strcpy(buffer, args[1]);
    strcat(buffer, " ");
    while(args[i] != NULL) {
        strcat(buffer, args[i]);
        strcat(buffer, " ");
        i++;
    }
    
    element->success = 0;
}

void doPwd(node* element, char *buffer, int size) {
    element->success = currentPosition(buffer, size);
}

void doExternCommand(node* element, char *args[20]) {
    element->success = 0;
    if(execvp(args[0], args) == -1) {
        exit(errno);
    }
}