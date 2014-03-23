#!/bin/bash
# File: Makefile
# Student Name: Daniel Martin


tokenizer: main.o print.o scanner.o
	gcc -g main.o print.o scanner.o -o tokenizer
	rm *.o

main.o: 
	gcc -g -c main.c scanner.h print.h common.h
print.o: 
	gcc -g -c print.c print.h common.h	
scanner.o: 
	gcc -g -c scanner.c scanner.h common.h
