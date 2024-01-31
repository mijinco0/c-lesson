#include <assert.h>
#include "clesson.h"
#include "stack.h"

static stack_t *sStack;

void eval() {}

static void test_eval_num_one() {
    char *input = "123";
    int expect = 123;

    cl_getc_set_src(input);
    stack_clear(sStack);

    eval();

    /* TODO: write code to pop stack top element */
    int actual = 0;

    int *d = (int *)stack_pop(sStack);
    if (d) {
        actual = *d;
    }

    assert(expect == actual);

}

static void test_eval_num_two() {
    char *input = "123 456";
    int expect1 = 456;
    int expect2 = 123;

    cl_getc_set_src(input);
    stack_clear(sStack);

    eval();

    /* TODO: write code to pop stack top and second top element */
    int actual1 = 0;
    int actual2 = 0;

    int *d;

    d = (int *)stack_pop(sStack);
    if (d) {
        actual1 = *d;
    }

    d = (int *)stack_pop(sStack);
    if (d) {
        actual2 = *d;
    }

    assert(expect1 == actual1);
    assert(expect2 == actual2);
}


static void test_eval_num_add() {
    char *input = "1 2 add";
    int expect = 3;

    cl_getc_set_src(input);
    stack_clear(sStack);

    eval();

    /* TODO: write code to pop stack top element */
    int actual = 0;

    int *d = (int *)stack_pop(sStack);
    if (d) {
        actual = *d;
    }

    assert(expect == actual);
}


int main() {
    sStack = stack_new(STACK_SIZE);

    test_eval_num_one();
    test_eval_num_two();
    test_eval_num_add();

    stack_finalize(sStack);

    return 0;
}
