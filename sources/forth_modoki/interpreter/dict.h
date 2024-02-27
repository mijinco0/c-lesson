#ifndef _DICT_H_
#define _DICT_H_

#include "stkelm.h"

void dict_new();
void dict_delete();
void dict_clear();
void dict_put(char *key, stkelm_t *elem);
int dict_get(char *key, stkelm_t *out_elem);
int dict_contains(char *key);
void dict_print_all();

/* for unit test */
void test_dict_elem_one();
void test_dict_elem_two();

#endif
