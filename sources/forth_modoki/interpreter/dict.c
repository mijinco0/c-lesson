#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dict.h"

#define ARRAY_SIZE_DEF 16

struct Node {
    char *key;
    stkelm_t *value;
    struct Node *next;
};

struct Dict {
    int size;               /* must be a multiple of 2 */
    struct Node **array;
    int (*hash)(char *, int);
};

static int hash_def(char *str, int size);
static void delete_list(struct Node *p);
static struct Node *update_or_insert_list(struct Node *p, char *key, stkelm_t *elem);
static struct Node *search_list(struct Node *p, char *key);
static struct Node *Node_new(char *key, stkelm_t *elem);
static struct Node *Node_delete(struct Node *node);

static int streq(char *s1, char *s2) {
    return !strcmp(s1, s2);
}

dict_t *dict_new()
{
    return dict_new_init(0, NULL);
}

dict_t *dict_new_init(int size, int (*hash)(char *, int))
{
    dict_t *d = (dict_t *)malloc(sizeof(dict_t));
    if (!d) return NULL;

    if (size <= 0) size = ARRAY_SIZE_DEF;

    d->array = (struct Node **)malloc(sizeof(struct Node *) * size);
    if (!d->array) {
        free(d);
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        d->array[i] = NULL;
    }

    d->size = size;
    d->hash = hash ? hash : hash_def;

    return d;
}

void dict_delete(dict_t *d)
{
    if (!d) return;

    dict_clear(d);
    if (d->array) free(d->array);
    free(d);
}

void dict_clear(dict_t *d)
{
    if (!d) return;

    for (int i = 0; i < d->size; i++) {
        delete_list(d->array[i]);
        d->array[i] = NULL;
    }
}

void dict_put(dict_t *d, char *key, stkelm_t *elem)
{
    if (!d) return;

    int i = d->hash(key, d->size);
    struct Node *head = d->array[i];

    if (head == NULL) {
        head = Node_new(key, elem);
        d->array[i] = head;
        return;
    }

    update_or_insert_list(head, key, elem);
}

int dict_get(dict_t *d, char *key, stkelm_t *out_elem)
{
    struct Node *p = search_list(d->array[d->hash(key, d->size)], key);
    if (p) {
        out_elem = stkelm_duplicate(p->value);
        return 1;
    }

    return 0;
}

int dict_contains(dict_t *d, char *key)
{
    int h = d->hash(key, d->size);
    return search_list(d->array[h], key) ? h : -1;
}

void dict_print_all(dict_t *d)
{
    struct Node *p;
    char s[32];
    int i;

    for (i = 0; i < d->size; i++) {
        printf("[%.2d]: ", i);
        p = d->array[i];
        while (p) {
            printf("(%s, %s) ", p->key, stkelm_tostr(s, p->value, 32));
            p = p->next;
        }
        printf("\n");
    }
    printf("\n");
}

/* size must be a multiple of 2 */
static int hash_def(char *str, int size)
{
    unsigned int val = 0;
    while (*str) val += *str++;
    return (int)(val & (size - 1));
}

/*
 * delete nodes subsequently connected to p
 */
static void delete_list(struct Node *p)
{
    if (p == NULL) return;

    struct Node *q;

    while (1) {
        q = p->next;
        if (q == NULL) {
            Node_delete(p);
            break;
        }
        p->next = Node_delete(q);
    }
}

/*
 * return pointer to the node updated or inserted
 */
static struct Node *update_or_insert_list(struct Node *p, char *key, stkelm_t *elem)
{
    if (p == NULL) return NULL;

    while (1) {
        if (streq(p->key, key)) {
            stkelm_delete(p->value);
            p->value = stkelm_duplicate(elem);
            return p;
        }
        if (p->next == NULL) break;
        p = p->next;
    }

    p->next = Node_new(key, elem);

    return p->next;
}

static struct Node *search_list(struct Node *p, char *key)
{
    if (p == NULL) return NULL;

    while (1) {
        if (streq(p->key, key)) {
            return p;
        }
        if (p->next == NULL) break;
        p = p->next;
    }

    return NULL;
}

static struct Node *Node_new(char *key, stkelm_t *elem)
{
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    if (!node) return NULL;

    node->key = (char *)malloc(strlen(key) + 1);
    if (!node->key) {
        free(node);
        return NULL;
    }
    strncpy(node->key, key, strlen(key) + 1);

    node->value = stkelm_duplicate(elem);
    if (!node->value) {
        free(node->key);
        free(node);
        return NULL;
    }

    node->next = NULL;

    return node;
}

/*
 * free node->key, node->value and node itself. then return node->next
 */
static struct Node *Node_delete(struct Node *node)
{
    if (!node) return NULL;

    struct Node *next = node->next;

    if (node->key) {
        free(node->key);
    }

    if (node->value) {
        stkelm_delete(node->value);
    }

    free(node);

    return next;
}

/* for unit test */
void test_dict_is_empty()
{
    int input = 123;
    int expect = 0;
    char *key = "hoge";

    dict_t *dict = dict_new();
    dict_print_all(dict);

    stkelm_t *e;
    int actual = dict_get(dict, key, e);

    dict_delete(dict);

    assert(expect == actual);
}

void test_dict_elem_one()
{
    int input = 123;
    int expect = 123;
    char *key = "hoge";

    dict_t *dict = dict_new();

    stkelm_t *e;

    e = stkelm_new_integer(input);
    dict_put(dict, key, e);
    stkelm_delete(e);

    dict_print_all(dict);

    int actual = 0;
    if (dict_get(dict, key, e)) {
        actual = *(int *)e->data;
        stkelm_delete(e);
    }

    dict_delete(dict);

    assert(expect == actual);
}

void test_dict_elem_two()
{
    int input[] = {123, 456};
    int expect[] = {123, 456};
    char *key[] = {"hoge", "fuga"};

    dict_t *dict = dict_new();

    stkelm_t *e;

    e = stkelm_new_integer(input[0]);
    dict_put(dict, key[0], e);

    e = stkelm_set_integer(e, input[1]);
    dict_put(dict, key[1], e);

    stkelm_delete(e);

    dict_print_all(dict);

    int actual[] = {0, 0};

    for (int i = 0; i < 2; i++) {
        if (dict_get(dict, key[i], e)) {
            actual[i] = *(int *)e->data;
            stkelm_delete(e);
        }
    }

    dict_delete(dict);

    assert(expect[0] == actual[0]);
    assert(expect[1] == actual[1]);
}

void test_dict_not_found()
{
    int input[] = {123, 456, 789};
    int expect = 0;
    char *key[] = {"hoge", "fuga", "piyo"};
    char *trykey = "foo";

    dict_t *dict = dict_new();

    stkelm_t *e = stkelm_new_integer(0);
    for (int i = 0; i < 3; i++) {
        e = stkelm_set_integer(e, input[i]);
        dict_put(dict, key[i], e);
    }
    stkelm_delete(e);

    dict_print_all(dict);

    int actual = dict_get(dict, trykey, e);

    dict_delete(dict);

    assert(expect == actual);
}

/* for hash table version */
void test_dict_same_hash()
{
    int input[] = {123, 456, 789, 1011};
    int expect[] = {123, 456, 789, 1011};
    char *key[] = {"abcde", "abyz", "cdwx", "efuv"};    /* last three keys has same hash */
    int i;

    dict_t *dict = dict_new();
    dict_print_all(dict);

    stkelm_t *e = stkelm_new_integer(0);
    for (i = 0; i < 4; i++) {
        e = stkelm_set_integer(e, input[i]);
        dict_put(dict, key[i], e);
    }
    stkelm_delete(e);

    dict_print_all(dict);

    int actual[] = {0, 0, 0, 0};

    /* put elements in random order */
    if (dict_get(dict, key[2], e)) {
        actual[2] = *(int *)e->data;
        stkelm_delete(e);
    }
    if (dict_get(dict, key[0], e)) {
        actual[0] = *(int *)e->data;
        stkelm_delete(e);
    }
    if (dict_get(dict, key[3], e)) {
        actual[3] = *(int *)e->data;
        stkelm_delete(e);
    }
    if (dict_get(dict, key[1], e)) {
        actual[1] = *(int *)e->data;
        stkelm_delete(e);
    }

    dict_delete(dict);

    assert(expect[0] == actual[0]);
    assert(expect[1] == actual[1]);
    assert(expect[2] == actual[2]);
    assert(expect[3] == actual[3]);
}

void test_dict_overwrite()
{
    int input[] = {123, 456, 789, 1011, 1213};
    int expect[] = {123, 456, 1213, 1011};
    char *key[] = {"ab", "abyz", "cdwx", "efuv", "cdwx"};    /* value of "cdwx" should be overwritten */
    int i;

    dict_t *dict = dict_new();

    stkelm_t *e = stkelm_new_integer(0);
    for (i = 0; i < 5; i++) {
        e = stkelm_set_integer(e, input[i]);
        dict_put(dict, key[i], e);
    }
    stkelm_delete(e);

    dict_print_all(dict);

    int actual[] = {0, 0, 0, 0};

    /* put elements in random order */
    if (dict_get(dict, key[2], e)) {
        actual[2] = *(int *)e->data;
        stkelm_delete(e);
    }
    if (dict_get(dict, key[0], e)) {
        actual[0] = *(int *)e->data;
        stkelm_delete(e);
    }
    if (dict_get(dict, key[3], e)) {
        actual[3] = *(int *)e->data;
        stkelm_delete(e);
    }
    if (dict_get(dict, key[1], e)) {
        actual[1] = *(int *)e->data;
        stkelm_delete(e);
    }

    dict_delete(dict);

    assert(expect[0] == actual[0]);
    assert(expect[1] == actual[1]);
    assert(expect[2] == actual[2]);
    assert(expect[3] == actual[3]);
}
