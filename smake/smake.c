#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smake.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> 


struct treeNode *newNode(char* data)
{
    struct treeNode *newNode = malloc(sizeof(treeNode));/*creates new node*/
       newNode->child = NULL;
       newNode->data = data;
       newNode->actions = NULL;
       newNode->next = NULL;
       newNode->parent = NULL;
     return newNode;
}

void addSibling(struct treeNode *node,struct treeNode *curNode) /*This will create a new node in any case*/
{
    while (node->next != NULL){ /* if the child is not node then find end of the silbing nod and add there*/
        node = node->next; /*ADD TO THE NEXT SIBLING*/
    }
    node->next = curNode; /*ADD  the next sibiling*/
}
void addChild(struct treeNode *node, struct treeNode *curNode) /*this will also create a new node in any instance*/
{
    if (node->child != NULL){ 
        /*if the child root already exists then add the next node as its sibling*/
          addSibling(node->child, curNode);
        }
    else{
         /* if is not a node then create then add it as the sibling*/
        node->child = curNode;
    }
}
struct lNode* argumentRead(lNode *headPtr, char* argv[])
{
    lNode *found = NULL;
    found = findDepen(headPtr,argv[1]);
    if(found != NULL)
    {
      return found;
    }
    else{
    fprintf(stderr,"make: *** No rule to make target <argument>. Stop.\n");
    exit(-1);
    return headPtr;
    }
}
char *readFile(FILE *fp)
{
  int c = 0;
  int i = 0;
  size_t capacity = 10;
  char *arr = NULL;
  char *temp = NULL;
  arr = (char *)malloc(capacity * sizeof(char));
  while (((c = fgetc(fp)) != EOF) && (c != '\n')) 
  {
    if (capacity == (i + 2))
    {
      capacity *= 2;
      temp = (char *)realloc(arr, capacity * sizeof(char));
      /*Assigns Address to newly allocated array*/
      arr = temp;
    }
    arr[i] = c;
    i++;
  }
    arr[i] = '\0';
  if(c == EOF) /*conditional statement*/
{
   arr[0] = -1;
}
  return arr;
}

struct lNode* createNodeL() /*possibly change these functions to allow for setting data fields ALSO this is the big linkedlist*/
{
    struct lNode* newNode = malloc(sizeof(lNode));
    newNode->target = NULL;
    newNode->headptr_actions = NULL;
    newNode->next = NULL;
    newNode->headptr_depen = NULL;
    return newNode;
}

/*smaller linked list helper functions*/
struct sNode* createNodeS() /*possibly change these functions to allow for setting data fields ALSO SMALL LINKED LIST*/
{
   struct sNode* newNode = malloc(sizeof(sNode));
   newNode->data = 0;
   newNode->next = NULL;
   return newNode;
}
struct sNode *addNodeS(sNode *head, char* value) { /*smaller linked list*/
  struct sNode *newNode = createNodeS();/*created new node*/
   newNode->next = NULL;
   newNode->data = value;
    if (head == NULL) { /*if node doesnt exist set equal new node*/
        head = newNode;
    } 
    else {
        sNode *current = head; /*asssigning it the pointer value*/
        while (current->next != NULL) {
            current = current->next;
        }
    current->next = newNode;
    }
    return head;
}

struct lNode *addNodeL(lNode *head, char* target) { /*Larger linked list*/
  struct lNode *newNode = createNodeL();/*created new node*/
  newNode->target = target;
   newNode->next = NULL;
    if (head == NULL) { /*if node doesnt exist set equal new node*/
        head = newNode;
    } 
    else {
        lNode *current = head; /*asssignin   mnhx`z1n/t\
        ';.,mn cg it the pointer value*/
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
    return head;
}
struct lNode *createLL(FILE *ptr, struct lNode *headPtr)
{
    char *tempString = NULL; /*string is used to store pointer from strtok*/
    char *fileString = NULL; /*where string from file is stored*/
    char *stringCopy = NULL; /*strdup stored string to input into linked list*/
    char *stringExtraCopy = NULL;
    char *lastCopy = NULL;
    int c,z = 0;
    int inc = 0;
    int i = 0;
    int check = 0;
    int val = 0;
    lNode *currentNode = NULL;
    fileString = readFile(ptr); /*Grabs the first line from the pointer*/
    while(*fileString != EOF ) /*while the reedline function does not return a EOF*/
    {
     val = 0;
     inc = 0;
     check++;   
        if(!isspace(*fileString))/*first case where string is not a space or a tab*/
        {
            /*first we want to read in start name before the colon*/
            /*This is just for the target*/
            tempString = strsep(&fileString,": ");
             /*makes all delimitors into null characters and moves the pointer*/
            if(tempString != NULL) /*will return Null if there there is no delemits specified found in the string*/
            {
                
                stringCopy = strdup(tempString); /*malloc copy of the string assign;*/ 
                headPtr = addNodeL(headPtr, stringCopy); /* this will work for now because we should always readin target before anything else and each target represent link in the list*/
                currentNode = headPtr;
                while (currentNode->next != NULL) { 
                currentNode = currentNode->next;
                }
            }
        
            else /*safety check*/ 
            {
                printf("Error in case 1 no delimitors\n");
            }

            while((tempString = strsep(&fileString,": ")) != NULL) /*start adding dependencies to the last added head*/
            {
                /*this should grab the last node in the list which should be the most current node*/
                /*This was one in order to add the depencies to the correct node*/
                /*This should work becasue at this point in parsing the head ptr should exist and allow use to add these dependencies*/
                currentNode = headPtr;/*Take address of head pointer and increment to the last node which should be the most current node*/
                while (currentNode->next != NULL) { 
                currentNode = currentNode->next;
                }
                stringCopy = strdup(tempString); /*takes the string copy*/
               
                currentNode->headptr_depen = addNodeS(currentNode->headptr_depen,stringCopy);/* creates head pointer if doesnt exist adds string to the end of the list*/
            }
        tempString = NULL; /*string is used to store pointer from strtok*/
        fileString = NULL; /*where string from file is stored*/
        stringCopy = NULL; 
        }
   else if(*fileString == '\t')
        {
             if(check == 1)
             {
                fprintf(stderr,"Makefile:1: *** recipe commences before first target.  Stop\n");
                exit(-1);
             }
            /*fprintf(stderr,"Makefile:1: *** recipe commences before first target.  Stop"\);*/
            currentNode = headPtr;
            while (currentNode->next != NULL) { /*this should grab the last node in the list which should be the most current node*/
            currentNode = currentNode->next;
            }
         
   stringCopy = strdup(fileString + 1); /*should skip the first adress in the node*/
   currentNode->headptr_actions = addNodeS(currentNode->headptr_actions,stringCopy); /*might havr to iternate to end of linked list each time to add toc current node*/
        }
      fileString = NULL;
      tempString = NULL; /*string is used to store pointer from strtok*/
      stringCopy = NULL;      
      fileString = readFile(ptr); /*Increment and continue*/
    }
   return headPtr;/*pointer to the head pointer``*/
}
void buildTree(lNode *headPtr, treeNode *curNode, treeNode *root, lNode *saved)
{
   lNode* foundTarget = NULL;
    sNode* depenList = NULL; 
     /* steps
     1. start searching main depenedcy list
     2. create first dependecy node for the root(headptr data field)
     3. check for the name in the Larger linked List( target data field)
     4. create the node
     5. if its not found return but it should still create leaf node!!!!
     6. if its found call build tree again with the foundTarget tree node as the new head pointer\
      problem: Does call return when foundTarget still allow it to pass null or will it go back up the parent?
      Also should  always be doing this*
     7. Once its done it create all node for tree it should start linking all childern then finally
     connnect it back to the main root
     8. Done*/
/*get head pointer of first link list*/
   if( headPtr == NULL) /*head pointer is also foundTarget which will be returned before entering the loop*/
    {
        return;
    }
    depenList = headPtr->headptr_depen;
    while(depenList != NULL){ /*while the main linked list is not NULL go though each link*/
    foundTarget = findDepen(saved, depenList->data); 
     /*grab the current node of the depenedency link list to see if it exists in the main list*/
    /* Create the New Node to be added to the tree and to intalize it main field*/
    curNode = newNode(depenList->data);
   // printf("%s\n",curNode->data);
    curNode->parent = root;  
    if(foundTarget != NULL)
    {
        curNode->actions = foundTarget->headptr_actions;
    }
    /*assign found target of the main link list*/
    /*curNode->actions = foundTarget->headptr_actions;/*Assign the actions*/
    buildTree(foundTarget,root,curNode,saved);
    addChild(root,curNode);
    
    /*I am assigning the headPtr again which mean it will infinite look becasue its always finding the an thing*/
    /*How should search the further linked nodes if its always looking back because it not incrementing*/
    /*add the finished node to the dependency of the root node we are looking at*/
        /*increment though the linked list*/
     depenList = depenList->next;
    }
}
struct lNode* findDepen(lNode *head, char* key) /*I want to use this to find targets in the*/ 
{
int c = 0;
while (head != NULL)
    {
      if((c = strcmp(head->target,key)) == 0)
      {
          return head;
      } 
      head = head->next;
    }
    return NULL;
}
void execAct(treeNode *root)
{ 
    int status = 0;
    int i = 0;
    int d = 0;
    const char concat[8] = "echo ";
    char Result[200] = {0};
    sNode* actionN = root->actions;
    while(actionN != NULL) /*iterate though the node passed though*/\
    {
       strcpy(Result, concat);
       strcat(Result,"\'" );
       strcat(Result,actionN->data); 
       strcat(Result,"\'");
       status = system(Result);
       status = system(actionN->data); /*run the actions passed into the linked list*/
       if(status < 0)/*catch the errors*/
       {
       fprintf(stderr,"Compilation Error");
       exit(-1);
       }
        actionN = actionN->next;/*continue thought the rest of the linked list*/
    }
}
void compile(treeNode *root)
{
struct stat curNode = {0};
struct stat Parent = {0};
int stat1 = 0;
int stat2 = 0;
char fg[5] = "echo";
if(root == NULL)
{
    return;
}
compile(root->child);
compile(root->next);
 if(root != NULL){
    execAct(root);
 }
}

void checkifallUpdated(treeNode *root, int *i)
{
struct stat curNode = {0};
struct stat Parent = {0};
int stat1 = 0;
int stat2 = 0;
  if(root == NULL)
  {
    return;
  } 
checkifallUpdated(root->child,i);
checkifallUpdated(root->next,i); 
if((root != NULL) || (root->actions != NULL))
    {
        if(root->parent != NULL)
        {
            
            stat1 = stat(root->data,&curNode);
            stat2 = stat(root->parent->data, &Parent);
            if(curNode.st_mtime > Parent.st_mtime)
            {
                *i = 1;
            }
        }
        else if(root->parent == NULL)
        {
            stat1 = stat(root->data,&curNode);
            stat2 = stat(root->child->data, &Parent);
        if(curNode.st_mtime < Parent.st_mtime)
            {
                *i = 1;
            }
        }
       
    }
}
void FreeEveryting(treeNode *root)
{
    if(root == NULL){
    return;
    }
FreeEveryting(root->child);
FreeEveryting(root->next);
free(root->child);
free(root->next);
free(root->parent);
free(root->actions);
free(root->data);
}
/*void depedencySearch(lNode *headPtr)
{
   lNode *found = NULL;
   sNode *depenHead = NULL;
   depenHead = headPtr->headptr_depen;
   while(depenHead != NULL)
   {
     found = findDepen(headPtr, depenHead->data); 

     if(found == NULL)
     {
        fprintf(stderr,"make: *** No rule to make target %s, needed by %s. Stop.\n", depenHead->data,headPtr->target);
        exit(-1);
     }
     depenHead = depenHead->next;
   }
}*/
