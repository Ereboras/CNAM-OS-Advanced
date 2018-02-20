#include <stdlib.h>
#include <stdio.h>
#include "../headers/tree.h"

node* createRoot(char* command) {
    node *root;
    root->command = command;
    return root;
}

void addRightChild(node *root, node *child)
{
    root->right = child;
}

void addLeftChild(node *root, node *child)
{
    root->left = child;
}