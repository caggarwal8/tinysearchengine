# Makefile for indexer module
# Written with the help of lectures
# CJ Aggarwal, April 2021

PROG = indexer
OBJS = indexer.o

LLIBS = ../common/common.a ../libcs50/libcs50-given.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../common -I../libcs50
CC = gcc

.PHONY: all test clean

# make both indexer and indextest
all: indexer indextest

# building the programs
$(PROG): $(OBJS) $(LLIBS)
	 $(CC) $(CFLAGS) $(OBJS) $(LLIBS) -o $(PROG)

indextest: indextest.o $(LLIBS)
	$(CC) $(CFLAGS) indextest.o $(LLIBS) -o indextest

# dependencies
indexer.o: ../common/pagedir.h ../libcs50/webpage.h ../libcs50/hashtable.h ../common/index.h ../common/word.h ../libcs50/counters.h ../libcs50/file.h ../libcs50/mem.h

indextest.o: ../common/index.h ../common/pagedir.h

# testing using the bash script
test:
	bash -v testing.sh

# make clean
clean:
	rm -rf *.dSYM
	rm -f *~ *.o
	rm -f $(PROG)
	rm -f indextest
	rm -f core
