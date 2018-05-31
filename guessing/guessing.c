#include "guessing.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
/*prints tree in preorder*/
void printTree(struct node *root)
{
    if( root == NULL){
      return;
    }
    
    printf("%s\n", root->data);
    printTree(root->left);
    printTree(root->right);
}
/*sets all varaibles to NULL*/
struct node *createNode() /*take input here*/
{
    struct node *newNode = malloc(sizeof(node));
    newNode->data = 0;
    newNode->right = NULL;
    newNode->left = NULL;
    newNode->parent = NULL;
    return newNode;
}
void initTree(struct node *root)
{
    printf("What is it?\n");
    root->data = inputData();

}
/* This assumes file always exists and determines whether has been opened and it checks if the file
   is empty and should begin to recreate with an intial condition to begin rewriting the tree.
    ********* Condtion where the file was never created anddentify when it courrpted*********/
int checkFile(FILE *ptr)
{
  int check = 0;
  int size = 0;
  if( ptr == NULL)
  {
      fprintf(stderr,"qa.db: No such file or directory\n");
      fprintf(stderr,"Unable to read database qa.db. Starting Fresh.\n");
      ptr = fopen("qa.db","w+");/*Create file check if its open*/
      check = 1;
      
  }
  else if (ptr != NULL) 
  {
    fseek (ptr, 0, SEEK_END);
    size = ftell(ptr);

    if (size == 0) {
     fprintf(stderr,"Empty Database: Recreating Database\n");
     check = 1;
    }
  else 
    {
      check = 0;
    }

  }
  return check;
}

/* Takes input data returns array */
char *inputData()
{
  int c = 0;
  size_t i = 0;
  size_t capacity = 10; /* Sets Max to 10*/
  char *arr = NULL;
  char* temp = NULL;
  arr = (char*) malloc(capacity * sizeof(char));
  while ((c = getchar()) != '\n')
  { 
    if(capacity == (i+2))
	{
	   capacity *= 2 ;
	   temp = (char*)realloc(arr,capacity * sizeof(char));
        /*Assigns Address to newly allocated array*/ 
       arr = temp;
	}
    arr[i] = c;
    i++;
  }
  arr[i] = '\0';
return arr;
}
/* fill the tree in preorder from the file*/
struct node *populate(FILE *fp,struct node *root)
{ /*reading char value*/
   char *data = readFile(fp);
   if(data[0] == ' ') /*possible issues with usingn null charater?*/ /*statement is true for both EOF and spaces*/
   {
     return root;
   }
   if(data[0] != ' ')
   {
   root = createNode();
   root->data = data;
   root->left = populate(fp,root->left);
   root->right = populate(fp,root->right);
   }
   return root;
}
void populateFile(FILE *fp,struct node *root)
{
   if( root == NULL){
      return;
   }
   fprintf(fp,"%s",root->data);
   populateFile(fp,root->left);
   fprintf(fp,"%s","\r");
   populateFile(fp,root->right);
   return;
}

/*Reads lines from the file until newline or EOF is reached returns char array*/
char *readFile(FILE *fp)
{
  int d = 0;
  int c = 0;
  int i = 0;
  size_t capacity = 10; 
  char *arr = NULL;
  char* temp = NULL;
  arr = (char*) malloc(capacity * sizeof(char));
 while (((c = fgetc(fp)) != '\n') && (c != EOF))
  { 
    if(capacity == (i+2))
	{
	   capacity *= 2 ;
	   temp = (char*)realloc(arr,capacity * sizeof(char));
        /*Assigns Address to newly allocated array*/ 
     arr = temp;
	}
    arr[i] = c;
    i++;
  }
d = arr[0];
 arr[i] = '\0';
if(isspace(d)) /* Line spaces to move up*/
  {
    printf("Got here SPACE\n");
    arr[0] = ' ';
  }
 else if(c == EOF)/*if its an end of file return*/
 {
    printf("Got here EOF \n");
    arr[0] = ' ';
 }
return arr;
}
