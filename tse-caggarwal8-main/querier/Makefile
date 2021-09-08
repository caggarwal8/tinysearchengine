# Makefile for querier module
# Written with the help of lectures
# CJ Aggarwal, May 2021

PROG = querier
OBJS = querier.o

LLIBS = ../common/common.a ../libcs50/libcs50-given.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../common -I../libcs50
CC = gcc

.PHONY: all test clean

# building the programs
$(PROG): $(OBJS) $(LLIBS)
	 $(CC) $(CFLAGS) $(OBJS) $(LLIBS) -o $(PROG)

# dependencies
querier.o: ../libcs50/counters.h ../libcs50/hashtable.h ../common/pagedir.h ../common/index.h ../common/word.h ../libcs50/file.h ../libcs50/mem.h ../libcs50/webpage.h

# testing using the bash script
test:
	bash -v testing.sh

# make clean
clean:
	rm -rf *.dSYM
	rm -f *~ *.o
	rm -f $(PROG)
	rm -f core
