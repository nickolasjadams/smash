# common var names
# OBJS, EXE, CC, CFLAGS

# automatic vars
# $^, $@, $<
# $< is the name of the first prerquiquiste dependency
# $^ is the name of all dependencies
# $@ is the name of the target

#standard make variable set c version and set Warnings All
CC = gcc
CFLAGS = -Wall -std=c99

OBJS = smash.o commands.o history.o
SRCS = smash.c commands.c history.c
EXE = smash

#Usage: make target

# all is a convention.  You should place this as the first rule
# Running `make` alone, will run the first rule

all: rules.d $(EXE)

#Rebuild rules.d if it’s out-of-date with any *.c or *.h file using gcc -MM
rules.d: $(wildcard *.c) $(wildcard *.h)
	gcc -MM $(wildcard *.c) > rules.d

debug:	CFLAGS += -g -O0
debug:	$(EXE)

#Incorporate the auto-generated dependencies from rules.d into this Makefile
-include rules.d

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

valgrind: debug
	valgrind --leak-check=yes --leak-check=full --show-leak-kinds=all --trace-children=no $(EXE)

$(EXE).o: $(EXE).c
	$(CC) $(CFLAGS) -c $(EXE).c

clean:
	rm -f *.o $(EXE) *.d
