#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

int arrayIterate(int sizeCommand, char* commands[]) {
    for(int i = 1; i < sizeCommand; i++) {
        printf("%s ", commands[i]);
    }
}

int main(int argc, char** argv){
    arrayIterate(argc, argv);
    printf("\n");
}