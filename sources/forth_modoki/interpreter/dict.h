#ifndef _DICT_H_
#define _DICT_H_

#include "stkelm.h"

struct Dict;
typedef struct Dict dict_t;

dict_t *dict_new();
dict_t *dict_new_init(int size, int (*hash)(char *, int));
void dict_free(dict_t *d);
void dict_clear(dict_t *d);
void dict_put(dict_t *d, char *key, stkelm_t *elem);
int dict_get(dict_t *d, char *key, stkelm_t *out_elem);
int dict_contains(dict_t *d, char *key);
void dict_print_all(dict_t *d);

/* free d, then set NULL */
#define dict_delete(d) { \
    dict_free(d); \
    d = NULL; \
}

/* for unit test */
void test_dict_is_empty();
void test_dict_elem_one();
void test_dict_elem_two();
void test_dict_not_found();
void test_dict_same_hash();
void test_dict_overwrite();

#endif
