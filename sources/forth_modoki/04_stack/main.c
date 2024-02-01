/*
 * main for c-lesson
 */

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "mydata.h"
#include "clesson.h"
#include "unittest.h"

static void unit_tests();
static void stack_print_all();

int main()
{
    unit_tests();
    stack_print_all();
    return 0;
}

static void unit_tests()
{
    test_stack_pop_empty_should_return_NULL();
    test_stack_push_and_pop_integer_1();
    test_stack_push_and_pop_integer_2();
    test_stack_push_and_pop_string_1();
    test_stack_push_and_pop_string_2();
}

#define NDATA 10

static void stack_print_all()
{
    stack_t *stack = stack_new(STACK_SIZE);
    if (!stack) {
        fprintf(stderr, "error: cannot initialize stack.\n");
        return;
    }

    printf("** initial stack **\n");
    print_stack(stack);
    printf("\n");

    mydata_t *pushd[NDATA];
    pushd[0] = mydata_new_string("abc");
    pushd[1] = mydata_new_string("def");
    pushd[2] = mydata_new_string("ghi");
    pushd[3] = mydata_new_integer(12);
    pushd[4] = mydata_new_string("jkl");
    pushd[5] = mydata_new_integer(34);
    pushd[6] = mydata_new_integer(56);
    pushd[7] = mydata_new_integer(78);
    pushd[8] = mydata_new_string("mno");
    pushd[9] = mydata_new_integer(9);

    for (int i = 0; i < NDATA; i++) {
        stack_push(stack, (void *)pushd[i]);
    }

    printf("** pushed stack **\n");
    print_stack(stack);
    printf("\n");

    mydata_t *popd;
    char buf[256];
    char *type;

    while (1) {
        popd = stack_pop(stack);
        if (!popd) break;

        type = mydata_get_type(popd);
        mydata_tostr(buf, popd, 256 - 1);
        printf("pop: %s, %s\n", type, buf);

        mydata_delete(popd);    /* each pushd(s) are also freed */
    }
    printf("\n");

    printf("** poped stack **\n");
    print_stack(stack);
    printf("\n");

    stack_delete(stack);
}
