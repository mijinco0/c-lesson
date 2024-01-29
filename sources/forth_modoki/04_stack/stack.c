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
    if (!stack || !data) return -1;
    if ((stack->height + 1) >= stack->size) return -2;

    stack->array[stack->height++] = data;

    return stack->height;
}

void *stack_pop(stack_t *stack)
{
    if (stack_is_empty(stack)) return NULL;
    return stack->array[--stack->height];
}

int stack_is_empty(stack_t *stack)
{
    return stack->height <= 0;
}

int stack_is_full(stack_t *stack)
{
    return stack->height >= stack->size;
}

/* for debug */
size_t stack_get_size(stack_t *stack)
{
    return stack->size;
}

/* for debug */
size_t stack_get_height(stack_t *stack)
{
    return stack->height;
}

/* for debug */
void *stack_get_elem(stack_t *stack, int i)
{
    if (i >= stack->height) return NULL;
    return stack->array[i];
}
