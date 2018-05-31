#include "httpd.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

int queueSize = 0;
int socketFD = 0;
int clientFD = 0;
int portNumber = 0;
int fileSze = 0;
char buff[BUFFSIZE] = {0};
int child_count = 0;
void determineRequest(int port, int clientFD, char* inputC)
{
   int fd;
   char *fileName = NULL;
   char * cmd;
   // There is space between the the first request EITHER GET OR HEAD
   cmd = strsep(&inputC," "); 
   // we should not longer have a get
   // The input no longer contains 
   if(strcmp(cmd, "GET") == 0)
   {
       
      fileName= strdup(inputC + 1);      
      fd = printHeader(port, clientFD, fileName);
      printFile(clientFD, fd);
      free(fileName); 
      close(fd); 
   }
   else if(strcmp(cmd, "HEAD") == 0)
   {
     fileName = strdup(inputC +1);
     /*This creates for a path that doesnt include the first backslash
     now we need to next check if its one a path and two if that path conta   ins CGI-Like */
     fd = printHeader(port, clientFD, fileName);
     free(fileName);
     close(fd);
    }  
  else
   {
      errorHandeling(clientFD,400);
   }
}
int fileExists(char *file)
{
    struct stat buf;
    if(lstat(file, &buf) == 0){
       return 1;
   }
  else
  {
   return 0;
 } 
}
int printHeader(int port, int clientFD, char* fileName){
     char HEADER[40] = {0}; //header for the head function
     struct stat fileD; //  stat
     char* cigPath = NULL; // Copy of the filename for testing
     int FD = 0; // FILE DESCRIPOR 
     char* path = NULL;
     int exists = 0;

     /* to test the new file we first need to make a copy of it becuase strstok ruins the orginal*/    
     /*to send to check it is cgi-like*/
      path = strdup(strsep(&fileName," "));
      
      cigPath = strdup(path);
  /*we need to do this in order to get rid of the HTTP bull shit the end of the input*/
     if(strcmp(strsep(&cigPath,"/"),"cgi-like") == 0)
   {
      cgi_like(port, clientFD, path);
   }
   else { 
  /*test to make sure it could be cgi like*/
   if((exists = fileExists(path)) == 1) {

    if((FD = open(path,O_RDONLY)) < 0)
      { 
        errorHandeling(clientFD,403);
      }
    /* add support for CGI_LIKE*/
    sprintf(HEADER,"HTTP/1.0 200 OK\r\n");
    if(send(clientFD,HEADER,strlen(HEADER),0) < 0)
     {
      perror("Send error in opening the file!\n");
     }
    /*add file exists error checking*/
    fileSze = fileSize(path);
    send(clientFD,"Content-Type: text/html\r\n",strlen("Content-Type: text/html\r\n"),0);
    memset(&HEADER,0,40);
    HEADER[0] = '\0';
    sprintf(HEADER,"Content-Length: %d\r\n",fileSze);
    send(clientFD,HEADER,strlen(HEADER),0);
    send(clientFD,"\r\n",strlen("\r\n"),0);
    return FD;
    } 
  
   else{
   errorHandeling(clientFD,404);   
  }
 }
  return FD;
  
}
int printFile(int clientFD, int FD)
{ 
    char *buf = (char*)malloc(fileSze * sizeof(char));
    int total = 0;   
    int len = fileSze;
    int bytesleft = len; 
    int n;
    if(read(FD,buf,fileSze) == -1)
    {
       perror("Some reading error\n");
    }  
    while(total < len) {
        n = send(clientFD, buf+total, bytesleft, 0);
        if (n == -1) { break; }
        total += n;
        bytesleft -= n;
    }
    len = total; 
    free(buf);
    return n==-1?-1:0; 
}
void errorHandeling(int clientFD, int errorCode){
   char err[BUFFSIZE];
   printf("ERROR!\n");
   printf("ERROR %d",errorCode);
   switch(errorCode) {
      case 400:
         sprintf(err, "HTTP/1.0 %s\r\n", "400 Bad Request");
         clientFD;
      break;
      case 403:
         sprintf(err, "HTTP/1.0 %s\r\n", "403 Permission Denied");
          clientFD;

      break;
      case 404:
         sprintf(err, "HTTP/1.0 %s\r\n", "404 Not Found");
              
      break;
      case 500:
   	 sprintf(err, "HTTP/1.0 %s\r\n", "500 Internal Error");
      break;
      case 501:
         sprintf(err, "HTTP/1.0 %s\r\n", "Not Implemented"); 
      break;   
   }
    send(clientFD,err,strlen(err),0);
    send(clientFD, "Content-Type: text/html\r\n", strlen("Content-Type: text/html\r\n"),0);
  
exit(1);
}
int fileSize(char* path)
{
    int fileSize = 0;
    struct stat fileD;
   /*creating intialize header*/
    if(lstat(path,&fileD) < 0){
        fprintf(stderr, "file doesn't exist\n");
	exit(-1);
     }
    fileSize = (int) fileD.st_size;
    /*sending header*/
   return fileSize;
}
void cgi_like(int port, int clientFD, char* cmd)
{
   int status; // child to return;
   pid_t pid; // pid of the child proccess which I will use in the file naming
   int fout = 0;
   char *cmds[60]; //2d array to hold the arguments
   char *args; //to hold the intalize CGI_LIKE content
   char *cgipath; //CGI_Like intial directory
   int i = 0;
  
   //Hold the CGI_LIKE 
   cgipath = strsep(&cmd, "/");
   char fileName[1024];
   // start iterating thought the arguments
   // eacg argument that follows in the set
   while((args = strsep(&cmd, " ?&")))
   { 
      cmds[i] = args;
      ++i;
   }
   //set the last argument in the list because it starts at zero to null
   //becuse that what execvp expects 
   cmds[i] = NULL;
   // now that we've loaded up all the arguments its time to fork
   if(((pid = fork()) < 0))
   {
     errorHandeling(clientFD,500); //fork failed
   }
   else if(pid == 0) 
   {
      char path[1024];
      // load the path arguments in execv
      sprintf(path, "./%s/%s", cgipath, cmds[0]);
      
      sprintf(fileName, "cgi-%d.txt", getpid());
       //new file in order to store contents
  fout = open(fileName, O_CREAT| O_WRONLY| O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
      // push all the standard output to the files
      dup2(fout, 1);
      //execv lets do this!
      execv(path, cmds);
   }

 else{
   sprintf(fileName, "cgi-%d.txt",(int) pid);
   wait(&status);
   int fDS = 0;
   // push it all out! let do some looping!
   // potential issues???
   printHeader(port, clientFD, fileName);
   fDS = open(fileName,O_RDONLY);
   printFile(clientFD, fDS);
   unlink(fileName);
   exit(1);
 }
}
void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
   
      while(waitpid(-1, NULL, WNOHANG) > 0){
         child_count--;
     }
 }
