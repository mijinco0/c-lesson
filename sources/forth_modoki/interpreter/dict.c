#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dict.h"

struct KeyValue {
    char *key;
    stkelm_t *value;
};

static int dict_pos = 0;
static struct KeyValue dict_array[1024];

static struct KeyValue KeyValue_make(char *key, stkelm_t *elem);
static void KeyValue_release(struct KeyValue *kv);

static int streq(char *s1, char *s2) {
    return !strcmp(s1, s2);
}

void dict_new()
{
    dict_pos = 0;
}

void dict_delete()
{
    dict_clear();
}

void dict_clear()
{
    while (--dict_pos >= 0) {
        KeyValue_release(&dict_array[dict_pos]);
    }
    dict_pos = 0;
}

void dict_put(char *key, stkelm_t *elem)
{
    int i = dict_contains(key);
    if (i >= 0) {
        stkelm_delete(dict_array[i].value);
        dict_array[i].value = stkelm_duplicate(elem);
    } else {
        dict_array[dict_pos++] = KeyValue_make(key, elem);
    }
}

int dict_get(char *key, stkelm_t *out_elem)
{
    int i = dict_contains(key);
    if (i >= 0) {
        out_elem = stkelm_duplicate(dict_array[i].value);
        return 1;
    }

    return 0;
}

int dict_contains(char *key)
{
    int i;
    for (i = 0; i < dict_pos; i++) {
        if (streq(key, dict_array[i].key)) {
            return i;
        }
    }

    return -1;
}

void dict_print_all()
{
    stkelm_t *e;
    char s[32];
    int i;

    for (i = 0; i < dict_pos; i++) {
        e = dict_array[i].value;
        if (!e) continue;

        printf("[%.4d] %s %s", i, dict_array[i].key, stkelm_tostr(s, e, 32));
        printf("\n");
    }
}

static struct KeyValue KeyValue_make(char *key, stkelm_t *elem)
{
    struct KeyValue kv = {NULL, NULL};

    kv.key = (char *)malloc(strlen(key) + 1);
    if (kv.key) {
        strncpy(kv.key, key, strlen(key) + 1);
    }

    kv.value = stkelm_duplicate(elem);

    return kv;
}

static void KeyValue_release(struct KeyValue *kv)
{
    if (kv->key) {
        free(kv->key);
        kv->key = NULL;
    }

    if (kv->value) {
        stkelm_delete(kv->value);
        kv->value = NULL;
    }
}

/* for unit test */
void test_dict_is_empty()
{
    int input = 123;
    int expect = 0;
    char *key = "hoge";

    dict_new();
    dict_print_all();

    stkelm_t *e;
    int actual = dict_get(key, e);

    dict_delete();

    assert(expect == actual);
}

void test_dict_elem_one()
{
    int input = 123;
    int expect = 123;
    char *key = "hoge";

    dict_new();

    stkelm_t *e;

    e = stkelm_new_integer(input);
    dict_put(key, e);
    stkelm_delete(e);

    dict_print_all();

    int actual = 0;
    if (dict_get(key, e)) {
        actual = *(int *)e->data;
        stkelm_delete(e);
    }

    dict_delete();

    assert(expect == actual);
}

void test_dict_elem_two()
{
    int input[] = {123, 456};
    int expect[] = {123, 456};
    char *key[] = {"hoge", "fuga"};

    dict_new();

    stkelm_t *e;

    e = stkelm_new_integer(input[0]);
    dict_put(key[0], e);

    e = stkelm_set_integer(e, input[1]);
    dict_put(key[1], e);

    stkelm_delete(e);

    dict_print_all();

    int actual[] = {0, 0};

    for (int i = 0; i < 2; i++) {
        if (dict_get(key[i], e)) {
            actual[i] = *(int *)e->data;
            stkelm_delete(e);
        }
    }

    dict_delete();

    assert(expect[0] == actual[0]);
    assert(expect[1] == actual[1]);
}

void test_dict_not_found()
{
    int input[] = {123, 456, 789};
    int expect = 0;
    char *key[] = {"hoge", "fuga", "piyo"};
    char *trykey = "foo";

    dict_new();

    stkelm_t *e = stkelm_new_integer(0);
    for (int i = 0; i < 3; i++) {
        e = stkelm_set_integer(e, input[i]);
        dict_put(key[i], e);
    }
    stkelm_delete(e);

    dict_print_all();

    int actual = dict_get(trykey, e);

    dict_delete();

    assert(expect == actual);
}

/* for hash table version */
void test_dict_same_hash()
{
    int input[] = {123, 456, 789, 1011};
    int expect[] = {123, 456, 789, 1011};
    char *key[] = {"abc", "abyz", "cdwx", "efuv"};    /* last three keys has same hash */
    int i;

    dict_new();
    dict_print_all();

    stkelm_t *e = stkelm_new_integer(0);
    for (i = 0; i < 4; i++) {
        e = stkelm_set_integer(e, input[i]);
        dict_put(key[i], e);
    }
    stkelm_delete(e);

    dict_print_all();

    int actual[] = {0, 0, 0, 0};

    /* put elements in random order */
    if (dict_get(key[2], e)) {
        actual[2] = *(int *)e->data;
        stkelm_delete(e);
    }
    if (dict_get(key[0], e)) {
        actual[0] = *(int *)e->data;
        stkelm_delete(e);
    }
    if (dict_get(key[3], e)) {
        actual[3] = *(int *)e->data;
        stkelm_delete(e);
    }
    if (dict_get(key[1], e)) {
        actual[1] = *(int *)e->data;
        stkelm_delete(e);
    }

    dict_delete();

    assert(expect[0] == actual[0]);
    assert(expect[1] == actual[1]);
    assert(expect[2] == actual[2]);
    assert(expect[3] == actual[3]);
}

void test_dict_overwrite()
{
    int input[] = {123, 456, 789, 1011, 1213};
    int expect[] = {123, 456, 1213, 1011};
    char *key[] = {"abc", "abyz", "cdwx", "efuv", "cdwx"};    /* value of "cdwx" should be overwritten */
    int i;

    dict_new();

    stkelm_t *e = stkelm_new_integer(0);
    for (i = 0; i < 5; i++) {
        e = stkelm_set_integer(e, input[i]);
        dict_put(key[i], e);
    }
    stkelm_delete(e);

    dict_print_all();

    int actual[] = {0, 0, 0, 0};

    /* put elements in random order */
    if (dict_get(key[2], e)) {
        actual[2] = *(int *)e->data;
        stkelm_delete(e);
    }
    if (dict_get(key[0], e)) {
        actual[0] = *(int *)e->data;
        stkelm_delete(e);
    }
    if (dict_get(key[3], e)) {
        actual[3] = *(int *)e->data;
        stkelm_delete(e);
    }
    if (dict_get(key[1], e)) {
        actual[1] = *(int *)e->data;
        stkelm_delete(e);
    }

    dict_delete();

    assert(expect[0] == actual[0]);
    assert(expect[1] == actual[1]);
    assert(expect[2] == actual[2]);
    assert(expect[3] == actual[3]);
}
