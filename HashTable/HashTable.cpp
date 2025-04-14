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

err_t HashTableCtor (char* namefile, hshtbl_t* hashtable)
{
    printf           (GRN "Start HashCtor\n" RESET);

    hashtable->log_file = fopen ("build/log_file.txt", "wt");

    CreateBufferText (namefile, hashtable);

    CreateHashTable  (hashtable);

    DumpHashTable    (*hashtable, POINTERS);

    LoadHashTable    (hashtable);

    DumpHashTable    (*hashtable, KEYS);

    //char* key = "Mikhalina luchshaya";

    //SearchHashTable (hashtable, key);

    return OK;
}

err_t SearchHashTable (hshtbl_t* hashtable, my_key_t key)
{
    size_t len_of_key = strlen (key);

    uint32_t hash = murmurhash3_32 (key, len_of_key, SEED);

    HASHTABLE_DBG printf  (GRN "key \"%.6s\" => hash =  %u\n" RESET, key, hash);
    fprintf (hashtable->log_file, "key \"%.6s\" => hash =  %u\n", key, hash);

    hash = hash % (uint32_t) NBASKETS;

    HASHTABLE_DBG printf (CYN "hash %% NBASKETS = %u\n" RESET, hash);
    fprintf (hashtable->log_file, "hash %% NBASKETS = %u\n", hash);

    int    status = 0;

    if (!FindInListValue (hashtable->HashTable[hash], key, &status))
    {
        if (status == 0)
        {
            fprintf (hashtable->log_file, "status = 0\n");

            HASHTABLE_DBG printf  (MAG                  "FindInListValue value = <%s>\n" RESET, key);
            fprintf (hashtable->log_file, "FindInListValue value = <%s>\n",       key);

            HASHTABLE_DBG printf  (MAG                  "value <%s> was not found in the List\n" RESET, key);
            fprintf (hashtable->log_file, "value <%s> was not found in the List\n",       key);

            ListAddTail  (&(hashtable->HashTable[hash]), key);

            return OK;
        }
        else
        {
            fprintf (hashtable->log_file, "status != 0\n");

            HASHTABLE_DBG printf  (MAG                  "FindInListValue value = <%s>\n" RESET, key);
            fprintf (hashtable->log_file, "FindInListValue value = <%s>\n",       key);

            HASHTABLE_DBG printf  (MAG                  "value <%s> was not found in the List\n" RESET, key);
            fprintf (hashtable->log_file, "value <%s> was not found in the List\n",       key);

            ListAddFairy (&(hashtable->HashTable[hash]), key);

            return OK;
        }
    }

    HASHTABLE_DBG printf  (YEL                  "FindInListValue value = <%s>\n" RESET, key);
    fprintf (hashtable->log_file, "FindInListValue value = <%s>\n",       key);

    HASHTABLE_DBG printf  (YEL                  "value <%s> was found in %u Basket\n" RESET, key, hash);
    fprintf (hashtable->log_file, "value <%s> was found in %u Basket\n",       key, hash);

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

        SearchHashTable (hashtable, key);

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

    close  (hashtable->buffer_with_text_id);

    ClearHashTable (hashtable);

    fclose (hashtable->log_file);

    return OK;
}

err_t ClearHashTable (hshtbl_t* hashtable)
{
    for (size_t index = 0; index < NBASKETS; index++)
    {
        fprintf   (stderr, YEL "start clear %lu list\n" RESET, index);
        ClearList (&hashtable->HashTable[index]);
        ListDtor  (&hashtable->HashTable[index]);
        fprintf   (stderr, GRN "end   clear %lu list\n" RESET, index);
    }

    free (hashtable->HashTable);
    hashtable->HashTable = NULL;

    return OK;
}
