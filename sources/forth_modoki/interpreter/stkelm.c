#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stkelm.h"

stkelm_t *stkelm_new_integer(int i)
{
    stkelm_t *d = stkelm_new();
    if (!d) return NULL;

    d->type = SE_INTEGER;
    d->data = (void *)malloc(sizeof(int));
    if (!d->data) {
        free(d);
        return NULL;
    }
    *(int *)d->data = i;

    return d;
}

stkelm_t *stkelm_new_string(char *s)
{
    stkelm_t *d = stkelm_new();
    if (!d) return NULL;

    size_t n = strlen(s) + 1;
    d->type = SE_STRING;
    d->data = (void *)malloc(sizeof(char) * n);
    if (!d->data) {
        free(d);
        return NULL;
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
