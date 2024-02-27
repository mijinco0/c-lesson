#include <string.h>
#include <assert.h>
#include "clesson.h"
#include "parser.h"
#include "stack.h"
#include "stkelm.h"
#include "dict.h"

static stack_t *sStack;

static int streq(char *s1, char *s2) {
    return !strcmp(s1, s2);
}

void eval() {
    if (!sStack) return;

    int i, ch = EOF;
    stkelm_t *e1, *e2;
    struct Token *token = parser_token_new();

    stack_clear(sStack);

    do {
        ch = parse_one(ch, token);
        if (token->ltype == UNKNOWN) continue;

        switch (token->ltype) {
            char *name;
            case NUMBER:
                e1 = stkelm_new_integer(token->u.number);
                stack_push(sStack, e1);
                break;
            case LITERAL_NAME:
                e1 = stkelm_new_string(token->u.name);
                stack_push(sStack, e1);
                break;
            case EXECUTABLE_NAME:
                name = token->u.name;
                if (streq(name, "add")) {
                    e1 = (stkelm_t *)stack_pop(sStack);
                    e2 = (stkelm_t *)stack_pop(sStack);
                    if (e1 && e2) {
                        i = *(int *)e1->data + *(int *)e2->data;
                        stkelm_delete(e1);
                        stkelm_delete(e2);
                        e1 = stkelm_new_integer(i);
                        stack_push(sStack, e1);
                    }
                } else if (streq(name, "def")) {
                    e1 = (stkelm_t *)stack_pop(sStack);
                    e2 = (stkelm_t *)stack_pop(sStack);
                    if (e1 && e2) {
                        dict_put((char *)e2->data, e1);
                        stkelm_delete(e1);
                        stkelm_delete(e2);
                    }
                } else if (dict_contains(name) >= 0) {
                    if (dict_get(name, e1)) {
                        stack_push(sStack, e1);
                        stkelm_delete(e1);
                    }
                } else {
                    /* do nothing */
                }
                break;
            default:
                break;
        }
    } while (ch != EOF);

    parser_token_delete(token);
}

static void test_eval_num_one() {
    char *input = "123";
    int expect = 123;

    cl_getc_set_src(input);

    eval();

    /* TODO: write code to pop stack top element */
    int actual = 0;

    stkelm_t *e = (stkelm_t *)stack_pop(sStack);
    if (e) {
        actual = *(int *)e->data;
        stkelm_delete(e);
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

    stkelm_t *e;

    e = (stkelm_t *)stack_pop(sStack);
    if (e) {
        actual1 = *(int *)e->data;
        stkelm_delete(e);
    }

    e = (stkelm_t *)stack_pop(sStack);
    if (e) {
        actual2 = *(int *)e->data;
        stkelm_delete(e);
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

    stkelm_t *e = (stkelm_t *)stack_pop(sStack);
    if (e) {
        actual = *(int *)e->data;
        stkelm_delete(e);
    }

    assert(expect == actual);
}

static void test_eval_literal_one() {
    char *input = "/hoge";
    char *expect = "hoge";

    cl_getc_set_src(input);

    eval();

    /* TODO: write code to pop stack top element */
    char actual[NAME_SIZE + 1]; *actual = '\0';

    stkelm_t *e = (stkelm_t *)stack_pop(sStack);
    if (e) {
        strncpy(actual, (char *)e->data, NAME_SIZE);
        stkelm_delete(e);
    }

    assert(streq(expect, actual));
}

static void test_eval_literal_two() {
    char *input = "/hoge /fuga";
    char *expect1 = "fuga";
    char *expect2 = "hoge";

    cl_getc_set_src(input);

    eval();

    /* TODO: write code to pop stack top and second top element */
    char actual1[NAME_SIZE + 1]; *actual1 = '\0';
    char actual2[NAME_SIZE + 1]; *actual2 = '\0';

    stkelm_t *e;

    e = (stkelm_t *)stack_pop(sStack);
    if (e) {
        strncpy(actual1, (char *)e->data, NAME_SIZE);
        stkelm_delete(e);
    }

    e = (stkelm_t *)stack_pop(sStack);
    if (e) {
        strncpy(actual2, (char *)e->data, NAME_SIZE);
        stkelm_delete(e);
    }

    assert(streq(expect1, actual1));
    assert(streq(expect2, actual2));
}

int main() {
    test_dict_is_empty();
    test_dict_elem_one();
    test_dict_elem_two();
    test_dict_not_found();

    sStack = stack_new(STACK_SIZE);

    test_eval_num_one();
    test_eval_num_two();
    test_eval_num_add();
    test_eval_literal_one();
    test_eval_literal_two();

#if 0
    char *input = "1 2 3 add add 4 5 6 7 8 9 add add add add add add";    /* =45 */
    cl_getc_set_src(input);
    eval();
    stkelm_t *e = (stkelm_t *)stack_pop(sStack);
    printf("result: %d\n", *(int *)e->data);
#elif 1
    char *input = "/abc 12 def abc abc add";    /* =24 */
    cl_getc_set_src(input);
    eval();
    stkelm_t *e = (stkelm_t *)stack_pop(sStack);
    printf("result: %d\n", *(int *)e->data);
#endif

    stack_delete(sStack);

    return 0;
}
