#ifndef _STACK_H_
#define _STACK_H_

#define STACK_SIZE 1024

struct Stack;
typedef struct Stack stack_t;

stack_t *stack_new(size_t size);
void stack_finalize(stack_t *stack);
int stack_push(stack_t *stack, void *data);
void *stack_pop(stack_t *stack);
void stack_print_all(stack_t *stack);

#endif
