# Makefile for the common module
# Written as per the specifications
# CJ Aggarwal, April 2021

L = ../libcs50
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L

OBJS = pagedir.o index.o word.o
LIB = common.a
LLIBS = $L/libcs50-given.a

# Building common.a
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS) $(LLIBS)

# object file - using header files
pagedir.o: $L/webpage.h pagedir.h
index.o: $L/hashtable.h $L/counters.h $L/webpage.h index.h $L/file.h
word.o: word.h


.PHONY: clean

# cleaning the products of this Makefile
clean:
	rm -f $(LIB) *~ *.o
