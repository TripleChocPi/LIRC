CC=gcc
CFLAGS=-ansi -Wall -pedantic

all: settings.o server.o main.o util.o client.o 
	$(CC) settings.o server.o main.o util.o client.o -o lirc 

util.o: util.c
	$(CC) $(CFLAGS) -c util.c

main.o: main.c 
	$(CC) $(CFLAGS) -c main.c 

settings.o: settings.c
	$(CC) $(CFLAGS) -c settings.c 

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

client.o: client.c
	$(CC) $(CFLAGS) -c client.c 

indent:
	astyle --style=ansi --indent=spaces=2 *.c *.h

clean:
	rm -f lirc
	rm -f *.o
	rm -f *.c.orig
	rm -f *.h.orig
