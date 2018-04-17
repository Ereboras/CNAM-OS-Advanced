/**
 * \file linked_list.c
 * \brief File for linked list functions
 * \author Emilie AM et Mohamed A
 */
#include <stdlib.h>
#include <stdio.h>
#include "../headers/linked_list.h"

// Allow to create a simple node
node* createNode(char* cmd) {
    node *root = malloc(sizeof(node));
    root->command = cmd;
    root->executed = false;
    return root;
}

// Create an empty node
node* createEmptyNode() {
    node *root = malloc(sizeof(node));
    root->command = "";
    root->executed = false;
    return root;
}

// Create a node and place it next to the root node
node* createNodeAndLinkNext(node* root, char* cmd) {
    node *temp = createNode(cmd);
    root->next = temp;
    temp->previous = root;
    return temp;
}