SHELL=/bin/bash
CC=gcc
CFlAGS=-std=c99 -Wall -pedantic
LDFLAGS=
LIBS=-lfl
LEX=flex
LFLAGS=--header-file=$*.h
BFLAGS= -v

mtp: parse.o lex.o main.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
lex.o: lex.c lex.h globals.h
	$(CC) $(CFLAGS) -c -o $@ $<
main.o: main.c lex.h parse.h globals.h
	$(CC) $(CLFAGS) -c -o $@ $<
parse.o: parse.c parse.h
	$(CC) $(CFLAGS) -c -o $@ $<
lex.c lex.h: lex.l
	$(LEX) $(LFLAGS) -o $*.c $<
parse.c parse.h: parse.y
	bison $(BFLAGS) --defines=$*.h -o $*.c $<
clean:
	$(RM) {main,lex,parse}.o lex.{c,h} parse.{c,h} mtp
