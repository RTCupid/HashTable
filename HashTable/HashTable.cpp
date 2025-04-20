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
#include "DumpHashTable.h"

extern "C" size_t _My_Strlen (char* string);

err_t HashTableCtor (hshtbl_t* hashtable)
{
    printf           (GRN "## Start HashCtor\n" RESET);

    hashtable->log_file = fopen ("build/log_file.txt", "wt");

    CreateHashTable  (hashtable);

    return OK;
}

err_t RunHashTable (hshtbl_t* hashtable, char* name_test_file)
{
    CreateBufferText (name_test_file, &(hashtable->size_test_text), &(hashtable->buffer_with_test_text_id), &(hashtable->buffer_with_test_text));

    size_t offset = 0;
    size_t index = 0;

    while (1)
    {
        char* key = (char*) calloc (MAX_SIZE_WORD, sizeof (*key));

//         for (size_t offset = 0; offset < hashtable->size_test_text; )
//         {
//             size_t start_of_string = offset;
//
//             while (offset < hashtable->size_test_text && hashtable->buffer_with_test_text[offset] != '\n')
//             {
//                 offset++;
//             }
//
//             printf ("%.*s\n", (int)(offset - line_start), hashtable->buffer_with_test_text + start_of_string);
//
//             if (offset < hashtable->size_test_text) offset++; // Пропускаем '\n'
//         }


        offset = 0;
        sscanf (hashtable->buffer_with_test_text + index, "%s%n", key, (int*)&offset);

        if (offset == 0)
        {
            free (key);
            key = NULL;
            break;
        }

        SearchHashTable (hashtable, key, TEST);

        index += offset;
    }

    return OK;
}

err_t SearchHashTable (hshtbl_t* hashtable, my_key_t key, mode_hashtable_t mode)
{
    size_t len_of_key = _My_Strlen (key);

    uint32_t hash     = murmurhash3_32 (key, len_of_key, SEED);

    HASHTABLE_DBG printf  (GRN "key \"%.6s\" => hash =  %u\n" RESET, key, hash);
    HASHTABLE_DBG fprintf (hashtable->log_file, "key \"%.6s\" => hash =  %u\n", key, hash);

    hash = hash % (uint32_t) NBASKETS;

    HASHTABLE_DBG printf  (CYN "hash %% NBASKETS = %u\n" RESET, hash);
    HASHTABLE_DBG fprintf (hashtable->log_file, "hash %% NBASKETS = %u\n", hash);

    int    status = 0;

    if (!FindInListValue (hashtable->HashTable[hash], key, &status))
    {
        if (status == 0)
        {
            fprintf (hashtable->log_file, "status = 0\n");

            HASHTABLE_DBG printf  (MAG                  "FindInListValue value = <%s>\n" RESET, key);
            HASHTABLE_DBG fprintf (hashtable->log_file, "FindInListValue value = <%s>\n",       key);

            HASHTABLE_DBG printf  (MAG                  "value <%s> was not found in the List\n" RESET, key);
            HASHTABLE_DBG fprintf (hashtable->log_file, "value <%s> was not found in the List\n",       key);

            if (mode == LOAD)
            {
                ListAddTail  (&(hashtable->HashTable[hash]), key);
            }
            else
            {
                free (key);
                key = NULL;
            }

            return OK;
        }
        else
        {
            fprintf (hashtable->log_file, "status != 0\n");

            HASHTABLE_DBG printf  (MAG                  "FindInListValue value = <%s>\n" RESET, key);
            HASHTABLE_DBG fprintf (hashtable->log_file, "FindInListValue value = <%s>\n",       key);

            HASHTABLE_DBG printf  (MAG                  "value <%s> was not found in the List\n" RESET, key);
            HASHTABLE_DBG fprintf (hashtable->log_file, "value <%s> was not found in the List\n",       key);

            if (mode == LOAD)
            {
                ListAddFairy (&(hashtable->HashTable[hash]), key);
            }
            else
            {
                free (key);
                key = NULL;
            }

            return OK;
        }
    }

    HASHTABLE_DBG printf  (YEL                  "FindInListValue value = <%s>\n" RESET, key);
    HASHTABLE_DBG fprintf (hashtable->log_file, "FindInListValue value = <%s>\n",       key);

    HASHTABLE_DBG printf  (YEL                  "value <%s> was found in %u Basket\n" RESET, key, hash);
    HASHTABLE_DBG fprintf (hashtable->log_file, "value <%s> was found in %u Basket\n",       key, hash);

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

err_t LoadHashTable (hshtbl_t* hashtable)
{
    size_t offset = 0;
    size_t index = 0;

    while (1)
    {
        char* key = (char*) calloc (MAX_SIZE_WORD, sizeof (*key));

        offset = 0;
        sscanf (hashtable->buffer_with_text + index, "%s%n", key, (int*)&offset);

        if (offset == 0)
        {
            free (key);
            key = NULL;
            break;
        }

        SearchHashTable (hashtable, key, LOAD);

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
    if (munmap (hashtable->buffer_with_text, hashtable->size_text) == -1)
    {
        perror ("munmap");
    }

    close  (hashtable->buffer_with_text_id);

    ClearHashTable (hashtable);

    fclose (hashtable->log_file);

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
