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

int MakePointers (MP Ongn, PTR* ParamString)
{
    if (!InputOnegin (Ongn.Onegin, &Ongn.sizeOfFile, Ongn.nameFile))
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

bool InputOnegin (char** Onegin, size_t* sizeOfFile, const char* nameFile)
{
    struct stat fileInf = {};

    if (!SizeFile (&fileInf, nameFile))
    {
        fprintf (stderr, "Error SizeFile!");

        exit (0);
    }

    *Onegin = (char*)calloc (fileInf.st_size + 1, sizeof(char));

    if (*Onegin == NULL)
    {
        fprintf (stderr, "Onegin = NULL");

        exit (0);
    }

    FILE* file = fopen (nameFile, "rt");

    if (file == NULL)
    {
        fprintf (stderr, "File opening error\n");

        printf("errno = <%d>\n", errno);

        perror(nameFile);

        exit (0);
    }

    *sizeOfFile = fread (*Onegin, sizeof (char), fileInf.st_size, file); // с помощью fread читаю файл в буффер, сохраняю возвращаемое значение fread ()

    if (*sizeOfFile == 0)
    {
        printf ("errno = <%d>\n", errno);

        perror (nameFile);

        exit (0);
    }

    printf ("\n%s\n", *Onegin);                                      // вывожу начальный текст Онегина

    fclose (file);                                                   // закрываю файл

    printf ("sizeOfFile = <%d>\n\n", *sizeOfFile);

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


void InitialisatorPointers (size_t sizeOfFile, PTR** Pointers, char* Onegin, struct PTR* ParamString , int* nPointer)
{
    printf ("\nInitialization of Pointers:\n\n");

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
            ParamString->lenString = (Onegin + i - 1) - ParamString->PtrStart;

            printf ("lenString = <%d>\n", ParamString->lenString);

            printf ("nPointer = <%d>\n", *nPointer);

            (*Pointers)[*nPointer] = *ParamString;

            printf (" Pointers[i].PtrStart = <%p>\n", ((*Pointers)[*nPointer]).PtrStart);
            printf ("2\n");

            *nPointer = *nPointer + 1;
            ParamString->PtrStart = &Onegin[i + 1];
        }
    }

    printf ("nPointer = <%d>\n", *nPointer);
}
