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
#include "InputData.h"
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

err_t LoadHashTable (hshtbl_t* hashtable, array_my_key_t* array_pointers)
{
    fprintf (stderr, GRN "Start Load HashTable\n" RESET);

    for (size_t index_pointer = 0; index_pointer < array_pointers->number_of_pointers; index_pointer++)
    {
        HASHTABLE_DBG fprintf (stderr, "index_pointer = %lu\n", index_pointer);
        HASHTABLE_DBG fprintf (stderr, "array_pointers->number_of_pointers = %lu\n", array_pointers->number_of_pointers);

        assert (index_pointer < array_pointers->number_of_pointers);

        uint32_t hash = 0;

        asm __volatile__
        (
            "mov %0, 0xEDABC526\n"                            //uint32_t hash = SEED;
            "mov rcx, 16\n"                                   // rcx          = 0;

            "loop_MAX_SIZE_WORD_times_load:\n"

            "movzx eax, byte ptr [%1 + rcx * 1]\n"            // eax   = byte ptr [%1 + rcx * 1];
            "xor %0, eax\n"                                   // hash ^= byte ptr [%1 + rcx * 1];

            "mov edx, %0\n"                                   // edx   = hash;

            "shl %0,  13\n"                                   // hash  = hash << 13;

            "shr edx, 19\n"                                   // edx   = hash >> 19;

            "or   %0, edx\n"                                  // hash  = (hash << 13) | (hash >> 19);

            "imul %0, 0x5bd1e995\n"                           // hash *= 0x5bd1e995;

            "add  %0, 0x165667b1\n"                           // hash += 0x165667b1;

            "loop loop_MAX_SIZE_WORD_times_load\n"            // if (--rcx != 0) goto loop_MAX_SIZE_WORD_times;

            : "=r" (hash)
            : "r" (array_pointers->pointers[index_pointer])
            : "rcx", "edx"
        );

//---------Rewrite-this-function---------------------------------------------------
//        uint32_t hash = murmurhash3 (array_pointers->pointers[index_pointer]);
//---------------------------------------------------------------------------------

        hash          = hash % (uint32_t) NBASKETS;

        int    status = 0;

        if (!FindInListValue (&(hashtable->HashTable[hash]), array_pointers->pointers[index_pointer], &status))
        {
            NOT_PROFILING printf  (MAG                  "value <%s> was not found in the List\n" RESET,
                                                                    (char*) array_pointers->pointers[index_pointer]);
            HASHTABLE_DBG fprintf (hashtable->log_file, "value <%s> was not found in the List\n",
                                                                    (char*) array_pointers->pointers[index_pointer]);

            if (status == 0)
            {
                HASHTABLE_DBG fprintf (hashtable->log_file, "status = 0,  add key to tail of list\n");
                ListAddTail  (&(hashtable->HashTable[hash]), array_pointers->pointers[index_pointer]);
            }
            else
            {
                HASHTABLE_DBG fprintf (hashtable->log_file, "status != 0, add key to head of list\n");
                ListAddFairy (&(hashtable->HashTable[hash]), array_pointers->pointers[index_pointer]);
            }
        }
        else
        {
            PRINTF_IF_FIND printf  (YEL                  "value <%s> was found in %u Basket\n" RESET, (char*) array_pointers->pointers[index_pointer], hash);
            HASHTABLE_DBG  fprintf (hashtable->log_file, "value <%s> was found in %u Basket\n",       (char*) array_pointers->pointers[index_pointer], hash);
        }
    }

    fprintf (stderr, GRN "Load HashTable Completed\n" RESET);

    return OK;
}

err_t RunHashTable  (hshtbl_t* hashtable, array_my_key_t* array_pointers)
{
    NOT_PROFILING fprintf (stderr, GRN "Start Run HashTable\n" RESET);

    for (size_t index_pointer = 0; index_pointer < array_pointers->number_of_pointers; index_pointer++)
    {
        HASHTABLE_DBG fprintf (stderr, "index_pointer = %lu\n", index_pointer);
        HASHTABLE_DBG fprintf (stderr, "array_pointers->number_of_pointers = %lu\n", array_pointers->number_of_pointers);

        assert (index_pointer < array_pointers->number_of_pointers);

        uint32_t hash = 0;

        asm __volatile__
        (
            "mov %0, 0xEDABC526\n"                            //uint32_t hash = SEED;
            "mov rcx, 16\n"                                   // rcx          = 0;

            "loop_MAX_SIZE_WORD_times_run:\n"

            "xor %0, [%1 + rcx * 1]\n"                        // hash ^= [%1 + rcx * 1];

            "mov edx, %0\n"                                   // edx   = hash;

            "shl %0,  13\n"                                   // hash  = hash << 13;

            "shr edx, 19\n"                                   // edx   = hash >> 19;

            "or   %0, edx\n"                                  // hash  = (hash << 13) | (hash >> 19);

            "imul %0, 0x5bd1e995\n"                           // hash *= 0x5bd1e995;

            "add  %0, 0x165667b1\n"                           // hash += 0x165667b1;

            "dec  rcx\n"                                      // if (--rcx != 0)

            "jne  loop_MAX_SIZE_WORD_times_run\n"                 // goto loop_MAX_SIZE_WORD_times;

            : "=r" (hash)
            : "r" (array_pointers->pointers[index_pointer])
            : "rcx", "edx"
        );

//---------Rewrite-this-function---------------------------------------------------
//        uint32_t hash = murmurhash3 (array_pointers->pointers[index_pointer]);
//---------------------------------------------------------------------------------
        hash          = hash % (uint32_t) NBASKETS;

        int    status = 0;

        if (!FindInListValue (&(hashtable->HashTable[hash]), array_pointers->pointers[index_pointer], &status))
        {
            NOT_PROFILING printf  (MAG                  "value <%s> was not found in the List\n" RESET,
                                                                    (char*) array_pointers->pointers[index_pointer]);
            HASHTABLE_DBG fprintf (hashtable->log_file, "value <%s> was not found in the List\n",
                                                                    (char*) array_pointers->pointers[index_pointer]);
        }
        else
        {
            PRINTF_IF_FIND printf  (YEL                  "value <%s> was found in %u Basket\n" RESET, (char*) array_pointers->pointers[index_pointer], hash);
            HASHTABLE_DBG  fprintf (hashtable->log_file, "value <%s> was found in %u Basket\n",       (char*) array_pointers->pointers[index_pointer], hash);
        }
    }

    NOT_PROFILING fprintf (stderr, GRN "Run HashTable Completed\n" RESET);

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

__attribute__((noinline))
uint32_t murmurhash3 (const void* key)
{
    const    uint8_t* data = (const uint8_t*)key;

    uint32_t hash          = SEED;

    for (size_t i = 0; i < MAX_SIZE_WORD; i++)
    {
        hash ^= data[i];

        hash  = (hash << 13) | (hash >> 19);

        hash  = hash * 0x5bd1e995 + 0x165667b1;
    }

    return hash;
}

err_t HashTableDtor (hshtbl_t* hashtable)
{
    ClearHashTable   (hashtable);

    fclose (hashtable->log_file);

    return OK;
}

err_t ClearHashTable (hshtbl_t* hashtable)
{
    for (size_t index = 0; index < NBASKETS; index++)
    {
        ListDtor  (&hashtable->HashTable[index]);
    }

    free (hashtable->HashTable);
    hashtable->HashTable = NULL;

    return OK;
}
