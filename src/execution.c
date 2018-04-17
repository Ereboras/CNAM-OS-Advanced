#include "../headers/execution.h"

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