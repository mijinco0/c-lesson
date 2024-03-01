#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stkelm.h"

stkelm_t *stkelm_new_integer(int i)
{
    stkelm_t *e = stkelm_new();
    if (!e) return NULL;

    e->data = (void *)malloc(sizeof(int));
    if (!e->data) {
        free(e);
        return NULL;
    }

    return stkelm_set_integer(e, i);
}

stkelm_t *stkelm_set_integer(stkelm_t *e, int i)
{
    if (!e) return NULL;
    if (!e->data) return NULL;

    e->type = SE_INTEGER;
    *(int *)e->data = i;

    return e;
}

stkelm_t *stkelm_new_string(char *s)
{
    stkelm_t *e = stkelm_new();
    if (!e) return NULL;

    e->data = NULL;
    e = stkelm_set_string(e, s);
    if (!e->data) {
        free(e);
        return NULL;
    }

    return e;
}

stkelm_t *stkelm_set_string(stkelm_t *e, char *s)
{
    if (!e) return NULL;

    e->type = SE_STRING;

    size_t n = strlen(s) + 1;
    if ((!e->data) || (n > (strlen((char *)e->data) + 1))) {
        e->data = (void *)realloc(e->data, sizeof(char) * n);
    }

    strncpy((char *)e->data, s, n);

    return e;
}

stkelm_t *stkelm_new(void)
{
    stkelm_t *e = (stkelm_t *)malloc(sizeof(stkelm_t));
    if (!e) return NULL;

    e->type = SE_UNKNOWN;
    e->data = NULL;

    return e;
}

void stkelm_delete(stkelm_t *e)
{
    if (!e) return;
    if (e->data) {
        free(e->data);
    }
    free(e);
}

stkelm_t *stkelm_duplicate(stkelm_t *src)
{
    if (!src) return NULL;

    stkelm_t *dst = NULL;

    switch (src->type) {
    case SE_INTEGER:
        dst = stkelm_new_integer(*(int *)src->data);
        break;
    case SE_STRING:
        dst = stkelm_new_string((char *)src->data);
        break;
    default:
        break;
    }

    return dst;
}

char *stkelm_tostr(char *out, stkelm_t *e, size_t n)
{
    if (!e) {
        *out = '\0';
        return out;
    }

    switch (e->type) {
    case SE_INTEGER:
        snprintf(out, n, "%d", *(int *)e->data);
        break;
    case SE_STRING:
        snprintf(out, n, "%s", (char *)e->data);
        break;
    default:
        *out = '\0';
        break;
    }

    return out;
}

char *stkelm_get_type(stkelm_t *e)
{
    if (!e) return NULL;

    char *t = "UNKNOWN";

    switch (e->type) {
    case SE_INTEGER:
        t = "INTEGER";
        break;
    case SE_STRING:
        t = "STRING";
        break;
    default:
        break;
    }

    return t;
}

void stkelm_print(stkelm_t *e, char *prefix, char *suffix)
{
    char data[256];
    stkelm_tostr(data, e, 256);

    if (prefix) printf("%s", prefix);
    printf("e = 0x%lx, e->data = 0x%lx *(e->data) = %s", (unsigned long)e, (unsigned long)e->data, data);
    if (suffix) printf("%s", suffix);
}
