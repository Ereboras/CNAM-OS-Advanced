<<<<<<< Updated upstream
/**
 * \file main.c
 * \author Emilie AM et Mohamed A
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
=======
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
>>>>>>> Stashed changes
#include "../headers/main.h"
#include "../headers/linked_list.h"

char *inputString(FILE* fp, size_t size){
//The size is extended by the input with the value of the provisional
    char *str;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(char)*size);//size is start size
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

void logAction(char* action) {
    FILE *fp = fopen("bin/log", "a");
    if(fp) {
        fprintf(fp, action);
        fprintf(fp, "\n");
        fclose(fp);
    } else {
        printf("Error while logging command: %s", strerror(errno));
    }
}

char* currentPosition(char* cwd, int size) {
    if(getcwd(cwd, size) != 0) {
        return cwd;
    } else {
        printf("Error while retrieving cwd: %s", strerror(errno));
        return "Unknown";
    }
}

void changeDir(char *path) {
    chdir(path);
}

bool createProcessAndExecuteCmd(node* element) {
    pid_t pid = fork();

    if(pid == -1) {
        printf("Error when creating child process: %s", strerror(errno));
        return false;
    } else if(pid == 0) {
        printf("Execute command %s", element->command);
        exit(0);
    } else {
        printf("Currently in parent process");
        wait(pid);
    }
}

bool executeCommands(node* root) {
    node* currentNode = root;

    while(currentNode->next != 0) {
        printf("Check cmd: %s\n", currentNode->command);

        if(strcmp(currentNode->command, "&&") == 0) {
            if(currentNode->previous->response == 0) {
                createProcessAndExecuteCmd(currentNode->previous);
            }
        }
        
        currentNode = currentNode->next;
    }
}

void displayChain(node* root) {
    printf("%s", root->command);
    if(root->next != 0) {
        displayChain(root->next);
    }
}

void buildChain(char *command) {
    char *token;
    node *root;
    node *current;
    
    token = strtok(command, " ");
    root = createNode(token);
    current = root;
    token = strtok(NULL, " ");

    while(token != NULL) {
        if (isOperator(token) == true) {
            current = createNodeAndLinkNext(current, token);
        } else {
            if(isOperator(current->command) == true) {
                current = createNodeAndLinkNext(current, token);
            } else {
                char *concat = malloc(strlen(current->command) + strlen(token) + 1);
                strcpy(concat, current->command);
                strcat(concat, " ");
                strcat(concat, token);
                current->command = concat;
            }
        }
        logAction(token);
        token = strtok(NULL, " ");
    }

    executeCommands(root);
}

void interactiveMode() {
    char *command;
    char path[8192];
    
    printf("Enter a command: \n");
    while(strcmp(command, "exit") != 0) {
        currentPosition(path, 8192);
        printf("\n%s > ", path);
        command = inputString(stdin, 10);
        buildChain(command);
    }

    if(strcmp(command, "exit") == 0) {
        exit(0);
    } else {
        exit(42);
    }
}

void checkMode(int sizeCommand, char *commands[]) {
    if(sizeCommand == 1) {
        printf("Activate interactive mode\n");
        interactiveMode();
    } else if(strcmp(commands[1], "-c") == 0) {
        printf("Executing command\n");
        buildChain(commands[2]);
    } else {
        printf("Bad argument error");
    }
}

int main(int argc, char** argv){
    checkMode(argc, argv);
    printf("\n");
}
