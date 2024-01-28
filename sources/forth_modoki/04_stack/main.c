/*
 * main for c-lesson
 */

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

enum DataType {
    NUMBER,
    NAME,
    UNKNOWN,
};

typedef struct StackData {
    int integer;
    char *string;
} stackdata_t;

static stackdata_t *stackdata_alloc(void);
static void stackdata_free(stackdata_t *d);

int main()
{
    stack_t *stack = stack_new(STACK_SIZE);
    if (!stack) {
        return 1;
    }

    char *name = "Yamada Taro";

    stackdata_t *pushd = stackdata_alloc();
    pushd->integer = NAME;
    pushd->string = (char *)malloc(sizeof(char) * (strlen(name) + 1));
    strncpy(pushd->string, name, (strlen(name) + 1));

    stack_push(stack, (void *)pushd);
    stackdata_t *popd = stack_pop(stack);

    stackdata_free(pushd);
    stackdata_free(popd);

    return 0;
}

static stackdata_t *stackdata_alloc(void)
{
    stackdata_t *d = (stackdata_t *)malloc(sizeof(stackdata_t));
    if (!d) return NULL;

    d->integer = UNKNOWN;
    d->string = NULL;

    return d;
}

static void stackdata_free(stackdata_t *d)
{
    if (!d) return;
    if (d->string) {
        free(d->string);
    }
    free(d);
}
