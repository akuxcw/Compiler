CC 		:= gcc
FLEX 	:= flex
BISON 	:= bison
CFLAGS 	:= -std=c99

all :
	$(FLEX) -o lexical.c lexical.l
	$(BISON) -d -o syntax.c syntax.y
	$(CC) lexical.c syntax.c main.c -o lexical -lfl -ly
