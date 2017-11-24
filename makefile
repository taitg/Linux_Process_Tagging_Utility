# CPSC 457 Spring 2016 Assignment 2
# Geordie Tait		10013837

# MAKEFILE for compiling _____
CC=gcc
CFLAGS=-g -Wall

all: ptag tagstat

ptag: ptag.c
	 $(CC) $(CFLAGS) -o $@ $<

tagstat: tagstat.c
	 $(CC) $(CFLAGS) -o $@ $<

clean:
	 rm -f ptag
	 rm -f tagstat
