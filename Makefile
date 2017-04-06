CC 		:= gcc
LD		:= ld
FLEX 	:= flex
BISON 	:= bison
CFLAGS 	:= -std=c99
#CFLAGS	+= -std=gnu11 -m32 -c #编译标准, 目标架构, 只编译
CFLAGS	+= -I include/

SRC_DIR	:= src
OBJ_DIR	:= obj

CFILES	:= $(shell find $(SRC_DIR) -name "*.c")
LFILE	:= $(shell find $(SRC_DIR) -name "*.l")
YFILE	:= $(shell find $(SRC_DIR) -name "*.y")

LFC		:= $(LFILE:$(SRC_DIR)%.l=$(OBJ_DIR)%.c)
YFC		:= $(YFILE:$(SRC_DIR)%.y=$(OBJ_DIR)%.c)
LFO		:= $(OBJ_DIR)/lexical.o
YFO		:= $(OBJ_DIR)/syntax.o

OBJS	:= $(CFILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

PARSER	:= obj/parser

all: parser

parser: $(CFILES) $(LFC) $(YFC)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $(PARSER) $(CFILES) $(LFC) $(YFC) -lfl -ly
#	$(LD) -o -e main $(PARSER) $(OBJS) $(LFO) $(YFO) -lfl -ly
#	@echo +LD $(KERN)
#	@perl ./kern/genkern.pl $(KERN)

#$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
#	@mkdir -p $(OBJ_DIR)/$(dir $<)
#	$(CC) $(CFLAGS) $< -o $@
#	@echo +CC $< -o $@

$(LFC): $(LFILE) $(YFC)
	$(FLEX) -o $(LFC) $(LFILE)
#	$(CC) $(CFLAGS) -o $(LFO) $(LFC)

$(YFC): $(YFILE)
	$(BISON) -d -o $(YFC) $(YFILE)
#	$(CC) $(CFLAGS) -o $(YFO) $(YFC)

.PHONY: clean

test:
	./$(PARSER) < testcase/test.cmm
clean:
	rm -rf obj/*

#all :
#	$(FLEX) -o src/lexical.c src/lexical.l
#	$(BISON) -d -o src/syntax.c src/syntax.y
#	$(CC) -I include/ src/lexical.c src/syntax.c src/main.c src/syntax_tree.c -o lexical -lfl -ly
