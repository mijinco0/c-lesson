#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stkelm.h"

stkelm_t *stkelm_new_integer(int i)
{
    stkelm_t *d = stkelm_new();
    if (!d) return NULL;

    d->data = (void *)malloc(sizeof(int));
    if (!d->data) {
        free(d);
        return NULL;
    }

    return stkelm_set_integer(d, i);
}

stkelm_t *stkelm_set_integer(stkelm_t *d, int i)
{
    if (!d) return NULL;
    if (!d->data) return NULL;

    d->type = SE_INTEGER;
    *(int *)d->data = i;

    return d;
}

stkelm_t *stkelm_new_string(char *s)
{
    stkelm_t *d = stkelm_new();
    if (!d) return NULL;

    d->data = NULL;
    d = stkelm_set_string(d, s);
    if (!d->data) {
        free(d);
        return NULL;
    }

    return d;
}

stkelm_t *stkelm_set_string(stkelm_t *d, char *s)
{
    if (!d) return NULL;

    d->type = SE_STRING;

    size_t n = strlen(s) + 1;
    if ((!d->data) || (n > (strlen((char *)d->data) + 1))) {
        d->data = (void *)realloc(d->data, sizeof(char) * n);
    }

    strncpy((char *)d->data, s, n);

    return d;
}

stkelm_t *stkelm_new(void)
{
    stkelm_t *d = (stkelm_t *)malloc(sizeof(stkelm_t));
    if (!d) return NULL;

    d->type = SE_UNKNOWN;
    d->data = NULL;

    return d;
}

void stkelm_delete(stkelm_t *d)
{
    if (!d) return;
    if (d->data) {
        free(d->data);
    }
    free(d);
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

char *stkelm_tostr(char *out, stkelm_t *d, size_t n)
{
    if (!d) {
        *out = '\0';
        return out;
    }

    switch (d->type) {
    case SE_INTEGER:
        snprintf(out, n, "%d", *(int *)d->data);
        break;
    case SE_STRING:
        snprintf(out, n, "%s", (char *)d->data);
        break;
    default:
        *out = '\0';
        break;
    }

    return out;
}

char *stkelm_get_type(stkelm_t *d)
{
    if (!d) return NULL;

    char *t = "UNKNOWN";

    switch (d->type) {
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
