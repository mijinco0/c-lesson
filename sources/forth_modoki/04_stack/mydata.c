#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mydata.h"

mydata_t *mydata_new_integer(int i)
{
    mydata_t *d = mydata_new();
    if (!d) return NULL;

    d->type = INTEGER;
    d->data = (void *)malloc(sizeof(int));
    if (!d->data) {
        free(d);
        return NULL;
    }
    *(int *)d->data = i;

    return d;
}

mydata_t *mydata_new_string(char *s)
{
    mydata_t *d = mydata_new();
    if (!d) return NULL;

    size_t n = strlen(s) + 1;
    d->type = STRING;
    d->data = (void *)malloc(sizeof(char) * n);
    if (!d->data) {
        free(d);
        return NULL;
    }
    strncpy((char *)d->data, s, n);

    return d;
}

mydata_t *mydata_new(void)
{
    mydata_t *d = (mydata_t *)malloc(sizeof(mydata_t));
    if (!d) return NULL;

    d->type = UNKNOWN;
    d->data = NULL;

    return d;
}

void mydata_delete(mydata_t *d)
{
    if (!d) return;
    if (d->data) {
        free(d->data);
    }
    free(d);
}

char *mydata_tostr(char *out, mydata_t *d, size_t n)
{
    if (!d) {
        *out = '\0';
        return out;
    }

    switch (d->type) {
    case INTEGER:
        snprintf(out, n, "%d", *(int *)d->data);
        break;
    case STRING:
        snprintf(out, n, "%s", (char *)d->data);
        break;
    default:
        *out = '\0';
        break;
    }

    return out;
}

char *mydata_get_type(mydata_t *d)
{
    if (!d) return NULL;

    char *t = "UNKNOWN";

    switch (d->type) {
    case INTEGER:
        t = "INTEGER";
        break;
    case STRING:
        t = "STRING";
        break;
    default:
        break;
    }

    return t;
}
