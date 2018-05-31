#ifndef MYTIMER_H
#define MYTIMER_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h> 
#include <string.h> 
#include <math.h>
#include <sys/time.h>
//Ticks Per Second
#define TICKS_PER_SECOND 25
#define HOURS 3600
#define MIN 60
#define SUBTRACT_CONST .04
extern double TIME;
extern int TFalse;
extern int ON;
extern struct termios savedAtr;
//set terminal Attributes
void intalizeTermAtrr(struct termios newAtr);
//restore terminal attriburtes
void resetTermAttr(struct termios savedAttr);
//set Timer: Command H,h,M,m S,s,C,R
//implement Run/Stop
void changeClock(char i);
//intalize tick sets the timer and set ALARM Function
void tick();
void twirlerP();
   
//Function for what happens once SIGALRM is sent
void alarmHandler();
void printTime(int boolern);
#endif

