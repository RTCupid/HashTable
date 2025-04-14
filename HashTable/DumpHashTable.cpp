#include <stdio.h>

#include "../common/colors.h"
#include "HashTable.h"
#include "../List/List.h"
#include "DumpHashTable.h"

err_t DumpHashTable (hshtbl_t hashtable, mode_print_hashtable_t mode)
{
    fprintf (stderr, CYN "-Dump-------------------------------------------------------------------------\n" RESET);
    fprintf (stderr, CYN "hashtable.buffer_with_text    = %p \n" RESET, hashtable.buffer_with_text);
    fprintf (stderr, CYN "hashtable.buffer_with_text_id = %d \n" RESET, hashtable.buffer_with_text_id);
    fprintf (stderr, CYN "hashtable.size_text           = %lu\n" RESET, hashtable.size_text);
    fprintf (stderr, CYN "hashtable.HashTable           = %p \n" RESET, hashtable.HashTable);

    PrintHashTable (hashtable, mode);

    fprintf (stderr, CYN "------------------------------------------------------------------------------\n" RESET);

    return OK;
}

err_t PrintHashTable (hshtbl_t hashtable, mode_print_hashtable_t mode)
{
    if (mode == POINTERS)
    {
        for (size_t index = 0; index < NBASKETS; index++)
        {
            fprintf (stderr, CYN "hashtable.HashTable[%lu] = %p\n" RESET, index, &hashtable.HashTable[index]);
        }
    }
    else if (mode == KEYS)
    {
        for (size_t index = 0; index < NBASKETS; index++)
        {
            fprintf (stderr, CYN "hashtable.HashTable[%lu] = %p\n" RESET, index, &hashtable.HashTable[index]);
            PrintList (hashtable.HashTable[index]);
        }
    }

    return OK;
}

err_t PrintList (list_t list)
{
    for (size_t index = 0; list.next[index] != -1;)
    {
        fprintf (stderr, CYN "list.data[%lu] = %s\n" RESET, index, list.data[index]);

        if (list.next[index] == 0)
        {
            break;
        }
        else
        {
            index = (size_t) list.next[index];
        }
    }
    return OK;
}
