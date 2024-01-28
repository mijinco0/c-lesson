#include <stdlib.h>
#include "mydata.h"

mydata_t *mydata_alloc(void)
{
    mydata_t *d = (mydata_t *)malloc(sizeof(mydata_t));
    if (!d) return NULL;

    d->type = UNKNOWN;
    d->data = NULL;

    return d;
}

void mydata_free(mydata_t *d)
{
    if (!d) return;
    if (d->data) {
        free(d->data);
    }
    free(d);
}
