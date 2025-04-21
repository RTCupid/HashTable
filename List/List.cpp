#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <immintrin.h>

#include "../common/colors.h"
#include "List.h"

extern "C" __m256i _My_Strcmp (char* first_string, char* second_string);

errlst_t ListCtor (list_t* List)
{
    List->data = (char**) calloc (SIZE_LIST, sizeof (*List->data));

    List->next = (int*)   calloc (SIZE_LIST, sizeof (int));

    List->prev = (int*)   calloc (SIZE_LIST, sizeof (int));

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

errlst_t ListAddAfter (list_t* List, int anch, char* value)
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
errlst_t ListAddFairy (list_t* List, char* value)
{
    errlst_t error = ListAddAfter (List, 0, value);
    return error;
}

errlst_t ListAddTail (list_t* List, char* value)
{
    errlst_t error = ListAddAfter (List, List->prev[0], value);
    return error;
}

errlst_t ListAddBefore (list_t* List, int anch, char* value)
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

int FindInListValue (list_t List, char* value, int* status)
{
    int index = 0;
    while (1)
    {
        LIST_DBG fprintf (stderr, "index = %d\n", index);

        if (index == 0)
        {
            if (List.next[index] == 0)
            {
                break;
            }

            index   = List.next[index];
            LIST_DBG fprintf (stderr, MAG "index changed! index = %d\n", index);
            *status = 1;
        }

        /*if (List.data[index] == NULL)
        {
            fprintf (stderr, RED "List.data[%d] == NULL\n" RESET, index);
            break;
        }

        if (value == NULL)
        {
            fprintf (stderr, RED "value == NULL\n" RESET);
            break;
        } */

        LIST_DBG fprintf (stderr, "Start strcmp\n");

        if (_My_Strcmp (List.data[index], value) == 0)
        {
            LIST_DBG LIST_DBG printf (GRN "FindInListValue value = <%s>\n"  RESET, value);
            LIST_DBG LIST_DBG printf (GRN "index of value <%s>   = %d\n"    RESET, value, index);
            return index;
        }
        else
        {
            if (List.next[index] == 0)
            {
                break;
            }

            index   = List.next[index];
            *status = 1;
        }
    }

    LIST_DBG printf (YEL "FindInListValue value = <%s>\n"         RESET, value);
    LIST_DBG printf (YEL "value <%s> was not found in the List\n" RESET, value);
    return 0;
}

void Pause ()
{
    printf ("Enter to continue...\n");
    getchar ();
}
