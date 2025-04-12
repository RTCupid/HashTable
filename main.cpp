#include <stdio.h>

#include "common/colors.h"
#include "HashTable/HashTable.h"


int main (int argc, char* argv[])
{
    printf (GRN "# My  Hash Table\n" RESET);
    printf (GRN "# (c) RTCupid, 2024\n\n" RESET);

    for (int i = 0; i < argc; i++)
    {
        printf ("argv[%d] = <%s>\n", i, argv[i]);
    }

    hshtbl_t hashtable = {};

    if (argc > 1)
    {
        HashTableCtor (argv[1], &hashtable);
    }

    HashTableDtor (&hashtable);

    return 0;
}
