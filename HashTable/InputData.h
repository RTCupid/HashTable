#ifndef INPUTDATA_H
#define INPUTDATA_H

typedef struct PTR
{
    char* PtrStart;
    int   lenString;
} PTR;

typedef struct MP
{
    char**      Onegin;
    size_t      sizeOfFile;
    int*        nPointer;
    PTR**       Pointers;
} MP;

int     MakePointers            (MP Ongn, PTR* ParamString);

bool    InputOnegin             (char** Onegin, size_t* sizeOfFile, const char* nameFile);

void    CounterSymbol           (int* nRows, char* Onegin, size_t sizeOfFile, char symbol);

bool    SizeFile                (struct stat* fileInf, const char* nameFile);

void    InitialisatorPointers   (size_t sizeOfFile, PTR** Pointers, char* Onegin, struct PTR* ParamString , int* nPointer);

#endif
