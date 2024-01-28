/*
 * main for c-lesson
 */

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "mydata.h"
#include "unittest.h"

static void unit_tests();

int main()
{
    unit_tests();
    return 0;
}

static void unit_tests()
{
    test_stack_pop_empty_should_return_NULL();
}
