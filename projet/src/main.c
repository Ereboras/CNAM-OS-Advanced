#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include "../headers/struct.h"

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

void interactiveMode() {
    char *command;
    printf("Enter a command: \n");
    while(strcmp(command, "exit") != 0) {
        printf(">");
        command = inputString(stdin, 10);
    }
}

void checkString(char* commands[]) {

}

void checkMode(int sizeCommand, char* commands[]) {
    if(sizeCommand == 1) {
        printf("Activate interactive mode\n");
        interactiveMode();
    } else if(strcmp(commands[1], "-c") == 0) {
        printf("Executing command\n");
        checkString(commands);
    } else {
        printf("Bad argument error");
    }
}

int main(int argc, char** argv){
    
    checkMode(argc, argv);
    printf("\n");
}
