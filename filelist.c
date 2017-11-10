#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filelist.h"
#include "blake2helper.h"

static filelist *head;

int compare_unsigned_char(unsigned char *a, unsigned char *b, int size)
{
    while(size-- > 0) {
        if ( *a != *b ) { return (*a < *b ) ? -1 : 1; }
        a++; b++;
    }
    return 0;
}

int check_duplicate(filelist *item)
{
    file **files = item->files;
    int i, j, size, count;
    size = item->idx;
    int freq[size];
    unsigned char *hashes[size];

    for(i = 0; i < size; i++) {
        freq[i] = -1;
    }

    for(i = 0; i < size; i++) {
        count = 1;
        for(j = i+1; j < size; j++) {
            if(compare_unsigned_char(files[i]->hash, files[j]->hash, 64) == 0) {
                hashes[i] = files[i]->hash;
                count++;
                freq[j] = 0;
            }
        }
        if(freq[i] != 0) {
            freq[i] = count;
        }
    }
    for(i = 0; i < size; i++) {
        printf("freq: %d\n", freq[i]);
        if(freq[i] != 0) {
            printf("%s occurs %d times\n", hashes[i], freq[i]);
        }
    }
    return 0;
}

int create_hashtable(void)
{
    if (!head)
        return -1;
    remove_uniques();
    filelist *current = head;
    
    while(current){
        file **files = current->files;
        for(int i = 0;i < current->idx; i++){
            files[i]->hash = create_hash(files[i]->path);
        }
        check_duplicate(current);
        current = current->next;
    }
    return 0;
}

filelist *lookup(long *size)
{
    if(!head)
        return NULL;
    filelist *current = head;
    while(current){
        if (*current->filesize == *size)
            return current;
        current = current->next;
    }
    return NULL;
}

filelist *get_filelist(void)
{
    return head;
}

int remove_uniques(void)
{
    filelist *current = head;
    while(current) {
        current = (current->idx == 1) ? delete_item(current) : current->next;            
    }
    return 0;
}

filelist *create_item(long *filesize, char *buf)
{
    filelist *new = malloc(sizeof(filelist));
    if(!new) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    new->filesize = filesize;
    new->next = NULL;
    file **files = malloc(sizeof(file *));
    if(!files) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    file *item = malloc(sizeof(file));
    if(!item) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    item->path = buf;
    item->hash = NULL;
    //char **files = malloc(1 * sizeof(char *));
    if(!files) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    new->files = files;
    new->files[0] = item;
    new->idx = 1;
    return new;
}


int add(long *filesize, char *path)
{
    if (!head){
        printf("no head, create one\n");
        head = create_item(filesize, path);
        return 0;
    }
    filelist *np = lookup(filesize);
    if (!np){
        filelist *current = head;
        while(current->next) {
            current = current->next;
        }
        current->next = create_item(filesize, path);
    } else {
        int idx = np->idx;
        file **tmp = realloc(np->files, (idx+ 1) * sizeof(file *));
        if (!tmp) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }
        file *item = malloc(sizeof(file *));
        if (!item) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }
        item->path = path;
        item->hash = NULL;
        np->files = tmp;
        np->files[idx] = item;
        np->idx = idx+1;
    }
    return 0;
}

filelist *delete_item(filelist *item)
{
    if(head == item) {
        head = item->next;
        for(int i = 0; i < item->idx;i++){
            free(item->files[i]);
        }
        free(item->files);
        free(item->filesize);
        free(item);
        return head;
    }
    filelist *prev = head;
    while(prev->next != NULL && prev->next != item)
        prev = prev->next;

    if(prev->next == NULL) {
        return NULL;
    }
    prev->next = prev->next->next;
    for(int i = 0; i < item->idx;i++)
        free(item->files[i]);
    free(item->files);
    free(item->filesize);
    free(item);
    return prev; 
}

int free_item(filelist *item)
{
    for(int i = 0; i < item->idx;i++)
        free(item->files[i]);
    free(item->files);
    free(item->filesize);
    free(item);
    return 0;
}

int destroy(void)
{
    filelist *current = head;
    while(current){
        for(int i = 0; i < current->idx;i++)
            free(current->files[i]);
        
        free(current->files);
        free(current->filesize);

        filelist *tofree = current;
        current = current->next;
        free(tofree);
    }
    head = NULL;
    return 0;
}

int dump(void)
{
    if(!head){
        printf("List is empty\n");
        return -1;
    }
    filelist *current = head;
    int c = 0;
    while(current){
        printf("idx: %d - filesize: %ld - files: %d - ", c, *current->filesize, current->idx);
        if(current->next)
            printf("has next\n");
        else
            printf("has no next\n");
        
        for(int i = 0; i < current->idx;i++) {
            printf("\t%d : %s - hash: ", i, current->files[i]->path);
            if(current->files[i]->hash) {
                for(size_t j = 0; j < 64; ++j )
                    printf( "%02x", current->files[i]->hash[j] );
            } else {
                printf("null");
            }
            printf("\n");
        }
        c += 1;
        current = current->next;
    }
    return 0;
}

int dump_filelist(filelist *item)
{
    if(!item)
        return -1;
    printf("filesize: %ld - files: %d - ", *item->filesize, item->idx);
    if(item->next)
        printf("has next\n");
    else
        printf("has no next\n");
    for(int i = 0; i < item->idx;i++) {
            printf("\t%d : %s - hash: ", i, item->files[i]->path);
            if(item->files[i]->hash) {
                for(size_t j = 0; j < 64; ++j )
                    printf( "%02x", item->files[i]->hash[j] );
            } else {
                printf("null");
            }
            printf("\n");
        }
    printf("\n");
    return 0;
}