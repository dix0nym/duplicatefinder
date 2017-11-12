#include <stdio.h>
#include <stdlib.h>
#include "blake2/blake2.h"

int blake2b_stream( FILE *stream, void *resstream, size_t outbytes )
{
    int ret = -1;
    size_t sum, n;
    blake2b_state S[1];
    static const size_t buffer_length = 32768;
    uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

    if( !buffer ) return -1;

    blake2b_init( S, outbytes );

    while(1)
    {
        sum = 0;
        while(1)
        {
            n = fread( buffer + sum, 1, buffer_length - sum, stream );
            sum += n;

            if( buffer_length == sum )
                break;

            if( 0 == n )
            {
                if( ferror( stream ) )
                    goto cleanup_buffer;

                goto final_process;
            }

            if( feof( stream ) )
                goto final_process;
        }
        blake2b_update( S, buffer, buffer_length );
    }

final_process:;
    if( sum > 0 ) blake2b_update( S, buffer, sum );
    blake2b_final( S, resstream, outbytes );
    ret = 0;
cleanup_buffer:
    free( buffer );
    return ret;
}

unsigned char *create_hash(char *path)
{
    unsigned long maxbytes = BLAKE2B_OUTBYTES;
    //unsigned char hash[BLAKE2B_OUTBYTES] = {0};
    unsigned char *hash = malloc((maxbytes+1) * sizeof(unsigned char));
    if(!hash) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    FILE *f = fopen( path, "rb" );
  
    if( !f ) {
        printf("Could not open '%s'\n", path);
        exit(EXIT_FAILURE);
    }
  
    if( blake2b_stream( f, hash, maxbytes ) < 0 )
        printf("Failed to hash '%s'\n", path);
    fclose( f );
    return hash;
}