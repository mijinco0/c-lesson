/*
 * main for c-lesson
 */

#include <stdio.h>
#include "clesson.h"
#include "parser.h"
#include "test/unittest.h"

static void parser_print_all();
static void unit_tests();

int main() {
    unit_tests();

    cl_getc_set_src("123 45 add /some { 2 3 add } def");
    parser_print_all();
    return 0;
}

static void parser_print_all() {
    int ch = EOF;
    struct Token *token = parser_alloc_token();

    do {
        ch = parse_one(ch, token);
        if(token->ltype != UNKNOWN) {
            switch(token->ltype) {
                case NUMBER:
                    printf("num: %d\n", token->u.number);
                    break;
                case SPACE:
                    printf("space!\n");
                    break;
                case OPEN_CURLY:
                    printf("Open curly brace '%c'\n", token->u.onechar);
                    break;
                case CLOSE_CURLY:
                    printf("Close curly brace '%c'\n", token->u.onechar);
                    break;
                case EXECUTABLE_NAME:
                    printf("EXECUTABLE_NAME: %s\n", token->u.name);
                    break;
                case LITERAL_NAME:
                    printf("LITERAL_NAME: %s\n", token->u.name);
                    break;

                default:
                    printf("Unknown type %d\n", token->ltype);
                    break;
            }
        }
    }while(ch != EOF);

    parser_free_token(token);
}

static void unit_tests() {
    test_parse_one_empty_should_return_END_OF_FILE();
    test_parse_one_number();
    test_parse_one_number2();
    test_parse_one_executable_name();
    test_parse_one_literal_name();
    test_parse_one_open_curly();
    test_parse_one_close_curly();
    test_parse_one_unknown();
}
