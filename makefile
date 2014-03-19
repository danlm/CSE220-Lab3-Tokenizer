#!/bin/bash
# File: Makefile
# Daniel Martin


lister: main.o print.o scanner.o
	gcc main.o print.o scanner.o -o tokenizer
	rm *.o

main.o: main.c scanner.h print.h common.h
print.o: print.c print.h common.h	
scanner.o: scanner.c scanner.h common.h
