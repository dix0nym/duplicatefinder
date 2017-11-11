#ifndef HASHTABLE_H
#define HASHTABLE_H

typedef struct hashitem {
    unsigned char *hash;
    char **files;
    int idx;
    struct hashitem *next;
} hashitem;

int hashtable_add_files(char **files, int size);

int hashtable_add_file(char *path);

hashitem *hashtable_get_duplicates(void);

int hashtable_destroy(void);

int hashtable_dump(void);

#endif
