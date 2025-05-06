#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include "../common/colors.h"
#include "InputData.h"
#include "HashTable/HashTable.h"

int MakeArrayPointers (array_my_key_t* array_pointers, const char* namefile)
{
    if (!InputBinaryFile (array_pointers, namefile))
    {
        return -1;
    }

    size_t number_words = ((size_t) array_pointers->size_file / MAX_SIZE_WORD) + 1;

    array_pointers->pointers = (char**) calloc (number_words, sizeof (*array_pointers->pointers));

    fprintf (stderr, "array_pointers->pointers = <%p>\n", array_pointers->pointers);

    if (array_pointers->pointers == NULL)
    {
        fprintf (stderr, RED "ERROR: calloc array_pointers->pointers return NULL");

        return -1;
    }

    InitialisatorPointers (array_pointers);

    return 0;
}

bool InputBinaryFile (array_my_key_t* array_pointers, const char* namefile)
{
    struct stat file_inf = {};

    if (!SizeFile (&file_inf, namefile))
    {
        fprintf (stderr, "Error SizeFile!");

        exit (0);
    }

    array_pointers->text = (char*) calloc (file_inf.st_size, sizeof(*array_pointers->text));

    if (array_pointers->text == NULL)
    {
        fprintf (stderr, RED "ERROR: calloc array_pointers->text return NULL\n" RESET);

        exit (0);
    }

    FILE* binary_file = fopen (namefile, "rb");

    if (binary_file == NULL)
    {
        fprintf (stderr, RED "ERROR: binary_file <%s> opening error\n" RESET, namefile);

        printf("errno = <%d>\n", errno);

        perror(namefile);

        exit (0);
    }

    array_pointers->size_file = fread (array_pointers->text, sizeof (*array_pointers->text), file_inf.st_size, binary_file);

    if (array_pointers->size_file == 0)
    {
        printf ("errno = <%d>\n", errno);

        perror (namefile);

        exit (0);
    }

    printf ("\n%s\n", array_pointers->text);

    fclose (binary_file);

    printf ("array_pointers->size_file = <%d>\n\n", array_pointers->size_file);

    return 1;
}

bool SizeFile (struct stat* fileInf, const char* nameFile)
{
    int err = stat (nameFile, fileInf);

    if (err != 0)
    {
        printf("Stat err %d\n", err);

        return 0;
    }

    printf ("\n%ld\n", (*fileInf).st_size);

    printf ("count of char = %ld\n", (*fileInf).st_size / sizeof (char));

    return 1;
}

void InitialisatorPointers (array_my_key_t* array_pointers)
{
    fprintf (stderr, GRN "\nInitialization of Pointers:\n\n" RESET);

    for (unsigned int i = 0; i < array_pointers->size_file; i += MAX_SIZE_WORD)
    {
        printf ("i = <%d>\n", i);

        assert (i < array_pointers->size_file);

        array_pointers->pointers = array_pointers->text + i;

        fprintf (stderr, BLU " array_pointers->pointers  = %p" RESET, array_pointers->pointers);

        fprintf (stderr, YEL "[array_pointers->pointers] = %s" RESET, (char*)array_pointers->pointers);
    }
}
