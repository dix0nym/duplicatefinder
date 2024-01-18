#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <openssl/sha.h>

typedef struct hashitem {
    char *hash;
    int idx;
    char **files;
    struct hashitem *next;
} hashitem;

int create_blake2b_hash(char *path, char *hash_string);

int hashtable_add_files(char **files, int size);

int hashtable_add_file(char *path);

hashitem *hashtable_get_duplicates(void);

int hashtable_destroy(void);

int hashtable_dump(void);

int hashtable_isEmpty(void);

#endif
