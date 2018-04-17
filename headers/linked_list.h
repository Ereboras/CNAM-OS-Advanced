#ifndef FILE_TREE_HEADER
#define FILE_TREE_HEADER

#include "../headers/typedef.h"

/**
 * \fn createNode(char* cmd)
 * \brief Allow to create a simple node .
 * 
 * \param cmd est un string correspondant à un morceau de la commande lancé par l'utilisateur qui a été parsée
 * \return node
 */
node* createNode(char* cmd);

/**
 * \fn createEmptyNode()
 * \brief Create an empty node.
 * \return node
 */
node* createEmptyNode();

/**
 * \fn createNodeAndLinkNext(node* root, char* cmd)
 * \brief Create a node and place it next to the root node.
 * The function return the created node.
 * 
 * \param root The root node
 * \param cmd The new node will be created with this command as node->command
 * \return node
 */
node* createNodeAndLinkNext(node* root, char* cmd);

#endif