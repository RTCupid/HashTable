#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <assert.h>

#include "../common/colors.h"
#include "HashTable.h"
#include "../List/List.h"

err_t HashTableCtor (char* namefile, hshtbl_t* hashtable)
{
    printf (GRN "Start HashCtor\n" RESET);

    CreateBufferText (namefile, hashtable);

    fprintf (stderr, "hashtable->size_text = %lu\n", hashtable->size_text);
    LoadHashTable (hashtable);

    return OK;
}

err_t LoadHashTable (hshtbl_t* hashtable)
{
    size_t offset = 0;
    size_t index = 0;

    while (1)
    {
        char* word = (char*) calloc (MAX_SIZE_WORD, sizeof (*word));

        offset = 0;
        sscanf (hashtable->buffer_with_text + index, "%s%n", word, (int*)&offset);

        if (offset == 0) break;

        printf (GRN "index = %lu => %s\n" RESET, index, word);

        index += offset;
    }
    return OK;
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
