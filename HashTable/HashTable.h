#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdint.h>

enum err_t
{
    OK,
    ERR_CTOR,
    ERR_OPEN,
    ERR_FTRUNCATE,
    ERR_MMAP,
    ERR_STAT,
};

const int      MAX_SIZE_WORD = 30;
const size_t   NBASKETS      = 8731;
const uint32_t SEED          = 0xEDAEDAEDA;

typedef struct hshtbl_t
{
    size_t size_text;
    int    buffer_with_text_id;
    char*  buffer_with_text;
} hshtbl_t;

err_t       HashTableCtor     (char* namefile, hshtbl_t* hashtable);

err_t       HashTableDtor     (hshtbl_t* hashtable);

err_t       CreateBufferText  (char * namefile, hshtbl_t* hashtable);

err_t       LoadHashTable     (hshtbl_t* hashtable);

uint32_t    murmurhash3_32    (const void* key, size_t len, uint32_t seed);

#endif
