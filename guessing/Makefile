CC = gcc
CFLAGS = -Wall -ansi -pedantic -g
MAIN = guessing
OBJS = testing.o guessing.o
all : $(MAIN)

$(MAIN) : $(OBJS) guessing.h
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

mainf.o : testing.c guessing.h
	$(CC) $(CFLAGS) -c mainf.c

myTr.o : guessing.c guessing.h
	$(CC) $(FLAGS) -c  myTr.c

clean:
	rm *.o $(MAIN) core
