#include <stdlib.h>
#include <stdio.h>
#include "blake2helper.h"
#include "hashtable.h"

int main(void)
{
    char **files = malloc(2 * sizeof(char*));
    files[0] = "/home/nyx/Documents/publickey.RitypeAr@protonmail.com.txt";
    files[1] = "/home/nyx/Documents/publickey.RitypeAr@protonmail.com2.txt";
    // files[2] = "/home/nyx/Documents/Cephalopod.mp3";
    // files[3] = "/home/nyx/Documents/Cephalopod.mp3";
    // char *path = malloc(sizeof(char*));
    //char *path = "/home/nyx/Documents/publickey.RitypeAr@protonmail.com.txt";
    hashtable_add_files(files, 2);
    hashtable_destroy();
    // free(path);
    return 0;
}