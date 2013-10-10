CC=gcc
CFLAGS=-Wall -pedantic

all: settings.o list.o server.o main.o util.o client.o channel.o 
	$(CC) settings.o list.o server.o main.o util.o client.o \
	channel.o -o lirc 

util.o: util.c
	$(CC) $(CFLAGS) -c util.c

list.o: list.c
	$(CC) $(CFLAGS) -c list.c

main.o: main.c 
	$(CC) $(CFLAGS) -c main.c 

settings.o: settings.c
	$(CC) $(CFLAGS) -c settings.c 

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

channel.o: channel.c
	$(CC) $(CFLAGS) -c channel.c

client.o: client.c
	$(CC) $(CFLAGS) -c client.c 

indent:
	astyle --style=ansi --indent=spaces=2 *.c *.h

clean:
	rm -f lirc
	rm -f *.o
	rm -f *.c.orig
	rm -f *.h.orig
