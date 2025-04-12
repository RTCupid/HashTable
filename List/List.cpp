#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../common/colors.h"
#include "List.h"

errlst_t ListCtor (list_t* List)
{
    List->data = (int*) calloc (SIZE_LIST, sizeof (int));

    List->next = (int*) calloc (SIZE_LIST, sizeof (int));

    List->prev = (int*) calloc (SIZE_LIST, sizeof (int));

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

    PS Pause ();
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

errlst_t ListAddAfter (list_t* List, int anch, int value)
{
    if (List->prev[anch] == -1)
    {
        fprintf (stderr, RED "prev[%d] = -1\n"           RESET, anch);
        fprintf (stderr, RED "ERROR: uncorrect anchor\n" RESET);
        return UNCORRECT_ANCHOR;
    }

    int indFree = List->free;
    List->free = List->next[List->free];

    if (indFree == -1)
    {
        fprintf (stderr, RED "Func ListAddAfter anch = %d:\n"
                             "ERROR: not enough memory" RESET, anch);
        return NOT_ENOUGH_MEMORY;
    }

    List->data[indFree] = value;

    List->next[indFree] = List->next[anch];
    List->next[anch] = indFree;

    List->prev[List->next[indFree]] = indFree;
    List->prev[indFree] = anch;

    PS Pause ();
    return LIST_OK;
}
errlst_t ListAddFairy (list_t* List, int value)
{
    errlst_t error = ListAddAfter (List, 0, value);
    return error;
}

errlst_t ListAddTail (list_t* List, int value)
{
    errlst_t error = ListAddAfter (List, List->prev[0], value);
    return error;
}

errlst_t ListAddBefore (list_t* List, int anch, int value)
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

    List->data[anch] *= -1;
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
        List->data[i] = 0;
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

int FindInListValue (list_t List, int value)
{
    int index = 0;
    while (1)
    {
        if (List.next[index] == 0)
        {
            break;
        }
        if (List.data[index] == value)
        {
            printf (GRN "FindInListValue value = <%d>" RESET, value);
            printf (GRN "index of value<%d> = %d\n"    RESET, value, index);
            return index;
        }
        else
        {
            index = List.next[index];
        }
    }

    printf (YEL "FindInListValue value = <%d>"           RESET, value);
    printf (YEL "value <%d> was not found in the List\n" RESET, value);
    return 0;
}

void Pause ()
{
    printf ("Enter to continue...\n");
    getchar ();
}
