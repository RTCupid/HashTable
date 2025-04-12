#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include "../common/colors.h"
#include "HashTable.h"
#include "../List/List.h"
#include "../FreadFile/FreadFile.h"

static int ID = 0;

err_t HashTableCtor (char* namefile, hshtbl_t* hashtable)
{
    printf (GRN "Start HashCtor\n" RESET);

    CreateBufferText (namefile, hashtable);

    char word[30] = {};

    int offset = 0;
    sscanf (hashtable->buffer_with_text, "%s%n", word, &offset);
    ID += offset;

    printf ("File Jane Osten: %s\n", word);

    offset = 0;
    sscanf (hashtable->buffer_with_text + ID, "%s%n", word, &offset);
    ID += offset;

    printf ("File Jane Osten: %s\n", word);

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
