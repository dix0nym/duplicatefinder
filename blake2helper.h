#ifndef BLAKE2HELPER_H_
#define BLAKE2HELPER_H_

int blake2b_stream( FILE *stream, void *resstream, size_t outbytes );

unsigned char *create_hash(char *path);

#endif