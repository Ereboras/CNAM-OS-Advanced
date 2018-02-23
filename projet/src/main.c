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

void executeCmd(node* element, char* input) {
    
    char* program = malloc(sizeof(element->command));
    strcpy(program, element->command);
    char* cmd = strtok(program, " ");

    if(strcmp(cmd, "cd") == 0) {
        char* path = strtok(NULL, " ");
        element->success = changeDir(path);
    } else if(strcmp(cmd, "pwd") == 0) {
        char path[8192];
        element->success = currentPosition(path, 8192);
        element->response = malloc(sizeof(path));
        element->response = path;
    } else if(strcmp(cmd, "exit") == 0) {
        element->success = 0;
        exit(0);
    } else if(strcmp(cmd, "echo") == 0) {
        element->response = element->command;
        element->success = 0;
    } else {
        char *args[10];
        int i;

        for(i = 0; cmd != NULL; i++) {
            args[i] = cmd;
            cmd = strtok(NULL, " ");
        }
        args[i] = NULL;
        element->success = 0;
        if(execvp(args[0], args) == -1) {
            exit(errno);
        }
    }
}

void createProcessAndExecuteCmd(node* element, char* input) {
    pid_t pid = fork();
    int link[2];

    if(pipe(link) == -1) {
        printf("Error when creating pipe : %s", strerror(errno));
        exit(errno);
    }

    if(pid == -1) {
        printf("Error when creating child process: %s", strerror(errno));
        exit(errno);
    } else if(pid == 0) {
        printf("Execute command %s\n", element->command);
        
        close(link[0]); //close reading end in the child
        dup2(link[1], 1); //send stdout to the pipe
        dup2(link[1], 2); //send stderr to the pipe

        close(link[1]);
        executeCmd(element, input);
        
        exit(errno);
    } else {
        int status_id = -1;
        wait(&status_id);

        if ( WIFEXITED(status_id) ) {
            printf("Exit status was %d\n", WEXITSTATUS(status_id));
            element->success = status_id;
        }
        
        close(link[1]); //close write end of the pipe in the parent

        char readbuffer[100];
        read(link[0], readbuffer, strlen(readbuffer));
        element->response = readbuffer;
        printf("AFFICHAGE DE RESPONSE:\n %s", readbuffer);
        close(link[0]);
    }
}

void liberateThem(node* root) {
    struct node* tmp;

   while (root != NULL)
    {
       tmp = root;
       root = root->next;
       free(tmp);
    }
}

bool launchInOrder(node* root) {
    node* currentNode = root;

    if(currentNode->next == NULL) {
        createProcessAndExecuteCmd(currentNode, NULL);
    }

    while(currentNode->next != 0) {

        if(strcmp(currentNode->command, "&&") == 0) {
            if(currentNode->previous->response == 0) {
                createProcessAndExecuteCmd(currentNode->previous, NULL);
            }
            if(currentNode->previous->success == 0 && currentNode->next != NULL) {
                createProcessAndExecuteCmd(currentNode->next, currentNode->previous->response);
            } else if (currentNode->previous->success  != 0){
                printf("Error while executing [%s] : code[%d] => %s", currentNode->previous->command, currentNode->previous->success, strerror(currentNode->previous->success));
                exit(currentNode->previous->success);
            }
        } else if (strcmp(currentNode->command, "||") == 0) {
            if(currentNode->previous->response == 0) {
                createProcessAndExecuteCmd(currentNode->previous, NULL);
            }
            if(currentNode->previous->success != 0 && currentNode->next != NULL) {
                createProcessAndExecuteCmd(currentNode->next, currentNode->previous->response);
            }
        } else if(strcmp(currentNode->command, "|") == 0) {
            if(currentNode->previous->response == 0) {
                createProcessAndExecuteCmd(currentNode->previous, NULL);
            }
            if(currentNode->previous->success == 0 && currentNode->next != NULL) {
                createProcessAndExecuteCmd(currentNode->next, NULL);
            } else if(currentNode->previous->success != 0) {
                printf("Error while executing [%s] : %s", currentNode->previous->command, strerror(currentNode->success));
                exit(currentNode->previous->success);
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

    logAction(command);
    
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
