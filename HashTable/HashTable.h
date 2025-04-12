#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdint.h>

#include "../List/List.h"

enum err_t
{
    OK,
    ERR_CTOR,
    ERR_OPEN,
    ERR_FTRUNCATE,
    ERR_MMAP,
    ERR_STAT,
};

enum mode_print_hashtable_t
{
    POINTERS,
    KEYS,
};

const int      MAX_SIZE_WORD = 30;
const size_t   NBASKETS      = 8731;
const uint32_t SEED          = 0xEDABC526;

typedef struct hshtbl_t
{
    size_t    size_text;
    int       buffer_with_text_id;
    char*     buffer_with_text;
    list_t*   HashTable;
} hshtbl_t;

err_t       HashTableCtor     (char* namefile, hshtbl_t* hashtable);

err_t       HashTableDtor     (hshtbl_t* hashtable);

err_t       CreateBufferText  (char * namefile, hshtbl_t* hashtable);

err_t       CreateHashTable   (hshtbl_t* hashtable);

err_t       DumpHashTable     (hshtbl_t hashtable, mode_print_hashtable_t mode);

err_t       LoadHashTable     (hshtbl_t* hashtable);

uint32_t    murmurhash3_32    (const void* key, size_t len, uint32_t seed);

err_t       PrintHashTable    (hshtbl_t hashtable, mode_print_hashtable_t mode);

err_t       PrintList         (list_t list);

#endif
