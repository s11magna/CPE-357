CC = gcc
CFLAGS = -Wall -pedantic -std=c99 -lm -D_POSIX_C_SOURCE
MAIN = mytimer
OBJS = main.o mytimer.o
all : $(MAIN)

$(MAIN) : $(OBJS) mytimer.h
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

main.o : main.c mytimer.h
	$(CC) $(CFLAGS) -c main.c

mytimer.o : mytimer.c mytimer.h
	$(CC) $(FLAGS) -c  mytimer.c

clean:
	rm *.o $(MAIN) core

