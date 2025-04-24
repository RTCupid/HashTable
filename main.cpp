#include <stdio.h>
#include <stdlib.h>

#include "common/colors.h"
#include "HashTable/HashTable.h"
#include "HashTable/DumpHashTable.h"

int main (int argc, char* argv[])
{
    printf (GRN "### My  Hash Table\n" RESET);
    printf (GRN "### (c) RTCupid, 2024\n\n" RESET);

    for (int i = 0; i < argc; i++)
    {
        printf ("argv[%d] = <%s>\n", i, argv[i]);
    }

    hshtbl_t hashtable = {};

    if (argc > 2)
    {
        CreateBufferText (argv[1], &(hashtable.size_text), &(hashtable.buffer_with_text_id), &(hashtable.buffer_with_text));

        HashTableCtor    (&hashtable);

        RunHashTable    (&hashtable, LOAD);

        printf (GRN "## Start searching:\n" RESET);

        CreateBufferText (argv[2], &(hashtable.size_test_text), &(hashtable.buffer_with_test_text_id), &(hashtable.buffer_with_test_text));

        if (argc > 3)
        {
            ProcessMeasurings (&hashtable, NTIMES, atoi (argv[3]));
        }
        else
        {
            ProcessMeasurings (&hashtable, NTIMES);
        }

        HashTableDtor    (&hashtable);
    }
    else
    {
        fprintf (stderr, RED "Enter file fot loading   hash table as first  cmd line arg\n" RESET);

        fprintf (stderr, RED "Enter file for search in hash table as second cmd line arg\n" RESET);
        return 0;
    }

    printf (GRN "### End of program \n\n" RESET);
    return 0;
}
