#ifndef FILE_TYPEDEF_HEADER
#define FILE_TYPEDEF_HEADER

typedef enum { false, true } bool;

typedef struct node {
    char *command;
    int success;
    FILE *response;
    bool executed;
    struct node *previous;
    struct node *next;
} node;

#endif