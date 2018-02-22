#ifndef FILE_TREE_HEADER
#define FILE_TREE_HEADER

#include "../headers/typedef.h"

/* @function : createRoot
 * @description: Create a node with command
 * @param : char* cmd
 * @return : node*
*/
node* createNode(char* cmd);

/* @function : createEmptyNode
 * @description: Create new empty nodes
 * @return : node*
*/
node* createEmptyNode();

/* @function : createNodeAndLinkNext
 * @description: Create node and link root with the new
 * @param : node *root
 * @param : node *cmd
 * @return : node*
*/
node* createNodeAndLinkNext(node* root, char* cmd);

#endif