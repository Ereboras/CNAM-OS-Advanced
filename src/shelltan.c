/**
 * \file shelltan.c
 * \brief Main file, managing the linked list
 * \author Emilie AM et Mohamed A
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "../headers/shelltan.h"
#include "../headers/linked_list.h"

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

// Put response of command in a file for the next command or display it if this is the last command
void resultInFile(node *element, bool lastCommand, char readbuffer[32000]) {
    if(!lastCommand) {
        element->response = fopen("tmp_command", "w+");
        fprintf(element->response, readbuffer);
        fclose(element->response);
    } else {
        printf("%s\n", readbuffer);
    }
}

// Manage child / parent process and input / output
void forkAndRedirectCmd(node *element, char *args[20], bool lastCommand, int input) {

    int link[2];

    if(pipe(link) == -1) {
        printf("Error when creating pipe : %s", strerror(errno));
        exit(errno);
    }

    pid_t pid = fork();

    if(pid == -1) {
        printf("Error when creating child process: %s", strerror(errno));
        exit(errno);
    } else if(pid == 0) {

        if(input != 0) {
            dup2(input, STDIN_FILENO);
        }
        
        dup2(link[1], STDOUT_FILENO);
        
        close(link[0]);
        close(link[1]);
        
        doExternCommand(element, args);
        
        exit(errno);
    } else {
        int status_id = -1;
        char readbuffer[32000];

        wait(&status_id);

        if ( WIFEXITED(status_id) ) {
            element->success = status_id;
        }

        close(link[1]);
        read(link[0], readbuffer, sizeof(readbuffer));
        resultInFile(element, lastCommand, readbuffer);
        memset(readbuffer, 0, 32000);
    }
}

// Check if command is built-in or external command and redirect to correct function
void createProcessAndExecuteCmd(node* element, int input, bool lastCommand) {
    
    element->executed = true; 

    // Seperate command & args
    char* program = malloc(sizeof(element->command));
    strcpy(program, element->command);
    char* cmd = strtok(program, " ");
    char *args[20];
    int i;
    
    for(i = 0; cmd != NULL; i++) {
        args[i] = cmd;
        cmd = strtok(NULL, " ");
    }
    args[i] = NULL;

    // Execute built-in command
    if(strcmp(args[0], "cd") == 0) {
        doCd(element, args);
    } else if(strcmp(args[0], "pwd") == 0) {
        char buffer[8192];
        doPwd(element, buffer, 8192);
        resultInFile(element, lastCommand, buffer);
    } else if(strcmp(args[0], "exit") == 0) {
        doExit(element);
    } else if(strcmp(args[0], "echo") == 0) {
        char buffer[1024];
        doEcho(element, args, buffer);
        resultInFile(element, lastCommand, buffer);
    }

    // External command
    if(!isBuiltInCommand(args[0])) {
        forkAndRedirectCmd(element, args, lastCommand, input);
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

    // The last command to execute
    if(currentNode->next == NULL && currentNode->executed == false) {
        createProcessAndExecuteCmd(currentNode, 0, true);
        if(currentNode->executed == true && currentNode->success != 0) {
            printCommandError(currentNode->command, currentNode->success);
            exit(currentNode->success);
        }
    }

    // Loop through the double linked list
    while(currentNode->next != 0) {
        
        // For && operator
        if(strcmp(currentNode->command, "&&") == 0) {
            if(currentNode->previous->executed == false) {
                createProcessAndExecuteCmd(currentNode->previous, 0, true);
            }
            
            if(currentNode->previous->executed == true && currentNode->previous->success == 0 && currentNode->next->executed == false) {
                createProcessAndExecuteCmd(currentNode->next, 0, true);
            } else if (currentNode->previous->executed == true && currentNode->previous->success  != 0){
                printCommandError(currentNode->previous->command, currentNode->previous->success);
                exit(currentNode->previous->success);
            }
        } 
        // For || operator
        else if (strcmp(currentNode->command, "||") == 0) {
            if(currentNode->previous->executed == false) {
                createProcessAndExecuteCmd(currentNode->previous, 0, true);
            }
            if(currentNode->previous->executed == true && currentNode->previous->success != 0 && currentNode->next->executed == false) {
                createProcessAndExecuteCmd(currentNode->next, 0, true);
            }
        } 
        // For | operator
        else if(strcmp(currentNode->command, "|") == 0) {
            if(currentNode->previous->executed == false) {
                createProcessAndExecuteCmd(currentNode->previous, 0, false);
            }

            if(currentNode->previous->executed == true && currentNode->previous->success == 0 && currentNode->next != NULL) {
                int filenum = openCommandFile("r");
                if(filenum >= 0) {
                    createProcessAndExecuteCmd(currentNode->next, filenum, true);
                    close(filenum);
                } else {
                    printCommandError(currentNode->previous->command, currentNode->previous->success);
                }
            } else if(currentNode->previous->success != 0) {
                printCommandError(currentNode->previous->command, currentNode->previous->success);
                exit(currentNode->previous->success);
            }
        } 
        // For ; operator
        else if(strcmp(currentNode->command, ";") == 0) {
            if(currentNode->previous->executed == false) {
                createProcessAndExecuteCmd(currentNode->previous, 0, true);
                if(currentNode->previous->success != 0) {
                    printCommandError(currentNode->previous->command, currentNode->previous->success);
                }
            }
            if(currentNode->next->executed == false) {
                createProcessAndExecuteCmd(currentNode->next, 0, true);
                if(currentNode->next->success != 0) {
                    printCommandError(currentNode->next->command, currentNode->next->success);
                }
            }
        }  
        // For > operator
        else if(strcmp(currentNode->command, ">") == 0) {
            createProcessAndExecuteCmd(currentNode->previous, 0, false);
            if(currentNode->previous->success != 0) {
                printCommandError(currentNode->previous->command, currentNode->previous->success);
                exit(currentNode->previous->success);
            }
            FILE *filedest = fopen(currentNode->next->command, "w");
            FILE *filesrc = fopen("tmp_command", "r");
            if(filedest != NULL && filesrc != NULL) {
                copyFile(filesrc, filedest);
            } else {
                printCommandError(currentNode->next->command, errno);
                exit(errno);
            }
        } 
        // For >> operator
        else if(strcmp(currentNode->command, ">>") == 0) {
            createProcessAndExecuteCmd(currentNode->previous, 0, false);
            if(currentNode->previous->success != 0) {
                printCommandError(currentNode->previous->command, currentNode->previous->success);
                exit(currentNode->previous->success);
            }
            FILE *filedest = fopen(currentNode->next->command, "a");
            FILE *filesrc = fopen("tmp_command", "r");
            if(filedest != NULL && filesrc != NULL) {
                copyFile(filesrc, filedest);
            } else {
                printCommandError(currentNode->next->command, errno);
                exit(errno);
            }
        } 
        // For < operator
        else if(strcmp(currentNode->command, "<") == 0) {
            FILE *file = fopen(currentNode->next->command, "r");
            if(file == NULL) {
                printCommandError(currentNode->next->command, errno);
                exit(errno);
            }
            int tmpfileint = fileno(file);
            if(tmpfileint == -1) {
                printCommandError(currentNode->next->command, errno);
                exit(errno);
            }
            createProcessAndExecuteCmd(currentNode->previous, tmpfileint, true);
            close(tmpfileint);
        } 
        // For << operator
        else if(strcmp(currentNode->command, "<<") == 0) {
            FILE *file = fopen("tmp_command", "w+");
            if(file == NULL) {
                printCommandError(currentNode->next->command, errno);
                exit(errno);
            }
            int tmpfileint = fileno(file);
            if(tmpfileint == -1) {
                printCommandError(currentNode->next->command, errno);
                exit(errno);
            }
            // Check if the user end the input with the corresponding string
            char *text;
            text = "0000000000";
            while(strcmp(currentNode->next->command, text) != 0) {
                printf(">");
                text = inputString(stdin, 10);
                if(strcmp(currentNode->next->command, text) != 0) {
                    fputs(text, file);
                    fputs("\n", file);
                }
            }
            fputs("\0", file);
            fclose(file);
            free(text);

            // Execute command on the input of user
            file = fopen("tmp_command", "r");
            tmpfileint = fileno(file);
            createProcessAndExecuteCmd(currentNode->previous, tmpfileint, true);
            fclose(file);
        }
        
        currentNode = currentNode->next;
    }

    return true;
}

// Display the linked list (only for debug)
void displayChain(node* root) {
    printf("%s", root->command);
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
