#ifndef INPUTDATA_H
#define INPUTDATA_H

typedef struct array_my_key_t
{
    char*      Onegin;
    size_t      sizeOfFile;
    int*        nPointer;
    char**      Pointers;
} array_my_key_t;

int     MakePointers            (array_my_key_t Ongn, PTR* ParamString);

bool    InputOnegin             (char** Onegin, size_t* sizeOfFile, const char* nameFile);

void    CounterSymbol           (int* nRows, char* Onegin, size_t sizeOfFile, char symbol);

bool    SizeFile                (struct stat* fileInf, const char* nameFile);

void    InitialisatorPointers   (size_t sizeOfFile, PTR** Pointers, char* Onegin, struct PTR* ParamString , int* nPointer);

#endif
