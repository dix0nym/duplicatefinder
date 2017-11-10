#ifndef FILELIST_H_
#define FILELIST_H_

typedef struct file {
    char* path;
    unsigned char *hash;
} file;

typedef struct filelist {
    long *filesize;
    int idx;
    struct file **files;
    struct filelist *next;
} filelist;

int blake2b_stream( FILE *stream, void *resstream, size_t outbytes );

unsigned char *create_hash(char *path);

int add(long *filesize, char *path);

int destroy(void);

filelist *lookup(long *filesize);

filelist *create_item(long *filesize, char *buf);

int create_hashtable(void);

int remove_uniques(void);

filelist *delete_item(filelist *item);

int dump(void);

int dump_filelist(filelist *item);

#endif