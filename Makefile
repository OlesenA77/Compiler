#Alexander Olesen
#1671836
SHELL=/bin/bash
CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g
LDFLAGS=
LIBS=-lfl
LEX=flex
LFLAGS=--header-file=$*.h
BFLAGS= -v

mtp: parse.o lex.o mtp.o tree.o hash.o analyze.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
lex.o: lex.c lex.h globals.h
	$(CC) $(CFLAGS) -c -o $@ $<
mtp.o: mtp.c lex.h parse.h globals.h analyze.h
	$(CC) $(CLFAGS) -c -o $@ $<
analyze.o: analyze.c analyze.h globals.h tree.h
	$(CC) $(CFLAGS) -c -o $@ $<
hash.o: hash.c hash.h globals.h
	$(CC) $(CFLAGS) -c -o $@ $<
tree.o: tree.c tree.h globals.h 	
	$(CC) $(CFLAGS) -c -o $@ $<
parse.o: parse.c parse.h
	$(CC) $(CFLAGS) -c -o $@ $<
lex.c lex.h: lex.l globals.h
	$(LEX) $(LFLAGS) -o $*.c $<
parse.c parse.h: parse.y globals.h tree.h
	bison $(BFLAGS) --defines=$*.h -o $*.c $<
clean:
	$(RM) {mtp,lex,parse,tree,hash,analyze}.o lex.{c,h} parse.{c,h} mtp
