#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

struct Stack {
    size_t height;
    size_t size;
    void **array;
};

stack_t *stack_new(size_t size)
{
    if (size > STACK_SIZE) return NULL;
    return NULL;
}

void stack_finalize(stack_t *stack)
{
}

int stack_push(stack_t *stack, void *data)
{
    return 0;
}

void *stack_pop(stack_t *stack)
{
    return NULL;
}

void stack_print_all(stack_t *stack)
{
}
