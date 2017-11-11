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