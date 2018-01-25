#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include "../headers/struct.h"

void manageOperators() {

}

void arrayIterate(int sizeCommand, char* commands[]) {
    for(int i = 1; i < sizeCommand; i++) {
        if(commands[i] == "&&" || commands[i] == "||") {
            manageOperators(commands, i);
        }
    }
}

int main(int argc, char** argv){
    arrayIterate(argc, argv);
    printf("\n");
}
