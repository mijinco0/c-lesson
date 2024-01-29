#include <stdio.h>
#include "stack.h"
#include "mydata.h"

void print_stack(stack_t *stack)
{
    int h = (int)stack_get_height(stack);
    mydata_t *d;

    printf("\nsize = %d, height = %d\n", (int)stack_get_size(stack), h);

    while (--h >= 0) {
        d = (mydata_t *)stack_get_elem(stack, h);

        printf("[%4d] addr(elem) = 0x%lx, ", h, (unsigned long)d);
        if (!d) {
            printf("NULL\n");
        } else if (d->type == INTEGER) {
            printf("type = INTEGER, data = %d, addr(data) = 0x%lx\n", *(int *)d->data, (unsigned long)d->data);
        } else if (d->type == STRING) {
            printf("type = STRING, data = %s, addr(data) = 0x%lx\n", (char *)d->data, (unsigned long)d->data);
        } else {
            printf("Unknown\n");
        }
    }
}
