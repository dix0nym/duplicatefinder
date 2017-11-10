#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "blake2/blake2.h"

int blake2s_stream( FILE *stream, void *resstream, size_t outbytes )
{
  int ret = -1;
  size_t sum, n;
  blake2s_state S[1];
  static const size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  if( !buffer ) return -1;

  blake2s_init( S, outbytes );

  while( 1 )
  {
    sum = 0;

    while( 1 )
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

    blake2s_update( S, buffer, buffer_length );
  }

final_process:;

  if( sum > 0 ) blake2s_update( S, buffer, sum );

  blake2s_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  free( buffer );
  return ret;
}

int blake2b_stream( FILE *stream, void *resstream, size_t outbytes )
{
  int ret = -1;
  size_t sum, n;
  blake2b_state S[1];
  static const size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  if( !buffer ) return -1;

  blake2b_init( S, outbytes );

  while( 1 )
  {
    sum = 0;

    while( 1 )
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



typedef int ( *blake2fn )( FILE *, void *, size_t );

int main( int argc, char **argv )
{
  blake2fn blake2_stream = blake2b_stream;
  unsigned long maxbytes = BLAKE2B_OUTBYTES;
  const char *algorithm = "BLAKE2s"; //BLAKE2s
  char *path = "main.c";
  unsigned char hash[BLAKE2B_OUTBYTES] = {0};

  if (strcmp(algorithm, "BLAKE2b") == 0) {
    blake2_stream = blake2b_stream;
    maxbytes = BLAKE2B_OUTBYTES;
  } else {
    blake2_stream = blake2s_stream;
    maxbytes = BLAKE2S_OUTBYTES;
  }
  printf("algo: %s\n", algorithm);
  FILE *f = NULL;
  f = fopen( path, "rb" );
  
  if( !f ) {
    printf("Could not open %s\n", path);
    exit(EXIT_FAILURE);
  }
  
  if( blake2_stream( f, hash, maxbytes ) < 0 ) {
    printf("Failed to hash `%s'\n", path);
  } else {
    size_t j;
    for( j = 0; j < maxbytes; ++j )
      printf( "%02x", hash[j] );
    printf( "  %s\n", path);
  }
  fclose( f );
  return 0;
}