/**
 * \file shelltan.c
 * \brief Main file, managing the linked list
 * \author Emilie AM et Mohamed A
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include "../headers/shelltan.h"


// Log action of user
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

// Free the double linked list
void liberateThem(node* root) {
    struct node* tmp;

   while (root != NULL)
    {
       tmp = root;
       root = root->next;
       free(tmp);
    }
}

// Execute commands in order according to operators
bool launchInOrder(node* root) {
    node* currentNode = root;

    // If this is the only command
    if(currentNode->next == NULL && currentNode->executed == false) {
        createProcessAndExecuteCmd(currentNode, 0, true);
        if(currentNode->executed == true && currentNode->success != 0) {
            printCommandError(currentNode->command, currentNode->success);
            //exit(currentNode->success);
        }
    }

    // Loop through the double linked list
    while(currentNode != 0) {
        printf(currentNode->command);
        // For && operator
        
        if(strcmp(currentNode->command, "&&") == 0) {
            logicalOperatorAnd(currentNode);
        } 
        // For || operator
        else if (strcmp(currentNode->command, "||") == 0) {
            logicalOperatorOr(currentNode);
        } 
        // For | operator
        else if(strcmp(currentNode->command, "|") == 0) {
            pipeOperator(currentNode);
        } 
        // For ; operator
        else if(strcmp(currentNode->command, ";") == 0) {
            listOperator(currentNode);
        }  
        // For > operator
        else if(strcmp(currentNode->command, ">") == 0) {
            redirectOperatorSimpleRight(currentNode);
        } 
        // For >> operator
        else if(strcmp(currentNode->command, ">>") == 0) {
            redirectOperatorDoubleRight(currentNode);
        } 
        // For < operator
        else if(strcmp(currentNode->command, "<") == 0) {
            redirectOperatorSimpleLeft(currentNode);
        } 
        // For << operator
        else if(strcmp(currentNode->command, "<<") == 0) {
            redirectOperatorDoubleLeft(currentNode);
        }
        
        currentNode = currentNode->next;
    }

    return true;
}

// Display the linked list (only for debug)
void displayChain(node* root) {
    printf("%s\n", root->command);
    if(root->next != 0) {
        displayChain(root->next);
    }
}

// Build the linked list
int buildChain(char *command) {
    char *token;
    node *root;
    node *current;

    logAction(command);
    
    token = strtok(command, " ");
    root = createNode(token);
    current = root;
    token = strtok(NULL, " ");

    // Separate by space operators and commands
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
    //displayChain(root);
    launchInOrder(root);
    liberateThem(root);
    return 0;
}

// If user want to enter multiple command
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

// Check if user execute in batch (one command) or interactive (multiple command) mode
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
