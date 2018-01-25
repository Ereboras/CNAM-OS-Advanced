#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include "../headers/struct.h"

void interactiveMode() {
    printf("Enter a command: ");

}

void checkString(char* commands[]) {

}

void checkMode(int sizeCommand, char* commands[]) {
    printf("|%s|\n", commands[1]);
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
