# Makefile for crawler
# CJ Aggarwal, April 2021

CC = gcc
CFLAGS= -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common

PROG = crawler
OBJS = crawler.o
LLIBS = ../libcs50/libcs50-given.a ../common/common.a

.PHONY: all test clean

# building the program
$(PROG): $(OBJS) $(LLIBS)
	 $(CC) $(CFLAGS) $(OBJS) $(LLIBS) -o $(PROG)

# object files
crawler.o: ../libcs50/webpage.h ../libcs50/bag.h ../libcs50/hashtable.h ../common/pagedir.h

# testing using the bash script
test:
	bash -v testing.sh

# cleaning all files created by this Makefile
clean:
	rm -f $(PROG)
	rm -f *~ *.o
	rm -rf *.dSYM

