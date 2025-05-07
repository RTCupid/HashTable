#ifndef INPUTDATA_H
#define INPUTDATA_H

#include "HashTable.h"

typedef struct array_my_key_t
{
    char*       text;
    size_t      size_file;
    size_t      number_of_pointers;
    my_key_t**  pointers;
} array_my_key_t;

int     ArrayPointersCtor       (array_my_key_t* array_pointers, const char* namefile);

bool    InputBinaryFile         (array_my_key_t* array_pointers, const char* namefile);

bool    SizeFile                (struct stat* fileInf, const char* nameFile);

void    InitialisatorPointers   (array_my_key_t* array_pointers);

#endif
