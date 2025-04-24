#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdint.h>

#include "../List/List.h"

#define HASHTABLE_DBG if(0)
#define PRINTF_IF_FIND if(0)

enum err_t
{
    OK,
    ERR_CTOR,
    ERR_OPEN,
    ERR_FTRUNCATE,
    ERR_MMAP,
    ERR_STAT,
};

enum mode_hashtable_t
{
    TEST,
    LOAD,
};

enum mode_print_hashtable_t
{
    POINTERS,
    KEYS,
};

const int      MAX_SIZE_WORD = 32;

const size_t   ALIGN         = 32;

const size_t   NBASKETS      = 1409;

const uint32_t SEED          = 0xEDABC526;

const size_t   NTIMES        = 30;

typedef struct hshtbl_t
{
    size_t    size_text;
    int       buffer_with_text_id;
    char*     buffer_with_text;
    size_t    size_test_text;
    int       buffer_with_test_text_id;
    char*     buffer_with_test_text;
    list_t*   HashTable;
    FILE*     log_file;
} hshtbl_t;

typedef char* my_key_t;

err_t       HashTableCtor     (hshtbl_t* hashtable);

err_t       HashTableDtor     (hshtbl_t* hashtable);

err_t       DeleteBufferText  (char* text_buffer, int text_buffer_id, size_t size_text);

err_t       ClearHashTable    (hshtbl_t* hashtable);

err_t       CreateBufferText  (char * namefile, size_t* size_text, int* buffer_with_text_id, char** buffer_with_text);

err_t       CreateHashTable   (hshtbl_t* hashtable);

uint32_t    murmurhash3_32    (const void* key, size_t len, uint32_t seed);

err_t       SearchHashTable   (hshtbl_t* hashtable, my_key_t key, size_t len_of_key, mode_hashtable_t mode);

err_t       RunHashTable      (hshtbl_t* hashtable, mode_hashtable_t mode);

#endif
