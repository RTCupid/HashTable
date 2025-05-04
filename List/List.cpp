#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <immintrin.h>

#include "../common/colors.h"
#include "List.h"
#include "../HashTable/HashTable.h"

extern "C" inline int _My_Strcmp (char* first_string, char* second_string);

errlst_t ListCtor (list_t* List)
{
    List->data = (my_key_t*) calloc (SIZE_LIST, sizeof (*List->data));

    List->next = (int*)      calloc (SIZE_LIST, sizeof (int));

    List->prev = (int*)      calloc (SIZE_LIST, sizeof (int));

    for (int i = 1; i < SIZE_FREE; i++)
    {
        List->next[i] = i + 1;
        List->prev[i] = -1;
    }
    List->next[SIZE_FREE] = -1;
    List->prev[SIZE_FREE] = -1;

    List->next[0] = 0;
    List->prev[0] = 0;

    List->free = 1;

    return LIST_OK;
}

errlst_t ListDtor (list_t* List)
{
    free (List->data);
    List->data = NULL;

    free (List->next);
    List->next = NULL;

    free (List->prev);
    List->prev = NULL;

    return LIST_OK;
}

errlst_t ListAddAfter (list_t* List, int anch, my_key_t value)
{
    if (List->prev[anch] == -1)
    {
        fprintf (stderr, RED "prev[%d] = -1\n"           RESET, anch);
        fprintf (stderr, RED "ERROR: uncorrect anchor\n" RESET);
        return UNCORRECT_ANCHOR;
    }

    int indFree = List->free;
    List->free  = List->next[List->free];

    if (indFree == -1)
    {
        fprintf (stderr, RED "Func ListAddAfter anch = %d:\n"
                             "ERROR: not enough memory" RESET, anch);
        return NOT_ENOUGH_MEMORY;
    }

    List->data[indFree] = value;

    List->next[indFree] = List->next[anch];
    List->next[anch]    = indFree;

    List->prev[List->next[indFree]] = indFree;
    List->prev[indFree]             = anch;

    PS Pause ();
    return LIST_OK;
}
errlst_t ListAddFairy (list_t* List, my_key_t value)
{
    errlst_t error = ListAddAfter (List, 0, value);
    return error;
}

errlst_t ListAddTail (list_t* List, my_key_t value)
{
    errlst_t error = ListAddAfter (List, List->prev[0], value);
    return error;
}

errlst_t ListAddBefore (list_t* List, int anch, my_key_t value)
{
    errlst_t error = ListAddAfter (List, List->prev[anch], value);
    return error;
}

errlst_t ListDel (list_t* List, int anch)
{
    if (List->prev[anch] == -1)
    {
        fprintf (stderr, RED "prev[%d] = -1\n"           RESET, anch);
        fprintf (stderr, RED "ERROR: uncorrect anchor\n" RESET);
        return UNCORRECT_ANCHOR;
    }
    if (anch == 0)
    {
        fprintf (stderr, RED "ERROR: anchor is null\n"   RESET);
        return NULL_ANCHOR;
    }

    List->data[anch] = NULL;
    List->next[List->prev[anch]] = List->next[anch];
    List->prev[List->next[anch]] = List->prev[anch];

    List->prev[anch] = -1;
    List->next[anch] = List->free;
    List->free = anch;

    PS Pause ();
    return LIST_OK;
}

errlst_t ClearList (list_t* List)
{
    for (int i = 1; i < SIZE_FREE; i++)
    {
        LIST_DBG fprintf (stderr, "clear %d key\n", i);
        free (List->data[i]);
        List->data[i] = NULL;
        List->next[i] = i + 1;
        List->prev[i] = -1;
    }
    List->next[SIZE_FREE] = -1;
    List->prev[SIZE_FREE] = -1;

    List->next[0] = 0;
    List->prev[0] = 0;

    List->free = 1;

    return LIST_OK;
}

int FindInListValue (list_t List, my_key_t key, int* status)
{
    int index = 0;

    if (List.next[index] != 0)
    {
        index = List.next[index];

        *status = 1;

        while (1)
        {
            uint32_t result_of_compare = MyStrcmp (List.data[index], key);

            if (result_of_compare == COMPARE_M128_MASK)
            {
                return index;
            }
            else
            {
                if (List.next[index] == 0)
                {
                    break;
                }

                index   = List.next[index];
            }
        }
    }

    return 0;
}

inline uint32_t MyStrcmp (my_key_t first_string, my_key_t second_string)
{
    __m128i result_of_compare = _mm_cmpeq_epi32   (*first_string, *second_string);

    uint32_t mask             = (uint32_t) _mm_movemask_epi8 (result_of_compare);

    return mask;
}

void Pause ()
{
    printf  ("Enter to continue...\n");
    getchar ();
}

void DebugPrint_m128i (__m128i vector)
{
    uint32_t values[4];
    _mm_storeu_si128 ((__m128i*)values, vector);

    printf (BMAG "-------------------------\n");

    printf ("Vector (uint32_t): ");
    for (int i = 0; i < 4; i++) {
        printf ("%08x ", values[i]);
    }
    printf ("\n-------------------------\n" RESET);
}
