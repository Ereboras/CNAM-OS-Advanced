/**
 * \file main.c
 * \author Emilie AM et Mohamed A
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include <sys/wait.h>
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
        fprintf(fp, "%s", action);
        fprintf(fp, "\n");
        fclose(fp);
    } else {
        printf("Error while logging command: %s", strerror(errno));
    }
}

bool currentPosition(char* cwd, int size) {
    if(getcwd(cwd, size) == 0)  {
        printf("Error while retrieving cwd: %s", strerror(errno));
        return false;
    }
    return true;
}

bool changeDir(char *path) {
    if(chdir(path) != 0) {
        printf("Error while changing cwd: %s", strerror(errno));
        return false;
    }
    return true;
}

void executeCmd(node* element) {

    char* program = malloc(sizeof(element->command));
    strcpy(program, element->command);
    char* cmd = strtok(program, " ");

    if(strcmp(cmd, "cd") == 0) {
        char* path = strtok(NULL, " ");
        element->success = changeDir(path);
    } else if(strcmp(cmd, "pwd") == 0) {
        char path[8192];
        element->success = currentPosition(path, 8192);
        printf("%s\n", path);
    } else if(strcmp(cmd, "exit") == 0) {
        element->success = true;
        exit(0);
    } else if(strcmp(cmd, "echo") == 0) {
        printf("%s\n", element->command);
        element->success = true;
    } else {
        char *args[10];
        int i = 0;
        cmd = strtok(NULL, " ");
        while(cmd != NULL) {
            args[i] = cmd;
            cmd = strtok(NULL, " ");
            i++;
        }
        int y = 0;
        printf("command: %s\n", program);
        while(args[y] != NULL) {
            printf("args[%d]: %s\n", y, args[y]);
            y++;
        }
        if(execvp(cmd, args) == -1) {
            printf("Fail executing command: %s", strerror(errno));
            element->success = false;
        } else {
            printf("Success! \n");
            element->success = true;
        }
    }
}

void createProcessAndExecuteCmd(node* element) {
    pid_t pid = fork();
    int link[2];

    if(pipe(link) == -1) {
        printf("Error when creating pipe : %s", strerror(errno));
    }

    if(pid == -1) {
        printf("Error when creating child process: %s", strerror(errno));
    } else if(pid == 0) {
        printf("Execute command %s\n", element->command);

        close(link[0]);
        dup(link[1]);
        close(link[1]);
        executeCmd(element);
        
        exit(0);
    } else {
        int wait_id = -1;
        close(link[1]);
        dup(link[0]);
        
        wait(&wait_id);

        char readbuffer[100];
        read(link[0], readbuffer, sizeof(readbuffer));
        printf("Received string: %s", readbuffer);
        close(link[0]);
    }
}

bool launchInOrder(node* root) {
    node* currentNode = root;

    while(currentNode->next != 0) {

        if(strcmp(currentNode->command, "&&") == 0) {
            if(currentNode->previous->response == 0) {
                createProcessAndExecuteCmd(currentNode->previous);
            }
        }
        
        currentNode = currentNode->next;
    }

    return true;
}

void displayChain(node* root) {
    printf("%s", root->command);
    if(root->next != 0) {
        displayChain(root->next);
    }
}

int buildChain(char *command) {
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

    launchInOrder(root);
    return 0;
}

void interactiveMode() {
    char *command;
    char path[8192];
    int success = 0;
    
    printf("Enter a command: \n");
    while(success == 0) {
        currentPosition(path, 8192);
        printf("\n%s > ", path);
        command = inputString(stdin, 10);
        success = buildChain(command);
    }

    exit(success);
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
