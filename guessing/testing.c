#include "guessing.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
int main(int argc, char *argv[])
{   
    int c = 0;
    node *root = createNode();
    FILE *fp = NULL;
    fp = fopen("qa.db","r+");
    c = checkFile(fp);
    if(c == 1){
        initTree(root);
        printf("Intalzied Root\n");
    }
    else if(c == 0){
       root = populate(fp,root);
       printf("populated root\n");
    }
   /* fclose(fp);*/
    printf("C values %d\n",c);
 /*   fp = fopen("qa.db","w+");*/
    printf("%d\n",c);
    populateFile(fp,root);
    printTree(root);
    fclose(fp);
    return 0;
}