CC=gcc
CFLAGS=-Wall -pedantic
BINS=test duplicatefinder
#-std=c89 
FILES=test.c blake2/blake2b.c blake2/blake2s.c

all: duplicatefinder test

duplicatefinder: main.o filelist.o
		$(CC) -g -o duplicatefinder main.o filelist.o  $(CFLAGS)

test:	test.c 
		$(CC) $(FILES) $(CFLAGS) $(LIBS) -o test

main: main.c
		$(CC) -c main.c $(CFLAGS)

filelist: filelist.c
		$(CC) -c filelist.c $(CFLAGS)

clean:
		rm -rf *.o $(BINS)
