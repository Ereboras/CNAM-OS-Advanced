#include "../headers/operators.h"

void listOperator(node* currentNode) {
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

void pipeOperator(node* currentNode) {
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
        //exit(currentNode->previous->success);
    }
}

void logicalOperatorAnd(node* currentNode) {
    if(currentNode->previous->executed == false) {
        createProcessAndExecuteCmd(currentNode->previous, 0, true);
    }
    
    if(currentNode->previous->executed == true && currentNode->previous->success == 0 && currentNode->next->executed == false) {
        createProcessAndExecuteCmd(currentNode->next, 0, true);
    } else if (currentNode->previous->executed == true && currentNode->previous->success  != 0){
        printCommandError(currentNode->previous->command, currentNode->previous->success);
        //exit(currentNode->previous->success);
    }
}

void logicalOperatorOr(node* currentNode) {
    if(currentNode->previous->executed == false) {
        createProcessAndExecuteCmd(currentNode->previous, 0, true);
    }
    if(currentNode->previous->executed == true && currentNode->previous->success != 0 && currentNode->next->executed == false) {
        createProcessAndExecuteCmd(currentNode->next, 0, true);
    }
}

void redirectOperatorSimpleLeft(node* currentNode) {
    FILE *file = fopen(currentNode->next->command, "r");
    if(file == NULL) {
        printCommandError(currentNode->next->command, errno);
        //exit(errno);
    }
    int tmpfileint = fileno(file);
    if(tmpfileint == -1) {
        printCommandError(currentNode->next->command, errno);
        //exit(errno);
    }
    createProcessAndExecuteCmd(currentNode->previous, tmpfileint, true);
    close(tmpfileint);
}

void redirectOperatorSimpleRight(node* currentNode) {
    createProcessAndExecuteCmd(currentNode->previous, 0, false);
    if(currentNode->previous->success != 0) {
        printCommandError(currentNode->previous->command, currentNode->previous->success);
        //exit(currentNode->previous->success);
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

void redirectOperatorDoubleLeft(node* currentNode) {
    FILE *file = fopen("tmp_command", "w+");
    if(file == NULL) {
        printCommandError(currentNode->next->command, errno);
        //exit(errno);
    }
    int tmpfileint = fileno(file);
    if(tmpfileint == -1) {
        printCommandError(currentNode->next->command, errno);
        //exit(errno);
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

void redirectOperatorDoubleRight(node* currentNode) {
    createProcessAndExecuteCmd(currentNode->previous, 0, false);
    if(currentNode->previous->success != 0) {
        printCommandError(currentNode->previous->command, currentNode->previous->success);
        //exit(currentNode->previous->success);
    }
    FILE *filedest = fopen(currentNode->next->command, "a");
    FILE *filesrc = fopen("tmp_command", "r");
    if(filedest != NULL && filesrc != NULL) {
        copyFile(filesrc, filedest);
    } else {
        printCommandError(currentNode->next->command, errno);
        //exit(errno);
    }
}