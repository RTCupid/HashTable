#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <immintrin.h>

#include "../common/colors.h"
#include "HashTable.h"
#include "../List/List.h"
#include "DumpHashTable.h"

err_t HashTableCtor (hshtbl_t* hashtable)
{
    printf           (GRN "## Start HashCtor\n" RESET);

    hashtable->log_file = fopen ("build/log_file.txt", "wt");

    CreateHashTable  (hashtable);

    hashtable->Measurings_file = fopen ("Measurings.txt", "w");
    fprintf (hashtable->Measurings_file, "Time, s \n");

    return OK;
}

err_t RunHashTable (hshtbl_t* hashtable, my_key_t* keys, mode_hashtable_t mode)
{
    size_t offset      = 0;
    size_t index       = 0;
    size_t size_text   = 0;
    size_t word_len    = 0;
    char*  text_buffer = NULL;

    if      (mode == TEST)
    {
        size_text   = hashtable->size_test_text;
        text_buffer = hashtable->buffer_with_test_text;
    }
    else if (mode == LOAD)
    {
        size_text   = hashtable->size_text;
        text_buffer = hashtable->buffer_with_text;
    }

    while (1)
    {
        if (index >= size_text)
        {
            break;
        }

        *((char*)key + MAX_SIZE_WORD - 1) = '\0';
        offset++;

        SearchHashTable (hashtable, key, word_len, mode);

        index += offset;
    }

    return OK;
}

err_t SearchHashTable (hshtbl_t* hashtable, my_key_t key, size_t len_of_key, mode_hashtable_t mode)
{
    uint32_t hash     = murmurhash3 (key, len_of_key);

    HASHTABLE_DBG printf  (GRN                  "key \"%.6s\" => hash =  %u\n" RESET, (char*) key, hash);
    HASHTABLE_DBG fprintf (hashtable->log_file, "key \"%.6s\" => hash =  %u\n",       (char*) key, hash);

    hash = hash % (uint32_t) NBASKETS;

    HASHTABLE_DBG printf  (CYN                  "hash %% NBASKETS = %u\n" RESET, hash);
    HASHTABLE_DBG fprintf (hashtable->log_file, "hash %% NBASKETS = %u\n",       hash);

    int    status = 0;

    if (!FindInListValue (hashtable->HashTable[hash], key, &status))
    {
        printf  (MAG                                "value <%s> was not found in the List\n" RESET, (char*) key);
        HASHTABLE_DBG fprintf (hashtable->log_file, "value <%s> was not found in the List\n",       (char*) key);

        if (mode == LOAD)
        {
            if (status == 0)
            {
                HASHTABLE_DBG fprintf (hashtable->log_file, "status = 0,  add key to tail of list\n");
                ListAddTail  (&(hashtable->HashTable[hash]), key);
            }
            else
            {
                HASHTABLE_DBG fprintf (hashtable->log_file, "status != 0, add key to head of list\n");
                ListAddFairy (&(hashtable->HashTable[hash]), key);
            }
        }
        else
        {
            free (key);
            key = NULL;
        }

        return OK;
    }

    PRINTF_IF_FIND printf  (YEL                  "value <%s> was found in %u Basket\n" RESET, (char*) key, hash);
    HASHTABLE_DBG  fprintf (hashtable->log_file, "value <%s> was found in %u Basket\n",       (char*) key, hash);

    free (key);
    key = NULL;

    return OK;
}

err_t CreateHashTable (hshtbl_t* hashtable)
{
    hashtable->HashTable = (list_t*) calloc (NBASKETS, sizeof (*(hashtable->HashTable)));

    for (size_t index = 0; index < NBASKETS; index++)
    {
        ListCtor (&(hashtable->HashTable[index]));
    }

    return OK;
}

uint32_t murmurhash3 (const void* key, size_t len)
{
    const    uint8_t* data = (const uint8_t*)key;

    uint32_t hash          = SEED;

    for (size_t i = 0; i < len; i++)
    {
        hash ^= data[i];

        hash  = (hash << 13) | (hash >> 19);

        hash  = hash * 0x5bd1e995 + 0x165667b1;
    }

    return hash;
}

err_t CreateBufferText  (char * namefile, size_t* size_text, int* buffer_with_text_id, char** buffer_with_text)
{
    *buffer_with_text_id = open (namefile, O_RDONLY);

    if (*buffer_with_text_id == -1)
    {
        perror ("open");
        return ERR_OPEN;
    }

    struct stat fileInf = {};

    int err = stat (namefile, &fileInf);

    if (err != 0)
    {
        printf("Stat err %d\n", err);
        return ERR_STAT;
    }

    *size_text = (size_t) fileInf.st_size + 1;

    *buffer_with_text = (char*) mmap (NULL, *size_text, PROT_READ, MAP_PRIVATE,
                                                      *buffer_with_text_id, 0);

    if (*buffer_with_text == MAP_FAILED)
    {
        perror ("mmap");
        return ERR_MMAP;
    }

    return OK;
}

err_t HashTableDtor (hshtbl_t* hashtable)
{
    DeleteBufferText (hashtable->buffer_with_text, hashtable->buffer_with_text_id, hashtable->size_text);

    DeleteBufferText (hashtable->buffer_with_test_text, hashtable->buffer_with_test_text_id, hashtable->size_test_text);

    ClearHashTable   (hashtable);

    fclose (hashtable->log_file);

    return OK;
}

err_t DeleteBufferText (char* text_buffer, int text_buffer_id, size_t size_text)
{
    if (munmap (text_buffer, size_text) == -1)
    {
        perror ("munmap");
    }

    close  (text_buffer_id);

    return OK;
}

err_t ClearHashTable (hshtbl_t* hashtable)
{
    for (size_t index = 0; index < NBASKETS; index++)
    {
        HASHTABLE_DBG fprintf (stderr, YEL "start clear %lu list\n" RESET, index);

        ClearList (&hashtable->HashTable[index]);

        ListDtor  (&hashtable->HashTable[index]);

        HASHTABLE_DBG fprintf (stderr, GRN "end   clear %lu list\n" RESET, index);
    }

    free (hashtable->HashTable);
    hashtable->HashTable = NULL;

    return OK;
}
