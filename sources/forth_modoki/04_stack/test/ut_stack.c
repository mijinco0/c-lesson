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

void test_stack_push_and_pop_integer_1()
{
    int input = 123;
    int expect_height = 1;
    mydata_type_t expect_type = INTEGER;
    int expect_data = input;

    int act_height;
    mydata_type_t act_type = UNKNOWN;
    int act_data = 0;

    stack_t *stack = stack_new(STACK_SIZE);
    if (!stack) {
        fprintf(stderr, "error: cannot initialize stack.\n");
        return;
    }

    mydata_t *pushd = mydata_init_integer(input);
    act_height = stack_push(stack, (void *)pushd);

    mydata_t *popd = stack_pop(stack);
    if (popd) {
        act_type = popd->type;
        act_data = *(int *)popd->data;
    }

    mydata_free(pushd);
    mydata_free(popd);
    stack_finalize(stack);

    assert(act_height == expect_height);
    assert(act_type == expect_type);
    assert(act_data == expect_data);
}
