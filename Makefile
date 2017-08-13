CFLAGS=-Wall -pthread -fpic
LIBFLAGS=-pthread -shared -fpic
DFLAGS=-g -D DEBUG
SRC=$(wildcard *.c)
OBJ=$(SRC:%.c=%.o) lp/c.tab.o lp/c.yy.o
CC=gcc

#ensures command cannot fail

all: lib test

lp/lp.o:
	cd lp; make
	cd ..

%.o: %.c %.h
	-$(CC) $(CFLAGS) $(DFLAGS) -c $< 

lib: lp/lp.o $(OBJ) 
	$(CC) $(LIBFLAGS) $(DFLAGS) -o pccc.so $(OBJ)

plugins:

test: lib

release:

clean:
	find . -name '*.o' -delete
	find . -name '*.yy.c' -delete
	find . -name '*.tab.c' -delete
	find . -name '*.tab.h' -delete