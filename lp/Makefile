LEX=$(wildcard *.lex)
BISON=$(wildcard *y)
LP=$(LEX:%.lex=%.yy.c) $(BISON:%.y=%.tab.c) $(BISON:%.y=%.tab.h)
CFLAGS=-Wall -fpic
DFLAGS=-g
CC=gcc

%.tab.c: %.y
	bison -d $<

%.yy.c: %.lex
	lex --outfile=$@ $< 

all: $(LP)
	-$(CC) $(CFLAGS) -c $(LP)