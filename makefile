CC=gcc
CFLAGS=-Wall -pedantic
.PHONY: all clean

all: duplicatefinder

duplicatefinder: main.o filelist.o blake2helper.c blake2b.o hashtable.o
		$(CC) main.o filelist.o blake2helper.c blake2b.o hashtable.o $(CFLAGS) -o duplicatefinder

main.o: main.c
		$(CC) -c main.c $(CFLAGS)

blake2b.o: blake2/blake2b.c
		$(CC) -c blake2/blake2b.c $(CFLAGS)

filelist: filelist.c
		$(CC) -c filelist.c $(CFLAGS)

hashtest: hashtest.c blake2b.o
		$(CC) hashtest.c hashtable.c blake2helper.c blake2b.o $(CFLAGS) -o hashtest

hashtable: hashtable.c
		$(CC) -c hashtable.c $(CFLAGS)

clean:
		rm -rf *.o duplicatefinder hashtest
