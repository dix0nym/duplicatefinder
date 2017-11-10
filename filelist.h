#ifndef FILELIST_H_
#define FILELIST_H_
#include "blake2/blake2.h"

typedef struct file {
    char* path;
    unsigned char hash[BLAKE2B_OUTBYTES];
} file;

typedef struct filelist {
    long *filesize;
    int idx;
    char **files;
    struct filelist *next;
} filelist;

int add(long *filesize, char *path);

int destroy(void);

filelist *lookup(long *filesize);

filelist *create_item(long *filesize, char *buf);

int remove_uniques(void);

filelist *delete_item(filelist *item);

int dump(void);

int dump_filelist(filelist *item);

#endif