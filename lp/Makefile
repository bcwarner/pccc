LEX=$(wildcard *.lex)
BISON=$(wildcard *y)
LP=$(LEX:%.lex=%.yy.c) $(BISON:%.y=%.tab.c) $(BISON:%.y=%.tab.h)
CFLAGS=-Wall -fpic
DFLAGS=-g
DLFLAGS=-v
DBFLAGS=--report=solved
CC=gcc

%.tab.c: %.y
	bison $(DBFLAGS) -d $<

%.yy.c: %.lex
	lex $(DLFLAGS) --outfile=$@ $< 

#ctest: c.tab.c c.yy.c # This rule breaks the Makefile chain
#	$(CC) $(CFLAGS) -v c.tab.h c.tab.c c.yy.c -o ctest

all: $(LP)
	-$(CC) $(CFLAGS) -c $(LP)