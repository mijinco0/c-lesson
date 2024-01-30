#include <stdio.h>
#include "stack.h"
#include "mydata.h"

void print_stack(stack_t *stack)
{
    int h = (int)stack_get_height(stack);
    mydata_t *d;
    char buf[256];
    char *type;

    printf("\nsize = %d, height = %d\n", (int)stack_get_size(stack), h);

    while (--h >= 0) {
        d = (mydata_t *)stack_get_elem(stack, h);

        printf("[%4d] addr(elem) = 0x%lx, ", h, (unsigned long)d);
        if (!d) {
            printf("NULL\n");
        } else if (d->type < UNKNOWN) {
            type = (d->type == INTEGER) ? "INTEGER" : "STRING";
            mydata_tostr(buf, d, 256 - 1);
            printf("type = %s, data = %s, addr(data) = 0x%lx\n", type, buf, (unsigned long)d->data);
        } else {
            printf("Unknown\n");
        }
    }
}
