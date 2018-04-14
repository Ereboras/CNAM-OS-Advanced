/**
 * \file main.c
 * \author Emilie AM and Mohamed A
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
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

int copyFile(FILE* filesrc, FILE* filedest) {
    char content[80];
    while(fgets(content, sizeof(content), filesrc) !=NULL) {
        fputs(content, filedest);
    }
    fclose(filedest);
    fclose(filesrc);
    return 0;
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

void executeCmd(node* element) {
    
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
    
    if(strcmp(args[0], "cd") == 0) {
        element->success = changeDir(args[1]);
    } else if(strcmp(args[0], "pwd") == 0) {
        char path[8192];
        element->success = currentPosition(path, 8192);
        if(element->success == 0) {
            printf(path);
        }
    } else if(strcmp(args[0], "exit") == 0) {
        element->success = 0;
        exit(0);
    } else if(strcmp(args[0], "echo") == 0) {
        i = 1;
        while(args[i] != NULL) {
            printf("%s ", args[i]);
            i++;
        }
        
        element->success = 0;
    } else {
        element->success = 0;
        if(execvp(args[0], args) == -1) {
            exit(errno);
        }
    }
    free(program);
}

void createProcessAndExecuteCmd(node* element, int input, bool lastCommand) {
    
    int link[2];
    element->executed = true;

    if(pipe(link) == -1) {
        printf("Error when creating pipe : %s", strerror(errno));
        exit(errno);
    }

    pid_t pid = fork();

    if(pid == -1) {
        printf("Error when creating child process: %s", strerror(errno));
        exit(errno);
    } else if(pid == 0) {
        // printf("Execute command %s\n", element->command);

        if(input != 0) {
            dup2(input, STDIN_FILENO);
        }
        
        dup2(link[1], STDOUT_FILENO);
        
        close(link[0]);
        close(link[1]);
        
        executeCmd(element);
        
        exit(errno);
    } else {
        int status_id = -1;
        char readbuffer[32000];

        wait(&status_id);

        if ( WIFEXITED(status_id) ) {
            // printf("Exit status was %d\n", WEXITSTATUS(status_id));
            element->success = status_id;
        }

        close(link[1]);
        read(link[0], readbuffer, sizeof(readbuffer));
        if(!lastCommand) {
            element->response = fopen("tmp_command", "w+");
            fprintf(element->response, readbuffer);
            fclose(element->response);
        } else {
            printf("%s\n", readbuffer);
        }
        memset(readbuffer, 0, 32000);
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

int openCommandFile(char *action) {
    FILE *file = fopen("tmp_command", action);
    if(file == NULL) {
        return -1;
    }
    return fileno(file);
}

void printCommandError(char* command, int error) {
    printf("Error while executing [%s] : code[%d] => %s\n", command, error, strerror(error));
}

bool launchInOrder(node* root) {
    node* currentNode = root;

    if(currentNode->next == NULL && currentNode->executed == false) {
        createProcessAndExecuteCmd(currentNode, 0, true);
        if(currentNode->executed == true && currentNode->success != 0) {
            printCommandError(currentNode->command, currentNode->success);
            exit(currentNode->success);
        }
    }

    while(currentNode->next != 0) {

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
        } else if (strcmp(currentNode->command, "||") == 0) {
            if(currentNode->previous->executed == false) {
                createProcessAndExecuteCmd(currentNode->previous, 0, true);
            }
            if(currentNode->previous->executed == true && currentNode->previous->success != 0 && currentNode->next->executed == false) {
                createProcessAndExecuteCmd(currentNode->next, 0, true);
            }
        } else if(strcmp(currentNode->command, "|") == 0) {
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
        } else if(strcmp(currentNode->command, ";") == 0) {
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
        }  else if(strcmp(currentNode->command, ">") == 0) {
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
        } else if(strcmp(currentNode->command, ">>") == 0) {
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
        } else if(strcmp(currentNode->command, "<") == 0) {
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
        } else if(strcmp(currentNode->command, "<<") == 0) {
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

            file = fopen("tmp_command", "r");
            tmpfileint = fileno(file);
            createProcessAndExecuteCmd(currentNode->previous, tmpfileint, true);
            fclose(file);
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
    liberateThem(root);
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
