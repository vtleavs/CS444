LIB=-lreadline -lcurses
CFLAGS=-DUNIX -ansi -pedantic-errors
DEBUG=-g
#DEBUG=

all: shell


shell:	shell.c parse.c parse.h commands.c commands.h
	gcc $(CFLAGS) $(DEBUG) shell.c parse.c commands.c -o shell $(LIB)
clean:
	rm -f shell *~

run:  shell
	./shell
