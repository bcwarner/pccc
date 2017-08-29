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
    along with this program.  If not, see <http://www.gnu.org/licenses/>. \

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