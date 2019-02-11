LIB=-lreadline -lcurses
CFLAGS=-DUNIX -ansi -pedantic-errors
DEBUG=-g
#DEBUG=

all: benjos


benjos:	shell.c parse.c parse.h commands.c commands.h
	gcc $(CFLAGS) $(DEBUG) shell.c parse.c commands.c -o benjos $(LIB)
clean:
	rm -f benjos *~

run:  benjos
	./benjos
