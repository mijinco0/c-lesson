#include <string.h>
#include <assert.h>
#include "clesson.h"
#include "parser.h"
#include "stack.h"

static stack_t *sStack;

static int streq(char *s1, char *s2) {
    return !strcmp(s1, s2);
}

void eval() {
    if (!sStack) return;

    int height, ch = EOF;
    int buf[STACK_SIZE];
    struct Token *token = parser_alloc_token();

    stack_clear(sStack);

    do {
        ch = parse_one(ch, token);
        if (token->ltype == UNKNOWN) continue;

        switch (token->ltype) {
            case NUMBER:
                height = stack_get_height(sStack);
                buf[height] = token->u.number;
                stack_push(sStack, &buf[height]);
                break;
            case EXECUTABLE_NAME:
                if (streq(token->u.name, "add")) {
                    int a = *(int *)stack_pop(sStack) + *(int *)stack_pop(sStack);
                    height = stack_get_height(sStack);
                    buf[height] = a;
                    stack_push(sStack, &buf[height]);
                }
                break;
            default:
                break;
        }
    } while (ch != EOF);

    parser_free_token(token);
}

static void test_eval_num_one() {
    char *input = "123";
    int expect = 123;

    cl_getc_set_src(input);

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

    char *input = "1 2 3 add add 4 5 6 7 8 9 add add add add add add";    /* =45 */
    cl_getc_set_src(input);
    eval();
    printf("result: %d\n", *(int *)stack_pop(sStack));

    stack_finalize(sStack);

    return 0;
}
