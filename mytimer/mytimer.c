//This function sets the new terminal attributes
//Disabling ECHO and Setting it to nonconcial modea
#include "mytimer.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <string.h>
#include <math.h>
//global variable 
double TIME = 0;
int TFalse = 1;
int ON = 1;
double testing = SUBTRACT_CONST;
struct termios savedAtr;
struct sigaction sa;
struct itimerval timer;


// TIME needs to be global in order for signal handler to read it in
// This is for the twirler to know the postion and also to reset it
int twirler = 0;
void intalizeTermAtrr(struct termios newAtr)
{
   //Make sure we are writing to the terminal
  //Save the terminals pervoius atributes
   tcgetattr(STDIN_FILENO, &savedAtr);
   //Create new terminal attributes 
   tcgetattr (STDIN_FILENO, &newAtr);
   newAtr.c_lflag &= ~(ICANON|ECHO); // Clear ICANON for nonconcical mode and stop echo
   newAtr.c_cc[VINTR] = 0; //ctrl-c turn off the kill 
   newAtr.c_cc[VEOF] = 0; //ctrl-d EOF to terminal turn it off
   newAtr.c_cc[VSUSP] = 0; //ctrl-z turn off the suspend
   newAtr.c_cc[VTIME] = 0;// number of characters to read in
   tcsetattr (STDIN_FILENO, TCSAFLUSH, &newAtr);// Set there atrributes
}

void resetTermAttr(struct termios savedAttr)
{
  // return the old terminal atrributes
  // This should be done only when 'q' is pressed
  tcsetattr(STDIN_FILENO, TCSANOW,&savedAttr);
}
// Adjust Time
void changeClock(char i)
 {  //set the time for H,h M,m S,s,C,R
   sigset_t sig;
   sigemptyset(&sig);
   sigaddset(&sig,SIGALRM);
    if(i == 'r') // run/stop
    {
      if(ON == 1) // stop
       { 
          ON = 0; // turn it off
        setitimer (ITIMER_REAL, 0, NULL); 
 //This block all alarm signals to stop incrementing
      } 
      else{ // run 
         ON = 1;// it must be back on
         setitimer (ITIMER_REAL, &timer, NULL); 
      }
    }
  
  if( i == 'c')
  {
   //sigprocmask(SIG_BLOCK,&sig,NULL);//block_signal
   TIME = (1 - SUBTRACT_CONST);
       
   TFalse = 0;
   
  }

   switch(i) {
         case('H'):
         TIME -= 3600; //minus an 1 hour
         break;

         case('h'): 
         TIME += 3600;//plus an hour
         break;

         case ('M'):
         TIME -= 60;// minus an hour
         break;

	case ('m'):
        TIME += 60;// plus a minute
        break;
       
        case ('S'):
         TIME -= 1;// minus a second
         break;

       case ('s'):
         TIME += 1;// plus a second
        break;

       default:
       break;
         
  }
 printTime(0);
}     
// implement Run/stop later
 

void tick()
{
  // to set SIGALARM ACTION
  // to set up when the SIGARLAM is going to be set

  //set up sigaction to catch signal SIGALRM
  memset (&sa, 0, sizeof (sa)); // init to zero
  sa.sa_handler = &alarmHandler; //pass function that will go off after each tick
  sigaction(SIGALRM,&sa,NULL); //alarm to be called

  //Configure the timer to expire after MICRO_TIME
  timer.it_value.tv_sec = 0; 
  timer.it_value.tv_usec =  1000000 /TICKS_PER_SECOND;
  //and every MICRO_TIME after that
  timer.it_interval.tv_usec = 1000000 / TICKS_PER_SECOND ;
   //set timer
  setitimer (ITIMER_REAL, &timer, NULL); 
 
}

void twirlerP()
{

 if(twirler == 0)
   { 
      printf("\r|");//first postion 
      twirler += 1;
   }
   else if(twirler == 1)
   {
     printf ("\r/");// second postion
     twirler += 1;
   }
   else if(twirler == 2)
   {
     printf("\r-");// third postion
      twirler += 1;  
   }
   else
   {
     printf("\r\\");// fourth postion
     twirler  = 0; //reset twirler back to orginal postion
   }
}

// Function for action to be taken once the SIGALRM is caught
void alarmHandler()
{
  printTime(TFalse);
  TIME -= SUBTRACT_CONST;// for the next time 
  TFalse = 1;
}

//Use as a true falseZZzzzzzzzzzzzzzzzzz\ to determine whether we are normally decrementing
//the clock or if we are changing the time in which we should not be doing anthing
void printTime(int TFalse)
{
   double rounded_up =  ((int)(TIME * 100 + .5) / 100.0);
   int timeT = fabs(floor(rounded_up));


  // if its one its not being updated which means if the timer hits zero it must done
  // else if its not 1 then its being updated so even if the timer reaches zero in this instant
  // its should not go off
  printf("              \r");// format for "twirler_-h:mm:ss 
  
  if(rounded_up == 0.96 && TFalse == 1)
   {
     printf("\a");// prints the bell character
     printf("\rBeeeep! Time's up!\n"); // Timer is u
   }
    if(TFalse == 0)
  {
     TFalse = 1;
  }
   // knowing the position of the twirler
     // to set negative time or not
   twirlerP();
   //q
   if(rounded_up >= 0)
   {
      printf("  "); // two spaces     
   }
   else
   {
      printf(" -"); // one space and a negative sign
   }
   // format printing
   printf("%d",timeT/HOURS);// Divide up seconds to Hours
   timeT -= ((timeT/HOURS)* MIN);// subtract leftovers
   printf(":%02d",timeT/MIN);// no divide into minutes
   TFalse;
   timeT -= ((timeT/MIN)*MIN); // subtract left overs
   printf(":%02d\r",timeT);// print the rest
    TFalse;
   fflush(stdout);
}
 
