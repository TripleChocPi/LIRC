CC=gcc
CFLAGS=-ansi -Wextra -pedantic

all: main.o settings.o util.o 
	$(CC) main.o settings.o util.o -o lirc 

main.o: main.c 
	$(CC) $(CFLAGS) -c main.c 

settings.o: settings.c
	$(CC) $(CFLAGS) -c settings.c 

util.o: util.c
	$(CC) $(CFLAGS) -c util.c

indent:
	astyle --style=ansi --indent=spaces=2 *.c *.h

clean:
	rm -f *.o
	rm -f *.c.orig
	rm -f *.h.orig
