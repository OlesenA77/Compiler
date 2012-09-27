SHELL=/bin/bash
CC=gcc
CFlAGS=-std=c99 -Wall -pedantic
LDFLAGS=
LIBS=-lfl
LEX=flex
LFLAGS=--header-file=$*.h

main: mtp.o main.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

mtp.o: mtp.c mtp.h globals.h
	$(CC) $(CFLAGS) -c -o $@ $<
main.o: main.c mtp.h globals.h
	$(CC) $(CLFAGS) -c -o $@ $<
mtp.h mtp.c: mtp.l
	$(LEX) $(LFLAGS) -o $*.c $<
clean:
	$(RM) {mtp,main}.o mtp.{c,h} main
