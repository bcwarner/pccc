CFLAGS=-Wall -pthread -fpic
SONAME=libpccc.so
SONAMEVER=$(SONAME).1
LIBFLAGS=-pthread -shared -fpic -Wl,-soname,$(SONAME)
DFLAGS=-g -D DEBUG
SRC=$(wildcard *.c)
OBJ=$(SRC:%.c=%.o) lp/c.tab.o lp/c.yy.o
PLUGINSRC=$(wildcard plugins/*)
CC=gcc

#ensures command cannot fail

all: lib release plugins test 

lp/lp.o:
	cd lp; make
	cd ..

%.o: %.c %.h
	-$(CC) $(CFLAGS) $(DFLAGS) -c $< 

lib: lp/lp.o $(OBJ) 
	$(CC) $(LIBFLAGS) $(DFLAGS) -o $(SONAME) $(OBJ)

plugins: $(PLUGINSRC)
	cd plugins; make

test: lib
	cd tests; make

release: lib plugins
	cp $(SONAME) /usr/lib/$(SONAMEVER)
	cd /usr/lib; ldconfig -v -l $(SONAMEVER); chmod 777 $(SONAMEVER)

clean:
	rm -f /usr/lib/$(SONAMEVER) /usr/lib/$(SONAME)
	find . -name '*.o' -delete
	find . -name '*.so.*' -delete
	find . -name '*.yy.c' -delete
	find . -name '*.tab.c' -delete
	find . -name '*.tab.h' -delete