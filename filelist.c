#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filelist.h"

static filelist *head;

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
    char **files = malloc(1 * sizeof(char *));
    if(!files) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    new->files = files;
    new->files[0] = buf;
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
        char **tmp = realloc(np->files, (idx+ 1) * sizeof(char *));
        if (!tmp) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }
        np->files = tmp;
        np->files[idx] = path;
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
        
        for(int i = 0; i < current->idx;i++)
            printf("\t%d : %s\n", i, current->files[i]);
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
    for(int i = 0; i < item->idx; i++) {
        printf("\t%d : %s\n", i, item->files[i]);
    }
    printf("\n");
    return 0;
}