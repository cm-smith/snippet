# File: Makefile

CC = gcc
CFLAGS = -Wall -pedantic -std=c11
UTILDIR = ./src/
#UTILFLAG = -lcurl
UTILLIB = $(UTILDIR)utils.h
UTILC = $(UTILDIR)snippet.c $(UTILDIR)utils.c $(UTILDIR)hashtable.c
UTILH = $(UTILDIR)hashtable.h

# Project details
EXEC = snippet
OBJS = snippet.o hashtable.o utils.o
SRCS = $(UTILLIB) $(UTILC) $(UTILH)

$(EXEC): $(OBJS) $(UTILLIB)
	@$(CC) $(CFLAGS) -o $(EXEC) $(OBJS) $(UTILFLAG)
	@rm -r *.o
	@rm -f ./src/*.gch
	@rm -rf *.dSYM

$(OBJS): $(SRCS)
	@$(CC) $(CFLAGS) -c $(SRCS)

debug: $(SRCS)
	@$(CC) $(CFLAGS) -g -ggdb3 -c $(SRCS)
	@$(CC) $(CFLAGS) -g -ggdb3 -o $(EXEC) $(OBJS) $(UTILFLAG)

test:
	@./test.sh

clean:
	@rm -f *.o
	@rm -f $(EXEC)
	@rm -f ./src/*.gch
	@rm -rf *.dSYM
