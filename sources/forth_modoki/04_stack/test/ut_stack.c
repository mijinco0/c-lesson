/*
 * unit test for stack
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "stack.h"
#include "mydata.h"
#include "clesson.h"

#define MAX_STRLEN 10

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

    stack_delete(stack);

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

    mydata_t *pushd = mydata_new_integer(input);
    act_height = stack_push(stack, (void *)pushd);

    mydata_t *popd = stack_pop(stack);
    if (popd) {
        act_type = popd->type;
        act_data = *(int *)popd->data;
    }

    mydata_delete(pushd);    /* popd is also freed */
    stack_delete(stack);

    assert(act_height == expect_height);
    assert(act_type == expect_type);
    assert(act_data == expect_data);
}

void test_stack_push_and_pop_integer_2()
{
    int input[] = {123, 456};
    int expect_height[] = {1, 2};
    mydata_type_t expect_type = INTEGER;
    int expect_data[] = {456, 123};
    int expect_popcnt = 2;

    int act_height[] = {0, 0};
    mydata_type_t act_type[] = {UNKNOWN, UNKNOWN};
    int act_data[] = {0, 0};
    int act_popcnt = 0;

    stack_t *stack = stack_new(STACK_SIZE);
    if (!stack) {
        fprintf(stderr, "error: cannot initialize stack.\n");
        return;
    }

    mydata_t *d;
    int i;

    for (i = 0; i < 2; i++) {
        d = mydata_new_integer(input[i]);
        act_height[i] = stack_push(stack, (void *)d);
    }

    //print_stack(stack);

    while (1) {
        d = stack_pop(stack);
        if (!d) break;

        if (act_popcnt < 2) {
            act_type[act_popcnt] = d->type;
            act_data[act_popcnt] = *(int *)d->data;
        }
        act_popcnt++;

        mydata_delete(d);
    }

    //print_stack(stack);

    stack_delete(stack);

    assert(act_height[0] == expect_height[0]);
    assert(act_type[0] == expect_type);
    assert(act_data[0] == expect_data[0]);

    assert(act_height[1] == expect_height[1]);
    assert(act_type[1] == expect_type);
    assert(act_data[1] == expect_data[1]);

    assert(act_popcnt == expect_popcnt);
}

void test_stack_push_and_pop_string_1()
{
    char *input = "abcde";
    int expect_height = 1;
    mydata_type_t expect_type = STRING;
    char *expect_data = input;

    int act_height;
    mydata_type_t act_type = UNKNOWN;
    char act_data[MAX_STRLEN + 1];

    stack_t *stack = stack_new(STACK_SIZE);
    if (!stack) {
        fprintf(stderr, "error: cannot initialize stack.\n");
        return;
    }

    mydata_t *pushd = mydata_new_string(input);
    act_height = stack_push(stack, (void *)pushd);

    mydata_t *popd = stack_pop(stack);
    if (popd) {
        act_type = popd->type;
        strncpy(act_data, popd->data, MAX_STRLEN);
        act_data[MAX_STRLEN] = '\0';
    }

    mydata_delete(pushd);    /* popd is also freed */
    stack_delete(stack);

    assert(act_height == expect_height);
    assert(act_type == expect_type);
    assert(strcmp(act_data, expect_data) == 0);
}

void test_stack_push_and_pop_string_2()
{
    char *input[] = {"abcde", "fghij"};
    int expect_height[] = {1, 2};
    mydata_type_t expect_type = STRING;
    char* expect_data[] = {"fghij", "abcde"};
    int expect_popcnt = 2;

    int act_height[] = {0, 0};
    mydata_type_t act_type[] = {UNKNOWN, UNKNOWN};
    char act_data[2][MAX_STRLEN + 1]; act_data[0][0] = act_data[1][0] = '\0';
    int act_popcnt = 0;

    stack_t *stack = stack_new(STACK_SIZE);
    if (!stack) {
        fprintf(stderr, "error: cannot initialize stack.\n");
        return;
    }

    mydata_t *d;
    int i;

    for (i = 0; i < 2; i++) {
        d = mydata_new_string(input[i]);
        act_height[i] = stack_push(stack, (void *)d);
    }

    //print_stack(stack);

    while (1) {
        d = stack_pop(stack);
        if (!d) break;

        if (act_popcnt < 2) {
            act_type[act_popcnt] = d->type;
            strncpy(act_data[act_popcnt], d->data, MAX_STRLEN);
            act_data[act_popcnt][MAX_STRLEN] = '\0';
        }
        act_popcnt++;

        mydata_delete(d);
    }

    //print_stack(stack);

    stack_delete(stack);

    assert(act_height[0] == expect_height[0]);
    assert(act_type[0] == expect_type);
    assert(strcmp(act_data[0], expect_data[0]) == 0);

    assert(act_height[1] == expect_height[1]);
    assert(act_type[1] == expect_type);
    assert(strcmp(act_data[1], expect_data[1]) == 0);

    assert(act_popcnt == expect_popcnt);
}
