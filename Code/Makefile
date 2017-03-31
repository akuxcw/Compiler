CC 		:= gcc
FLEX 	:= flex
BISON 	:= bison
CFLAGS 	:= -std=c99

all :
	$(FLEX) -o src/lexical.c src/lexical.l
	$(BISON) -d -o src/syntax.c src/syntax.y
	$(CC) -I include/ src/lexical.c src/syntax.c src/main.c src/syntax_tree.c -o lexical -lfl -ly
