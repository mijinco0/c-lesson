/*
 * unit test for stack
 */

#include <stdio.h>
#include <assert.h>
#include "stack.h"
#include "mydata.h"

void test_stack_pop_empty_should_return_NULL()
{
    mydata_t *input = NULL;
    mydata_t *expect = NULL;
    mydata_t *act;

    stack_t *stack = stack_new(STACK_SIZE);
    if (!stack) {
        fprintf(stderr, "error: cannot initialize stack.\n");
        return;
    }

    act = stack_pop(stack);

    stack_finalize(stack);

    assert(act == expect);
}
