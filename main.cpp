#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "common/colors.h"
#include "HashTable/HashTable.h"
#include "HashTable/DumpHashTable.h"
#include "HashTable/InputData.h"

int main (int argc, char* argv[])
{
    printf (GRN "### My  Hash Table\n" RESET);
    printf (GRN "### (c) RTCupid, 2024\n\n" RESET);

    for (int i = 0; i < argc; i++)
    {
        printf ("argv[%d] = <%s>\n", i, argv[i]);
    }

    if (argc > 2)
    {
        array_my_key_t load_array_pointers = {};
        array_my_key_t test_array_pointers = {};

        hshtbl_t hashtable = {};

        InputBinaryFile (&load_array_pointers, "WarAndPeace.bin");

        HashTableCtor    (&hashtable);

        RunHashTable    (&hashtable, &load_array_pointers, LOAD);

        printf (GRN "## Start searching:\n" RESET);

        InputBinaryFile (&test_array_pointers, "Pride.bin");

//------------------Tests-------------------------------------------------

        clock_t start_search_time = clock ();

        for (size_t times = 0; times < NTIMES; times++)
        {
            RunHashTable (&hashtable, &test_array_pointers, TEST);
        }

        clock_t end_search_time = clock ();

        double cpu_time_used    = ((double) (end_search_time - start_search_time)) / CLOCKS_PER_SEC;

        printf  (              "Search time: %f s\n", cpu_time_used);
        fprintf (hashtable.Measurings_file, "%f  \n", cpu_time_used);

//------------------------------------------------------------------------
        HashTableDtor     (&hashtable);
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
