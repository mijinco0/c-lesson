#ifndef _STACK_H_
#define _STACK_H_

#define STACK_SIZE 1024

struct Stack;
typedef struct Stack stack_t;

stack_t *stack_new(size_t size);
void stack_free(stack_t *stack);
int stack_push(stack_t *stack, void *data);
void *stack_pop(stack_t *stack);
int stack_is_empty(stack_t *stack);
int stack_is_full(stack_t *stack);
void stack_clear(stack_t *stack);

/* free stack, then set NULL */
#define stack_delete(stack) { \
    stack_free(stack); \
    stack = NULL; \
}

/* for debug */
size_t stack_get_size(stack_t *stack);
size_t stack_get_height(stack_t *stack);
void *stack_get_elem(stack_t *stack, int i);

#endif
