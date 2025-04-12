#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#include "../common/colors.h"
#include "HashTable.h"
#include "../List/List.h"

err_t HashTableCtor (char* namefile, hshtbl_t* hashtable)
{
    printf           (GRN "Start HashCtor\n" RESET);

    CreateBufferText (namefile, hashtable);

    CreateHashTable  (hashtable);

    DumpHashTable    (*hashtable, POINTERS);

    LoadHashTable    (hashtable);

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

err_t DumpHashTable (hshtbl_t hashtable, mode_print_hashtable_t mode)
{
    fprintf (stderr, CYN "-Dump-------------------------------------------------------------------------\n" RESET);
    fprintf (stderr, CYN "hashtable.buffer_with_text    = %p \n" RESET, hashtable.buffer_with_text);
    fprintf (stderr, CYN "hashtable.buffer_with_text_id = %d \n" RESET, hashtable.buffer_with_text_id);
    fprintf (stderr, CYN "hashtable.size_text           = %lu\n" RESET, hashtable.size_text);
    fprintf (stderr, CYN "hashtable.HashTable           = %p \n" RESET, hashtable.HashTable);

    PrintHashTable (hashtable, mode);

    fprintf (stderr, CYN "------------------------------------------------------------------------------\n" RESET);

    return OK;
}

err_t PrintHashTable (hshtbl_t hashtable, mode_print_hashtable_t mode)
{
    if (mode == POINTERS)
    {
        for (size_t index = 0; index < NBASKETS; index++)
        {
            fprintf (stderr, CYN "hashtable.HashTable[%lu] = %p\n" RESET, index, &hashtable.HashTable[index]);
        }
    }
    else if (mode == KEYS)
    {
        for (size_t index = 0; index < NBASKETS; index++)
        {
            fprintf (stderr, CYN "hashtable.HashTable[%lu] = %p\n" RESET, index, &hashtable.HashTable[index]);
            PrintList (hashtable.HashTable[index]);
        }
    }

    return OK;
}

err_t PrintList (list_t list)
{
    for (size_t index = 0; list.next[index] != -1; index = (size_t) list.next[index])
    {
        fprintf (stderr, CYN "list.data[%lu] = %s\n" RESET, index, list.data[index]);
    }
    return OK;
}

err_t LoadHashTable (hshtbl_t* hashtable)
{
    size_t offset = 0;
    size_t index = 0;

    while (1)
    {
        char* key = (char*) calloc (MAX_SIZE_WORD, sizeof (*key));

        offset = 0;
        sscanf (hashtable->buffer_with_text + index, "%s%n", key, (int*)&offset);

        if (offset == 0) break;

        size_t len_of_key = strlen (key);

        uint32_t hash = murmurhash3_32 (key, len_of_key, SEED);

        printf (GRN "index = %lu => %.6s => %u\n" RESET, index, key, hash);

        hash = hash % (uint32_t) NBASKETS;

        fprintf (stderr, CYN "hash %% NBASKETS = %u\n" RESET, hash);

        int    status = 0;

        if (!FindInListValue (hashtable->HashTable[hash], key, &status))
        {
            if (status == 0)
            {
                fprintf (stderr, GRN "status = 0\n" RESET);
                ListAddTail  (&(hashtable->HashTable[hash]), key);
            }
            else
            {
                fprintf (stderr, BLU "status != 0\n" RESET);
                ListAddFairy (&(hashtable->HashTable[hash]), key);
            }

        }

        index += offset;
    }
    return OK;
}

uint32_t murmurhash3_32 (const void* key, size_t len, uint32_t seed)
{
    const    uint8_t* data = (const uint8_t*)key;
    uint32_t hash          = seed;

    for (size_t i = 0; i < len; i++)
    {
        hash ^= data[i];
        hash  = (hash << 13) | (hash >> 19);
        hash  = hash * 0x5bd1e995 + 0x165667b1;
    }

    return hash;
}

err_t CreateBufferText (char * namefile, hshtbl_t* hashtable)
{
    hashtable->buffer_with_text_id = open (namefile, O_RDONLY);
    if (hashtable->buffer_with_text_id == -1)
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

    hashtable->size_text = (size_t) fileInf.st_size + 1;

    hashtable->buffer_with_text = (char*) mmap (NULL, hashtable->size_text, PROT_READ, MAP_PRIVATE,
                                                      hashtable->buffer_with_text_id, 0);

    if (hashtable->buffer_with_text == MAP_FAILED)
    {
        perror ("mmap");
        return ERR_MMAP;
    }

    return OK;
}

err_t HashTableDtor (hshtbl_t* hashtable)
{
    if (munmap (hashtable->buffer_with_text, hashtable->size_text) == -1)
    {
        perror ("munmap");
    }
    close (hashtable->buffer_with_text_id);

    return OK;
}
