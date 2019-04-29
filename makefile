CC=gcc
CFLAGS= -Wall -ansi -Iincludes

main.c :
	$(CC) $(CFLAGS) src/functions.c src/main.c includes/defs.h -o bin/contactList

list:
		$(CC) $(CFLAGS) src/functions.c src/main.c includes/defs.h -o bin/contactList
		./bin/contactList

clean:
	rm bin/*
	clear
