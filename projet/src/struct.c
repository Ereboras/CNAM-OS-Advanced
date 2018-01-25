#include "../headers/struct.h"
#include <stdlib.h>
# include <stdio.h>

void addNode(node **tree, unsigned int key)
{
    node *tmpNode;
    node *tmpTree = *tree;

    node *elem = 0;
    elem = malloc(sizeof(node));
    elem->key = key;
    elem->left = 0;
    elem->right = 0;

    if(tmpTree)
    do
    {
        tmpNode = tmpTree;
        if(key > tmpTree->key )
        {
            tmpTree = tmpTree->right;
            if(!tmpTree) tmpNode->right = elem;
        }
        else
        {
            tmpTree = tmpTree->left;
            if(!tmpTree) tmpNode->left = elem;
        }
    }
    while(tmpTree);
    else  *tree = elem;
}

int searchNode(node *tree, unsigned int key)
{
    while(tree)
    {
        if(key == tree->key) return 1;

        if(key > tree->key ) tree = tree->right;
        else tree = tree->left;
    }
    return 0;
}

void clearTree(node **tree)
{
    node *tmpTree = *tree;
    if(!tree) return;
    if(tmpTree->left)  clearTree(&tmpTree->left);
    if(tmpTree->right) clearTree(&tmpTree->right);

    free(tmpTree);      
    *tree = 0;
}
