TARGETS=lib sublime
CFLAGS=-Wall
DFLAGS=-g
SRC=$(wildcard *.c)
OBJ=$(patsubst %.c,%.o,$(SRCS))
CC=gcc

%.o: %.c $(wildcard %.h)
	$(CC) $(CFLAGS) -o $@ $< 

lib: $(OBJ)
	gcc -o $

clean:
	rm -f *.o
	rm -f *.yy.c
	rm -f *.tab.c
	rm -f *.tab.h