#include "mytimer.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
int main(int argc, char* argv[])
{
  char i = 'x'; //intialize to x;
  ON = 1;
  int s = 0;
  char *t = NULL;
  struct termios newAtr;
  //check arguments
  if(argc != 2){
      fprintf(stderr,"usage: mytimer <seconds>\n");
      exit(-1);
   }
    for(s = 0;argv[1][s] != '\0';s++)
      {
       
       if(argv[1][s] > 57 || argv[1][s] < 48)      
           {
      fprintf(stderr,"%s: malformed time.\nmytimer <seconds>\n",argv[1]); 
      exit(-1);

         }                   
      }
  
  TIME = strtod(argv[1],&t);
  if(TIME < 0)
  {
   fprintf(stderr,"Invalid time(-1). Must be >= 0.\nusage: mytimer <seconds>\n");
    exit(-1);
  } 
  if(TIME >= LONG_MAX) {
  fprintf(stderr, "%s: time out of range.\n",argv[1]);
  fprintf(stderr,"usage: mytimer <seconds>\n");
  exit(-1);
 }
  intalizeTermAtrr(newAtr); // setting termial attributes
  tick(); // get intervals and setting alarm timer started 
  TIME += (1 - SUBTRACT_CONST); // alittle before the second ends
  for(;;){
     read(STDIN_FILENO, &i ,1); //Take input from terminal
     if( i == 'q'){ // quit
      printf("\n");//new line
      fflush(stdout);
      resetTermAttr(savedAtr);
      exit(1);// successful exit!
    }
  if((i == 'c')||(i == 'h') || (i == 'H') || (i == 'm') ||
     (i == 'M') || (i == 's') || (i  == 'S')||(i == 'r')) // still need to add run stop
      {
         changeClock(i);
         i = 'x'; // reset back
      }
  }     
  
  return 0;
}
