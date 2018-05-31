CC = gcc
CFLAGS = -Wall -pedantic -g
MAIN = httpd
OBJS = driver.o httpd.o simple_net.o
all : $(MAIN)

$(MAIN) : $(OBJS) httpd.h simple_net.h
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

simple_net.o : simple_net.c simple_net.h
	$(CC) $(CFLAFS) -c simple_net.c

driver.o : driver.c simple_net.c httpd.h
	$(CC) $(CFLAGS) -c driver.c simple_net.c

httpd.o : httpd.c httpd.h
	$(CC) $(FLAGS) -c  httpd.c

clean:
	rm *.o $(MAIN) core
