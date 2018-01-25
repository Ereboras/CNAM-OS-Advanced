#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
  unsigned int key;
  struct node *left;
  struct node *right;
} Node;

/* @function : addNode
 * @description: Permet d'ajouter un noeud dans l'arbre
 * @param : node **tree
 * @param : unsigned int key
  * @return : nothing
*/
void addNode(node **tree, unsigned int key);

/* @function : searchNode
 * @description: Permet de chercher un noeud spécifique enregistré dans l'arbre
 * @param : node **tree
 * @param : unsigned int key
 * @return : int
*/
int searchNode(node *tree, unsigned int key);

/* @function : clearTree
 * @description: Permet de supprimer tous les noeuds de l'arbre
 * @param : node **tree
 * @return : nothing
*/
void clearTree(node **tree);
