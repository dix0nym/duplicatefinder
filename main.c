#define _XOPEN_SOURCE 700

#define _LARGEFILE64_SOURCE
#define _FILE_OFFSET_BITS 64 

#include <stdlib.h>
//#include <unistd.h>
#include <ftw.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "filelist.h"

#ifndef USE_FDS
#define USE_FDS 15
#endif

int addtodict(const char *filepath, const struct stat *info,
    const int typeflag, struct FTW *pathinfo)
{
    if(typeflag == FTW_F) {
        long *size = malloc(sizeof(long));
        if(!size){
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        struct stat st;
        stat(filepath, &st);
        *size = st.st_size;
        //printf("%d - %s\n", *size, filepath);

        char *path = malloc(sizeof(char)*(strlen(filepath) + 1));
        if(!path){
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        strncpy(path, filepath, strlen(filepath)+1);
        add(size, path);
    }
    return 0;
}

int walk(const char *const dirpath){
    if(nftw(dirpath, addtodict, USE_FDS, FTW_PHYS) == -1){
        perror("nftw");
        exit(EXIT_FAILURE);
    }
    return 0;
}
int main(int argc, char **argv) {
    char *dirpath = "/home/nyx/Documents/";
    walk(dirpath);
    create_hashtable();
    dump();
    destroy();
    return 0;
}