CC=gcc
CFLAGS=-ansi -Wextra -pedantic

all: settings.o server.o main.o util.o 
	$(CC) settings.o server.o main.o util.o -o lirc 

util.o: util.c
	$(CC) $(CFLAGS) -c util.c

main.o: main.c 
	$(CC) $(CFLAGS) -c main.c 

settings.o: settings.c
	$(CC) $(CFLAGS) -c settings.c 

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

indent:
	astyle --style=ansi --indent=spaces=2 *.c *.h

clean:
	rm -f lirc
	rm -f *.o
	rm -f *.c.orig
	rm -f *.h.orig
