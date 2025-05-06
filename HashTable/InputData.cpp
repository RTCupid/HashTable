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

int MakePointers (array_my_key_t* array_pointers, const char* namefile)
{
    if (!InputOnegin (array_pointers, namefile))
    {
        return -1;
    }

    int nRows = 0;

    char symbol = '\n';

    CounterSymbol (&nRows, *Ongn.Onegin, Ongn.sizeOfFile, symbol);

    *Ongn.Pointers = (PTR*)calloc (nRows * 2, sizeof (char*));        // каллокаю массив указателей

    printf ("Ongn.Pointers = <%p>\n", *Ongn.Pointers);

    if (Ongn.Pointers == NULL)
    {
        printf ("Error calloc Pointers");

        return -1;
    }

    printf ("Ongn.Pointers = <%p>\n", *Ongn.Pointers);

    InitialisatorPointers (Ongn.sizeOfFile, Ongn.Pointers, *Ongn.Onegin, ParamString, Ongn.nPointer);

    printf ("nPointer = <%d>\n", *Ongn.nPointer);

    return 1;
}

bool InputOnegin (array_my_key_t* array_pointers, const char* namefile)
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

void CounterSymbol (int* nRows, char* Onegin, size_t sizeOfFile, char symbol)
{
    for (unsigned int i = 0; i < sizeOfFile; i++)                    // посимвольно зачем-то вывожу начальный текст Онегина
    {                                                            // точно, я не просто вывожу, а параллельно считаю количество
        assert (i < sizeOfFile);

        if (Onegin[i] == symbol)                                     // строк, равное кличеству '\n'
        {
            printf ("Onegin[%d] = <'%c'>\n", i, symbol);
            *nRows = *nRows + 1;
        }
        else
        {
            printf ("Onegin[%d] = <%c>\n", i, Onegin[i]);
        }
    }

    printf ("nRow = <%d>\n", *nRows);
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

void InitialisatorPointers (size_t sizeOfFile, char* Onegin, int* nPointer)
{
    fprintf (stderr, GRN "\nInitialization of Pointers:\n\n" RESET);

    printf ("&ParamString = <%p>\n", ParamString);
    printf ("&Pointers    = <%p>\n", *Pointers);

    printf ("Onegin = <%p>\n", Onegin);

    ParamString->PtrStart = Onegin;

    for (unsigned int i = 0; i < sizeOfFile; i++)
    {
        printf ("i = <%d>\n", i);

        assert (i < sizeOfFile);

        printf ("nPointer = <%d>\n", *nPointer);

        if (Onegin[i] == '\n')
        {
            printf ("nPointer = <%d>\n", *nPointer);

            (*Pointers)[*nPointer] = *ParamString;

            printf (" Pointers[i].PtrStart = <%p>\n", ((*Pointers)[*nPointer]).PtrStart);

            *nPointer = *nPointer + 1;
            ParamString->PtrStart = &Onegin[i + 1];
        }
    }

    printf ("nPointer = <%d>\n", *nPointer);
}
