/**
 * \file linked_list.c
 * \brief Fichier des fonctions de la liste chaînée
 * \author Emilie AM et Mohamed A
 */
#include <stdlib.h>
#include <stdio.h>
#include "../headers/linked_list.h"


node* createNode(char* cmd) {
    node *root = malloc(sizeof(node));
    root->command = cmd;
    root->executed = false;
    return root;
}


node* createEmptyNode() {
    node *root = malloc(sizeof(node));
    root->command = "";
    root->executed = false;
    return root;
}


node* createNodeAndLinkNext(node* root, char* cmd) {
    node *temp = createNode(cmd);
    root->next = temp;
    temp->previous = root;
    return temp;
}