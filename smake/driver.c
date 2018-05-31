#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> 
extern char *strsep(char **stringp, const char *delim);
extern char *strdup(const char *s);
extern int strcasecmp(const char *s1, const char *s2);
#include "smake.h"
#include <ctype.h>
int main( int argc, char *argv[])
{       

   lNode *headPtr  = NULL; 
   lNode *argument = NULL; 
   treeNode* root = NULL;
   treeNode* curNode = NULL;
   int  i = 0;
   int size = 0;
   FILE *ptr = fopen("Smakefile","r");
   struct stat st;
   stat("Smakefile", &st); 
   size = st.st_size;
   if( ptr == NULL)
   {
       fprintf(stderr,"make: *** No targets specified and no makefile found. Stop.\n");
       exit(-1);
       /*File doesnt Exist*/
   }  
   if(size < 3)
   {
       fprintf(stderr,"make: *** No targets. Stop.\n");
       exit(-1);
   }
   
  headPtr = createLL(ptr,headPtr);
    /*take in smaller linked list so we know where to go*/
  if(argv[1] != NULL)
      {
      argument = argumentRead(headPtr, argv);
      }
  else
      {
          argument  = headPtr;
      }

  root = newNode(argument->target);/*make this a function later*/
  root->actions = argument->headptr_actions;
  buildTree(argument,curNode,root,headPtr);
 /*checkifallUpdated(root, &i);*/
  compile(root);
  return 0;
}


