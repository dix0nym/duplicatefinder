#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filelist.h"
#include "hashtable.h"

static filelist *head;

filelist *create_item(long *filesize, char *buf)
{
    filelist *new = malloc(sizeof(filelist));
    if(!new) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    new->filesize = filesize;
    new->next = NULL;
    char **files = malloc(sizeof(char *));
    
    //file **files = malloc(sizeof(file *));
    if(!files) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    // file *item = malloc(sizeof(file));
    // if(!item) {
    //     perror("malloc");
    //     exit(EXIT_FAILURE);
    // }
    // item->path = buf;
    // item->hash = NULL;
    //char **files = malloc(1 * sizeof(char *));
    // if(!files) {
    //     perror("malloc");
    //     exit(EXIT_FAILURE);
    // }
    new->files = files;
    new->files[0] = buf;
    new->idx = 1;
    return new;
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


int filelist_add(long *filesize, char *path)
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
        char **tmp = realloc(np->files, (idx+ 1) * sizeof(char *));
        if (!tmp) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }
        // file *item = malloc(sizeof(file *));
        // if (!item) {
        //     perror("realloc");
        //     exit(EXIT_FAILURE);
        // }
        // item->path = path;
        // item->hash = NULL;
        np->files = tmp;
        np->files[idx] = path;
        np->idx = idx+1;
    }
    return 0;
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

filelist *remove_item(filelist *item)
{
    if(head == item) {
        head = item->next;
        free_item(item);
        return head;
    }
    filelist *prev = head;
    while(prev->next != NULL && prev->next != item)
        prev = prev->next;

    if(prev->next == NULL) {
        return NULL;
    }
    prev->next = prev->next->next;
    free_item(item);
    return prev; 
}

filelist *get_filelist(void)
{
    return head;
}

int remove_uniques(void)
{
    filelist *current = head;
    while(current)
        current = (current->idx == 1) ? remove_item(current) : current->next;            
    return 0;
}

int filelist_destroy(void)
{
    filelist *current = head;
    while(current){
        filelist *item = current;
        current = current->next;
        free_item(item);
    }
    head = NULL;
    return 0;
}

int check_duplicate(void)
{
    if(!head)
        return -1;
    remove_uniques();
    filelist *current = head;
    while(current)
    {
        hashtable_add_files(current->files, current->idx);
        hashtable_get_duplicates();
        hashtable_destroy();
        current = current->next;
    }
    return 0;
}

int dump_item(filelist *item)
{
    if(!item)
        return -1;
    printf("filesize: %ld - files: %d - next: %p\n", *item->filesize, item->idx, (void*)item->next);
    for(int i = 0; i < item->idx;i++) {
        printf("\t%d : %s\n", i, item->files[i]);
    }
    return 0;
}

int filelist_dump(void)
{
    if(!head){
        printf("List is empty\n");
        return -1;
    }
    filelist *current = head;
    int c = 0;
    while(current){
        printf("idx: %d - ", c);
        dump_item(current);
        c += 1;
        current = current->next;
    }
    return 0;
}