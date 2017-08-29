#    PCCC: A portable library for context-cognizant completions.\
    Copyright (C) 2017  bcwarner \
\
    This program is free software: you can redistribute it and/or modify \
    it under the terms of the GNU General Public License as published by \
    the Free Software Foundation, either version 3 of the License, or \
    (at your option) any later version. \
\
    This program is distributed in the hope that it will be useful, \
    but WITHOUT ANY WARRANTY; without even the implied warranty of \
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the \
    GNU General Public License for more details. \
\
    You should have received a copy of the GNU General Public License \
    along with this program.  If not, see <http://www.gnu.org/licenses/>.


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