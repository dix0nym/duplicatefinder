#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <openssl/sha.h>

typedef struct hashitem {
    char *hash;
    int idx;
    char **files;
    struct hashitem *next;
} hashitem;


int SHA256_Init(SHA256_CTX *c);
int SHA256_Update(SHA256_CTX *c, const void *data, size_t len);
int SHA256_Final(unsigned char *md, SHA256_CTX *c);
unsigned char *SHA256(const unsigned char *d, size_t n,
                    unsigned char *md);

int hashtable_add_files(char **files, int size);

int hashtable_add_file(char *path);

hashitem *hashtable_get_duplicates(void);

int hashtable_destroy(void);

int hashtable_dump(void);

int hashtable_isEmpty(void);

#endif
