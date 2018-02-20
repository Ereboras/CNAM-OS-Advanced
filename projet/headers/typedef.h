#ifndef FILE_TYPEDEF_HEADER
#define FILE_TYPEDEF_HEADER

typedef enum { false, true } bool;

typedef struct node {
    char *command;
    bool success;
    char *response;
    struct node *left;
    struct node *right;
} node;

#endif