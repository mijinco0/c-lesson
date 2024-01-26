/*
 * unit test for parser
 */

#include <string.h>
#include <assert.h>
#include "parser.h"
#include "clesson.h"

void test_parse_one_number() {
    char *input = "123";
    int expect_ch = EOF;
    int expect_ltype = NUMBER;
    int expect_number = 123;

    int act_ch, act_ltype, act_number;

    struct Token *t = parser_alloc_token();

    cl_getc_set_src(input);
    act_ch = parse_one(EOF, t);
    act_ltype = t->ltype;
    act_number = t->u.number;

    parser_free_token(t);

    assert(act_ch == expect_ch);
    assert(act_ltype == expect_ltype);
    assert(act_number == expect_number);
}

void test_parse_one_number2() {
    char *input = "123 456";
    int expect_ch[3];
    struct Token expect_token[3];

    expect_ch[0] = ' ', expect_token[0].ltype = NUMBER, expect_token[0].u.number = 123;
    expect_ch[1] = '4', expect_token[1].ltype = SPACE,  expect_token[1].u.onechar = ' ';
    expect_ch[2] = EOF, expect_token[2].ltype = NUMBER, expect_token[2].u.number = 456;

    int act_ch[3];
    struct Token act_token[3];
    int ch = EOF;

    struct Token *t = parser_alloc_token();

    cl_getc_set_src(input);

    for (int i = 0; i < 3; i++) {
        ch = parse_one(ch, t);
        act_ch[i] = ch;
        act_token[i] = *t;
    }

    parser_free_token(t);

    assert(act_ch[0] == expect_ch[0]);
    assert(act_token[0].ltype == expect_token[0].ltype);
    assert(act_token[0].u.number == expect_token[0].u.number);

    assert(act_ch[1] == expect_ch[1]);
    assert(act_token[1].ltype == expect_token[1].ltype);
    assert(act_token[1].u.onechar == expect_token[1].u.onechar);

    assert(act_ch[2] == expect_ch[2]);
    assert(act_token[2].ltype == expect_token[2].ltype);
    assert(act_token[2].u.number == expect_token[2].u.number);
}

void test_parse_one_empty_should_return_END_OF_FILE() {
    char *input = "";
    int expect_ch = EOF;
    int expect_ltype = END_OF_FILE;

    int act_ch;
    enum LexicalType act_ltype;

    struct Token *t = parser_alloc_token();

    cl_getc_set_src(input);
    act_ch = parse_one(EOF, t);
    act_ltype = t->ltype;

    parser_free_token(t);

    assert(act_ch == expect_ch);
    assert(act_ltype == expect_ltype);
}

void test_parse_one_executable_name() {
    char* input = "add";
    int expect_ch = EOF;
    int expect_type = EXECUTABLE_NAME;
    char* expect_name = "add";

    int act_ch;
    enum LexicalType act_ltype;
    char act_name[NAME_SIZE];

    struct Token *t = parser_alloc_token();

    cl_getc_set_src(input);
    act_ch = parse_one(EOF, t);
    act_ltype = t->ltype;
    if (t->u.name) {
        strncpy(act_name, t->u.name, strlen(t->u.name) + 1);
    } else {
        *act_name = (char)'\0';
    }

    parser_free_token(t);

    assert(act_ch == expect_ch);
    assert(act_ltype == expect_type);
    assert(strcmp(act_name, expect_name) == 0);
}

void test_parse_one_literal_name() {
    char* input = "/add";
    int expect_ch = EOF;
    int expect_type = LITERAL_NAME;
    char* expect_name = "add";

    int act_ch;
    enum LexicalType act_ltype;
    char act_name[NAME_SIZE];

    struct Token *t = parser_alloc_token();

    cl_getc_set_src(input);
    act_ch = parse_one(EOF, t);
    act_ltype = t->ltype;
    if (t->u.name) {
        strncpy(act_name, t->u.name, strlen(t->u.name) + 1);
    } else {
        *act_name = (char)'\0';
    }

    parser_free_token(t);

    assert(act_ch == expect_ch);
    assert(act_ltype == expect_type);
    assert(strcmp(act_name, expect_name) == 0);
}

void test_parse_one_open_curly() {
    char* input = "{";
    int expect_ch = EOF;
    int expect_ltype = OPEN_CURLY;
    char expect_onechar = '{';

    int act_ch;
    int act_ltype;
    char act_onechar;

    struct Token *t = parser_alloc_token();

    cl_getc_set_src(input);
    act_ch = parse_one(EOF, t);
    act_ltype = t->ltype;
    act_onechar = t->u.onechar;

    parser_free_token(t);

    assert(act_ch == expect_ch);
    assert(act_ltype == expect_ltype);
    assert(act_onechar == expect_onechar);
}

void test_parse_one_close_curly() {
    char* input = "}";
    int expect_ch = EOF;
    int expect_ltype = CLOSE_CURLY;
    char expect_onechar = '}';

    int act_ch;
    int act_ltype;
    char act_onechar;

    struct Token *t = parser_alloc_token();

    cl_getc_set_src(input);
    act_ch = parse_one(EOF, t);
    act_ltype = t->ltype;
    act_onechar = t->u.onechar;

    parser_free_token(t);

    assert(act_ch == expect_ch);
    assert(act_ltype == expect_ltype);
    assert(act_onechar == expect_onechar);
}

void test_parse_one_unknown() {
    char* input = "?";
    char expect_ch = EOF;
    int expect_ltype = UNKNOWN;

    int act_ch;
    int act_ltype;

    struct Token *t = parser_alloc_token();

    cl_getc_set_src(input);
    act_ch = parse_one(EOF, t);
    act_ltype = t->ltype;

    parser_free_token(t);

    assert(act_ch == expect_ch);
    assert(act_ltype == expect_ltype);
}
