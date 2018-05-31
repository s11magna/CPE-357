#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv [])
{
    char arr[100];
    FILE *fp;
    fp = fopen("qa.db", "r");
    if(fp == NULL)
    {
        fprintf(stderr, "File Failed to open\n");
    }
    if(fgets(arr,100,fp) != NULL)
    {
        printf("%d %d",arr[0],arr[1]);
    }
    fclose(fp);
    return 0;
}