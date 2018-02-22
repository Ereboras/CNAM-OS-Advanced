/**
 * \file linked_list.c
 * \brief Fichier des fonctions de la liste chaînée
 * \author Emilie AM et Mohamed A
 */
#include <stdlib.h>
#include <stdio.h>
#include "../headers/linked_list.h"

/**
 * \fn createNode(char* cmd)
 * \brief Cette fonction permet de créer un noeud à partir d'un string correspondant à une commande.
 * 
 * \param cmd est un string correspondant à un morceau de la commande lancé par l'utilisateur qui a été parsée
 * \return node
 */
node* createNode(char* cmd) {
    node *root = malloc(sizeof(node));
    root->command = cmd;
    return root;
}

/**
 * \fn createEmptyNode()
 * \brief Cette fonction permet de créer un noeud vide.
 * \return node
 */
node* createEmptyNode() {
    node *root = malloc(sizeof(node));
    root->command = "";
    return root;
}

/**
 * \fn createNodeAndLinkNext(node* root, char* cmd)
 * \brief Cette fonction permet de créer un noeud à partir d'un string correspondant à une commande ainsi que du noeud qui le précède.
 * La Fonction renvoie vers le noeud nouvellement créé.
 * 
 * \param root est le noeud dans lequel on souhaite enregistré le prochain noeud
 * \param cmd est un string correspondant à un morceau de la commande lancé par l'utilisateur qui a été parsée
 * \return node
 */
node* createNodeAndLinkNext(node* root, char* cmd) {
    node *temp = createNode(cmd);
    root->next = temp;
    temp->previous = root;
    return temp;
}