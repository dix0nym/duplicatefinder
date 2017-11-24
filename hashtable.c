#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashtable.h"
#include "blake2/blake2.h"

static hashitem *head;

hashitem *create_hashitem(char *path, char *hash)
{
    hashitem *item = malloc(sizeof(hashitem));
    if (!item)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    char **files = malloc(sizeof(char *));
    if(!files)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    item->hash = hash;
    item->files = files;
    item->files[0] = path;
    item->idx = 1;
    item->next = NULL;
    return item;
}

hashitem *hashtable_lookup(char *hash)
{
    hashitem *current = head;
    while(current)
    {
        if(strcmp(current->hash, hash) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

int create_hash(char *path, char *hash_string)
{
    FILE *file = fopen(path, "rb");
    if (!file) return -1;
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    const int bufSize = 32768;
    char* buffer = malloc(bufSize);
    int bytesRead = 0;
    if(!buffer) return -1;
    while((bytesRead = fread(buffer, 1, bufSize, file)))
        SHA256_Update(&sha256, buffer, bytesRead);
    SHA256_Final(hash, &sha256);

    for(unsigned int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        sprintf(hash_string + (i * 2), "%02x", hash[i]);
    hash_string[64] = 0;
    fclose(file);
    free(buffer);
    return 0;
}

int create_blake2b_hash(char *path, char *hash_string)
{
    FILE *file = fopen(path, "rb");
    if (!file) return -1;
    unsigned char hash[BLAKE2B_OUTBYTES];
    blake2b_state S; //s[1];
    blake2b_init(&S, BLAKE2B_OUTBYTES);
    const int bufSize = 32768;
    char* buffer = malloc(bufSize);
    int bytesRead = 0;
    if(!buffer) return -1;
    while((bytesRead = fread(buffer, 1, bufSize, file)))
        blake2b_update(&S, buffer, bytesRead);
    blake2b_final(&S, hash, BLAKE2B_OUTBYTES);
    fclose(file);
    free(buffer);
    for(unsigned int i = 0; i < BLAKE2B_OUTBYTES; i++)
        sprintf(hash_string + (i * 2), "%02x", hash[i]);
    hash_string[64] = 0;
    return 0;
}

int hashtable_add_file(char *path)
{
    char *hash = malloc(65*sizeof(char));
    create_blake2b_hash(path, hash);
    
    if(!head)
    {
        head = create_hashitem(path, hash);
        return 0;
    }
    
    hashitem *item = hashtable_lookup(hash);
    if(item)
    {
        free(hash);
        char **tmp = realloc(item->files, (item->idx+1) * sizeof(char *));
        if(!tmp)
        {
            perror("realloc");
            return -1;
        }
        item->files = tmp;
        item->files[item->idx] = path;
        item->idx += 1;
    } else {
        hashitem *current = head;
        while(current->next)
            current = current->next;
        current->next = create_hashitem(path, hash);
    }
    return 0;
}

int hashtable_add_files(char **files, int size)
{
    for(int i = 0; i < size; i++)
        hashtable_add_file(files[i]);
    return 0;
}

int free_hashitem(hashitem *item)
{
    free(item->files);
    free(item->hash);
    free(item);
    return 0;
}

hashitem *remove_hashitem(hashitem *item)
{
    if(head == item)
    {
        head = item->next;
        free_hashitem(item);
        return head;
    }
    hashitem *prev = head;
    while(prev->next != NULL && prev->next != item)
        prev = prev->next;

    if(prev->next == NULL)
        return NULL;
    prev->next = prev->next->next;
    free_hashitem(item);
    return prev; 
}

hashitem *hashtable_get_duplicates(void)
{
    if(!head)
        return NULL;
    hashitem *current = head;
    while(current)
        current = (current->idx == 1) ? remove_hashitem(current) : current->next;
    return head;
}

int hashtable_destroy(void)
{
    if(!head)
        return -1;
    hashitem *current = head;
    while(current)
    {
        hashitem *next = current->next;
        free_hashitem(current);
        current = next;
    }
    head = NULL;
    return 0;
}

int hashtable_isEmpty(void)
{
    return !head;
}

int hashtable_dump(void)
{
    if(!head)
    {
        printf("hashtable is empty\n");
        return -1;
    }
    hashitem *current = head;
    while(current)
    {
        printf("hash: %s - size: %d - next: %p\n", current->hash, current->idx, (void *)current->next);
        int count = 0;
        for(int i = 0; i < current->idx; i++)
            printf("\t%d - %s\n", count++, current->files[i]);
        current = current->next;
    }
    return 0;
}