#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdint.h>
#include <immintrin.h>

struct array_my_key_t;

#define HASHTABLE_DBG if(0)
#define PRINTF_IF_FIND if(0)

struct list_t;

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

const int      MAX_SIZE_WORD     = 16;

const size_t   ALIGN             = 32;

const size_t   NBASKETS          = 1409;

const uint32_t SEED              = 0xEDABC526;

const size_t   NTIMES            = 150;

const uint32_t COMPARE_M128_MASK = 0x0000FFFF;

typedef struct hshtbl_t
{
    list_t*   HashTable;

    FILE*     log_file;

    FILE*     Measurings_file;

} hshtbl_t;

typedef __m128i my_key_t;

err_t       HashTableCtor       (hshtbl_t* hashtable);

err_t       HashTableDtor       (hshtbl_t* hashtable);

err_t       ClearHashTable      (hshtbl_t* hashtable);

err_t       CreateBufferText    (char * namefile, size_t* size_text, int* buffer_with_text_id, char** buffer_with_text);

err_t       CreateHashTable     (hshtbl_t* hashtable);

uint32_t    murmurhash3         (const void* key);

err_t       SearchHashTable     (hshtbl_t* hashtable, my_key_t* key, mode_hashtable_t mode);

err_t       LoadHashTable       (hshtbl_t* hashtable, array_my_key_t* array_pointers);

err_t       RunHashTable        (hshtbl_t* hashtable, array_my_key_t* array_pointers);

#endif
