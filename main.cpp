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

    array_my_key_t array_pointers = {};

    InputOnegin (&array_pointers, "Pride.bin");

//     hshtbl_t hashtable = {};
//
//     if (argc > 2)
//     {
//         CreateBufferText (argv[1], &(hashtable.size_text), &(hashtable.buffer_with_text_id), &(hashtable.buffer_with_text));
//
//         HashTableCtor    (&hashtable);
//
//         RunHashTable    (&hashtable, keys, LOAD);
//
//         printf (GRN "## Start searching:\n" RESET);
//
//         CreateBufferText (argv[2], &(hashtable.size_test_text), &(hashtable.buffer_with_test_text_id), &(hashtable.buffer_with_test_text));
//
// //------------------Tests-------------------------------------------------
//
//         clock_t start_search_time = clock ();
//
//         for (size_t times = 0; times < NTIMES; times++)
//         {
//             RunHashTable (hashtable, keys, TEST);
//         }
//
//         clock_t end_search_time = clock ();
//
//         double cpu_time_used    = ((double) (end_search_time - start_search_time)) / CLOCKS_PER_SEC;
//
//         printf  (               "Search time: %f s\n", cpu_time_used);
//         fprintf (hashtable.Measurings_file, "%f  \n", cpu_time_used);
//
// //------------------------------------------------------------------------
//         HashTableDtor     (&hashtable);
//     }
//     else
//     {
//         fprintf (stderr, RED "Enter file fot loading   hash table as first  cmd line arg\n" RESET);
//
//         fprintf (stderr, RED "Enter file for search in hash table as second cmd line arg\n" RESET);
//         return 0;
//     }

    printf (GRN "### End of program \n\n" RESET);
    return 0;
}
