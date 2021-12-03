CC = gcc
CFLAGS = -O2 -g

all: lrparser.tab.c lex.yy.c ast.c main.c semantic.c
	$(CC) $(CFLAGS) lrparser.tab.c lex.yy.c ast.c main.c semantic.c
 
lrparser.tab.c : lrparser.y
	bison -d lrparser.y

lex.yy.c : lrlex.l
	flex lrlex.l
	

.PHONY: clean
clean:
	$(RM) *.o *.out lrparser.tab.c lrparser.tab.h lex.yy.c