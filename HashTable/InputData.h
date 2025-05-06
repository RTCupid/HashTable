#ifndef INPUTDATA_H
#define INPUTDATA_H

typedef struct array_my_key_t
{
    char*       text;
    size_t      size_file;
    int*        number_of_pointers;
    char**      pointers;
} array_my_key_t;

int     MakePointers            (array_my_key_t* array_pointers, const char* namefile);

bool    InputOnegin             (array_my_key_t* array_pointers, const char* namefile);

void    CounterSymbol           (int* nRows, char* Onegin, size_t sizeOfFile, char symbol);

bool    SizeFile                (struct stat* fileInf, const char* nameFile);

void    InitialisatorPointers   (size_t sizeOfFile, char* Onegin, int* nPointer);

#endif
