#include <stdlib.h>
#include <stdio.h>
#include "hashtable.h"
#include "blake2helper.h"

static hashitem *head;

hashitem *create_hashitem(char *path)
{
    hashitem *item = malloc(sizeof(hashitem *));
    if (!item)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    unsigned char *hash = create_hash(path);
    item->hash = hash;
    char **files = (char **)malloc(sizeof(char*));
    if(!files)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    files[0] = path;
    item->files = files;
    item->idx = 1;
    item->next = NULL;
    return item;
}

int compare_unsigned_char(unsigned char *a, unsigned char *b, int size)
{
    while(size-- > 0) {
        if ( *a != *b ) { return (*a < *b ) ? -1 : 1; }
        a++; b++;
    }
    return 0;
}

hashitem *hashtable_lookup(unsigned char *hash)
{
    hashitem *current = head;
    while(current)
    {
        if(compare_unsigned_char(current->hash, hash, 64) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int hashtable_add_file(char *path)
{
    if(!head)
    {
        head = create_hashitem(path);
        return 0;
    }
    unsigned char *hash = create_hash(path);
    hashitem *item = hashtable_lookup(hash);
    if(item)
    {
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
        while(current->next) {
            current = current->next;
        }
        current->next = create_hashitem(path);
    }
    return 0;

}

int hashtable_add_files(char **files, int size)
{
    for(int i = 0; i < size; i++)
    {
        hashtable_add_file(files[i]);
    }
    return 0;
}

int free_hashitem(hashitem *item)
{
    for(int i = 0; i < item->idx;i++){
        free(item->files[i]);
    }
    free(item->files);
    free(item->hash);
    free(item);
    return 0;
}

hashitem *remove_hashitem(hashitem *item)
{
    if(head == item) {
        head = item->next;
        free_hashitem(item);
        return head;
    }
    hashitem *prev = head;
    while(prev->next != NULL && prev->next != item)
        prev = prev->next;

    if(prev->next == NULL) {
        return NULL;
    }
    prev->next = prev->next->next;
    free_hashitem(item);
    return prev; 
}

int hashtable_remove_uniques(void)
{
    hashitem *current = head;
    while(current)
        current = (current->idx == 1) ? remove_hashitem(current) : current->next;
    return 0;
}

hashitem *hashtable_get_duplicates(void)
{
    if(!head)
        return NULL;
    hashtable_remove_uniques();
    hashtable_dump();
    return head;
}

int hashtable_destroy(void)
{
    if(!head)
        return -1;
    hashitem *current = head;
    while(current)
    {
        hashitem *item = current;
        current = current->next;
        free_hashitem(item);
    }
    return 0;
}

int hashtable_dump(void)
{
    if(!head)
    {
        printf("hashitem is empty\n");
        return -1;
    }
    hashitem *current = head;
    while(current)
    {
        for(size_t j = 0; j < 64; ++j )
            printf( "%02x", current->hash[j] );
        printf(" - size: %d - next: %p\n", current->idx, (void *)current->next);
        int count = 0;
        for(int i = 0; i < current->idx; i++) {
            printf("\t%d - %s\n", count, current->files[i]);
            count++;
        }
        current = current->next;
    }
    return 0;
}