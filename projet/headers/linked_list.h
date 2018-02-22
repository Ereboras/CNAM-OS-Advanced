#ifndef FILE_TREE_HEADER
#define FILE_TREE_HEADER

#include "../headers/typedef.h"

/* @function : createRoot
 * @description: Permet d'ajouter un noeud dans l'arbre
 * @param : char* command
  * @return : node*
*/
node* createNode(char* cmd);

/* @function : addRightChild
 * @description: Add child to the right side
 * @param : node *root
 * @param : node *child
 * @return : nothing
*/
void addRightChild(node *root, node *child);

/* @function : addLeftChild
 * @description: Add child to the left side
 * @param : node *root
 * @param : node *child
 * @return : nothing
*/
void addLeftChild(node *root, node *child);

#endif