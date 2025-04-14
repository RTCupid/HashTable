#ifndef LIST_H
#define LIST_H

#define PS if(0)
#define LIST_DBG if(0)

typedef struct
{
    char** data;
    int*   next;
    int*   prev;
    int    free;
} list_t;

enum errlst_t
{
    LIST_OK,
    UNCORRECT_ANCHOR,
    NULL_ANCHOR,
    NOT_ENOUGH_MEMORY,
    LOG_FILE_UNCORRECT,
    DESYNCHRON_NEXT_PREV,
    LIST_IS_LOOP,
};

const int SIZE_LIST = 20;
const int SIZE_FREE = SIZE_LIST - 1;

errlst_t ListCtor       (list_t* List);

errlst_t ListDtor       (list_t* List);

errlst_t ListAddAfter   (list_t* List, int anch, char* value);

errlst_t ListAddBefore  (list_t* List, int anch, char* value);

errlst_t ListAddFairy   (list_t* List, char* value);

errlst_t ListAddTail    (list_t* List, char* value);

errlst_t ListDel        (list_t* List, int anch);

errlst_t ClearList      (list_t* List);

int      FindInListValue (list_t List, char* value, int* status);

void     Pause ();
#endif
