#ifndef FILELIST_H_
#define FILELIST_H_

typedef struct file {
    char* path;
    unsigned char *hash;
} file;

typedef struct filelist {
    long *filesize;
    int idx;
    char **files;
    struct filelist *next;
} filelist;

int filelist_add(long *filesize, char *path);

int filelist_destroy(void);

int filelist_remove_uniques(void);

int check_duplicate(void);

int filelist_dump(void);

filelist *get_filelist(void);

#endif