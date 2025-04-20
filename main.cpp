#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "common/colors.h"
#include "HashTable/HashTable.h"
#include "HashTable/DumpHashTable.h"

extern "C" size_t _My_Strlen (char* string);

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


        char* key = (char*) calloc (MAX_SIZE_WORD, sizeof (*key));

        sscanf (hashtable.buffer_with_text, "%s", key);

        size_t size = _My_Strlen (key);

        fprintf (stderr, YEL "Size of %s = %lu\n", key, size);


        HashTableCtor    (&hashtable);

        LoadHashTable    (&hashtable);

        printf (GRN "## Start searching:\n" RESET);

        clock_t start_program_time = clock ();
                                                    //;-------------------------|
        RunHashTable     (&hashtable, argv[2]);     //; Searching words in text |
        RunHashTable     (&hashtable, argv[2]);     //;                         |
                                                    //;--------------------------
        clock_t end_program_time   = clock ();

        HashTableDtor    (&hashtable);

        double cpu_time_used       = ((double) (end_program_time - start_program_time)) / CLOCKS_PER_SEC;

        printf ("Search time: %f s\n", cpu_time_used);
    }
    else
    {
        fprintf (stderr, RED "Enter file fot loading hash table as first cmd line arg\n" RESET);

        fprintf (stderr, RED "Enter file for search in hash table as second cmd line arg\n" RESET);
        return 0;
    }

    printf (GRN "### End of program \n\n" RESET);
    return 0;
}
