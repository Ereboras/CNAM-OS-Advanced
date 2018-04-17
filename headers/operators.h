#ifndef FILE_OPERATORS_HEADER
#define FILE_OPERATORS_HEADER

#include <stdio.h>
#include "../headers/typedef.h"
#include "../headers/execution.h"

void listOperator(node* currentNode);

void pipeOperator(node* currentNode);

void logicalOperatorAnd(node* currentNode);

void logicalOperatorOr(node* currentNode);

void redirectOperatorSimpleLeft(node* currentNode);

void redirectOperatorSimpleRight(node* currentNode);

void redirectOperatorDoubleLeft(node* currentNode);

void redirectOperatorDoubleRight(node* currentNode);

#endif