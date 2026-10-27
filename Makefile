CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: addressbook

addressbook: main.o contact.o file.o
	$(CC) $(CFLAGS) -o addressbook main.o contact.o file.o

main.o: main.c contact.h file.h
	$(CC) $(CFLAGS) -c main.c

contact.o: contact.c contact.h file.h
	$(CC) $(CFLAGS) -c contact.c

file.o: file.c file.h contact.h
	$(CC) $(CFLAGS) -c file.c

clean:
	rm -f *.o addressbook
