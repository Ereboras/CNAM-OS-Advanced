/**
 * \file utils.c
 * \brief File for global utility functions
 * \author Emilie AM et Mohamed A
 */
#include "../headers/utils.h"

// Dynamically allocate memory for user inputs
char *inputString(FILE* fp, size_t size) {
    char *str;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(char)*size);// The size is extended by the input with the value of the provisional
    if(!str)return str;
    while(EOF!=(ch=fgetc(fp)) && ch != '\n'){
        str[len++]=ch;
        if(len==size){
            str = realloc(str, sizeof(char)*(size+=16));
            if(!str)return str;
        }
    }
    str[len++]='\0';

    return realloc(str, sizeof(char)*len);
}

// Copy content from source file and write it to destination file
int copyFile(FILE* filesrc, FILE* filedest) {
    char content[80];
    while(fgets(content, sizeof(content), filesrc) !=NULL) {
        fputs(content, filedest);
    }
    fclose(filedest);
    fclose(filesrc);
    return 0;
}

// Check if string is a linux operator
bool isOperator(char* operator) {
    if ((strcmp(operator, ";") == 0) || 
        (strcmp(operator, "|") == 0) || 
        (strcmp(operator, "&") == 0) || 
        (strcmp(operator, "&&") == 0) || 
        (strcmp(operator, "||") == 0) || 
        (strcmp(operator, "<") == 0) || 
        (strcmp(operator, ">") == 0) || 
        (strcmp(operator, "<<") == 0) || 
        (strcmp(operator, ">>") == 0)) {
        return true;
    } else {
        return false;
    }
}

// Check if string is a built in command
bool isBuiltInCommand(char* cmd) {
    if ((strcmp(cmd, "cd") == 0) ||
        (strcmp(cmd, "pwd") == 0) ||
        (strcmp(cmd, "exit") == 0) ||
        (strcmp(cmd, "echo") == 0)) {
        return true;
    } else {
        return false;
    }
}

// Open the file for commands
int openCommandFile(char *action) {
    FILE *file = fopen("tmp_command", action);
    if(file == NULL) {
        return -1;
    }
    return fileno(file);
}

// Display error with command
void printCommandError(char* command, int error) {
    printf("Error while executing [%s] : code[%d] => %s\n", command, error, strerror(error));
}