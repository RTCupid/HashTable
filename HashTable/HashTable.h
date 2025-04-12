#ifndef HASHTABLE_H
#define HASHTABLE_H

enum err_t
{
    OK,
    ERR_CTOR,
    ERR_OPEN,
    ERR_FTRUNCATE,
    ERR_MMAP,
    ERR_STAT,
};

const int MAX_SIZE_WORD = 30;

typedef struct hshtbl_t
{
    size_t size_text;
    int    buffer_with_text_id;
    char*  buffer_with_text;
} hshtbl_t;

err_t HashTableCtor (char* namefile, hshtbl_t* hashtable);

err_t HashTableDtor (hshtbl_t* hashtable);

err_t CreateBufferText (char * namefile, hshtbl_t* hashtable);

#endif
