#ifndef SMAKE_H
#define SMAKE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> 

#define BUFFER_SIZE 1024

extern char *strsep(char **stringp, const char *delim);
extern char *strdup(const char *s);

/*Tree structure elements*/
typedef struct treeNode{
char *data; 
struct treeNode* child; 
struct treeNode* parent;
struct treeNode* next;
struct sNode* actions;
}treeNode;
/*smaller linked list structure*/
typedef struct sNode
    {   char* data; /*takes in dependents*/
        struct sNode *next;  /*points to the next node*/
}sNode;
/*larger linked list structure*/
typedef struct lNode{
    char* target;/*target string*/
    struct sNode* headptr_depen;/*small linked list to store depen*/
    struct lNode* next; /*point to the next node of the larger*/
    struct sNode* headptr_actions; /* new pointer for each linked list of actions*/
}lNode;

/*tree node helper functions*/
struct treeNode* newNode(char* data);
void addSibling(struct treeNode *n, struct treeNode *curNode);
void addChild(struct treeNode *n, struct treeNode *curNode);

/*Larger linked list helper fucnctions*/
struct lNode *createNodeL(); /*possibly change these functions to allow for setting data fields*/
struct lNode *addNodeL(lNode *head, char* target);
struct lNode *createLL(FILE *ptr, struct lNode *headPtr);
struct lNode *findDepen(lNode *headptr, char* target);

/*smaller linked list helper functions*/
struct sNode *createNodeS();/*possibly change these functions to allow for setting data fields*/
struct sNode *addNodeS(sNode *head, char* value);

/*file parsing elements*/
char *readFile(FILE *ptr); /* reads file lines line by line*/
/*Parsing Function*/
/*Steps 
1. Run until EOF using readline function
2. Using StrTOK or STRsep with delimitors for each time
3. when reading each line check for \t and spaces
4. when its a tab store into the action char no matter the size
5. target which should be one it stored into the string
6. a new node will be whenever the time is no a tab or spaces since
will signify the beginning of a new target which means a new node*/
/*Tree functions*/
void buildTree(lNode *head ,treeNode *node, treeNode *root,lNode *saved);

/*compliation functions*/
void execAct(treeNode *root);
void compile(treeNode *root);
void FreeEveryting(treeNode *root);
struct lNode *argumentRead(lNode *headPtr, char* argv[]);
void checkifallUpdated(treeNode *root, int *i);
void depedencySearch(lNode *headPtr);
#endif
