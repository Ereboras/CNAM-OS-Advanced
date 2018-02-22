#include <stdlib.h>
#include <stdio.h>
#include "../headers/linked_list.h"

node* createNode(char* cmd) {
    node *root = malloc(sizeof(node));
    root->command = cmd;
    return root;
}

node* createEmptyNode() {
    node *root = malloc(sizeof(node));
    root->command = "";
    return root;
}