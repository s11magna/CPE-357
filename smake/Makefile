CC = gcc
CFLAGS = -Wall -ansi -pedantic -g
MAIN = smake
OBJS = driver.o smake.o
all : $(MAIN)

$(MAIN) : $(OBJS) smake.h
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

driver.o : driver.c smake.h
	$(CC) $(CFLAGS) -c driver.c

smake.o : smake.c smake.h
	$(CC) $(FLAGS) -c  smake.c

clean:
	rm *.o $(MAIN) core
