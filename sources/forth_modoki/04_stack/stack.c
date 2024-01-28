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

    stack_t *stack = (stack_t *)malloc(sizeof(stack_t));
    if (!stack) return NULL;

    stack->height = 0;
    stack->size = size;

    stack->array = (void **)malloc(sizeof(void *) * size);
    if (!stack->array) {
        free(stack);
        return NULL;
    }

    return stack;
}

void stack_finalize(stack_t *stack)
{
    if (!stack) return;

    if (stack->array) free(stack->array);
    free(stack);
}

int stack_push(stack_t *stack, void *data)
{
    return 0;
}

void *stack_pop(stack_t *stack)
{
    return (void *)"";    // to let test fail
}

void stack_print_all(stack_t *stack)
{
}
