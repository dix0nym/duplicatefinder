CC=gcc
CFLAGS=-Wall -pedantic
BINS=duplicatefinder

all: duplicatefinder

duplicatefinder: main.o filelist.c blake2helper.c blake2b.o hashtable.o
		$(CC) main.o filelist.c blake2helper.c blake2b.o hashtable.o $(CFLAGS) -o duplicatefinder

main.o: main.c
		$(CC) -c main.c $(CFLAGS)

blake2b.o: blake2/blake2b.c
		$(CC) -c blake2/blake2b.c $(CFLAGS)

filelist: filelist.c
		$(CC) -c filelist.c $(CFLAGS)

hashtable: hashtable.c
		$(CC) -c hashtable.c $(CFLAGS)

clean:
		rm -rf *.o $(BINS)
