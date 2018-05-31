#include "simple_net.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h> 
#include "httpd.h"
#include <signal.h>
int main(int argc, char *argv[])
{
    pid_t pid;
    
    struct sigaction sa;
     if(argc != 2){
    fprintf(stderr,"NOT ENOUGH ARGUMENTS SPECIFIY PORT\n");
    exit(1);
    }
  portNumber = atoi(argv[1]);
  printf("Attempting to connect via port %d\n",portNumber);
  if((socketFD = create_service(portNumber,queueSize)) < 0 )
  { 
   fprintf(stderr,"FAILED TO CREATE SERVICE\n");
  }    
  while(1){
 sa.sa_handler = sigchld_handler;
 sigemptyset(&sa.sa_mask);
 sa.sa_flags = SA_RESTART;
  if((clientFD = accept_connection(socketFD)) < 0){
	fprintf(stderr,"Failed to Connect\n");
         exit(1);	
        }     

 	if((pid = fork()) < 0){
 	   /* fix later need to exit gracefully
 	   // clean this shit up later */
 	   errorHandeling(clientFD,500);
        }
   	else if(pid  == 0){
           /* I am the child!
           // start reading in information
           // make space for the null character*/
          if(recv(clientFD,buff, BUFFSIZE - 1,0) < 0)
             {
             exit(-1);
            } 
          determineRequest(portNumber,clientFD,buff);
          child_count++;
       exit(1);
      }
       else{
         /* parent*/
   if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
       }
    }
  }

return 0;

}
