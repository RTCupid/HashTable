#ifndef DUMPHASHTABLE_H
#define DUMPHASHTABLE_H

enum   mode_print_hashtable_t;

struct hshtbl_t;

struct list_t;

err_t       DumpHashTable     (hshtbl_t hashtable, mode_print_hashtable_t mode);

err_t       PrintHashTable    (hshtbl_t hashtable, mode_print_hashtable_t mode);

err_t       PrintList         (list_t list);

#endif
