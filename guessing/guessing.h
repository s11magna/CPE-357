#ifndef GUESSING_H
#define GUESSING_H
#include <stdio.h>
/*Tree structure contains a parent, left child , roght child and data field*/
typedef struct node{
char *data; /*input needs to grow dynamically for input*/
struct node* left;
struct node* right;
struct node* parent;
}node;
void populateFile(FILE *fp, struct node *root);
struct node *populate(FILE *fp, struct node *root);
void printTree(struct node *root);
struct node *createNode();
int checkFile(FILE *ptr);
void initTree(struct node *root);
char *inputData();
char *readFile(FILE *fp);
struct node *freeData(struct node *root);
#endif