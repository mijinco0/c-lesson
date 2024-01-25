#include "clesson.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

enum LexicalType {
    NUMBER,
    SPACE,
    EXECUTABLE_NAME,
    LITERAL_NAME,
    OPEN_CURLY,
    CLOSE_CURLY,
    END_OF_FILE,
    UNKNOWN
};



struct Token {
    enum LexicalType ltype;
    union {
        int number;
        char onechar;
        char *name;
    } u;
};

#define NAME_SIZE 256

int ctoi(int c)
{
    return isdigit(c) ? c - '0' : -1;
}

static int r_number(int prev_ch, struct Token *out_token) {
    int c, n = ctoi(prev_ch);

    while (1) {
        c = cl_getc();
        if (!isdigit(c)) break;
        n = (n * 10) + ctoi(c);
    }

    out_token->ltype = NUMBER;
    out_token->u.number = n;

    return c;
}

static int r_space(int prev_ch, struct Token *out_token) {
    int c;

    while (1) {
        c = cl_getc();
        if ((c != ' ') || (c == EOF)) break;
    }

    out_token->ltype = SPACE;
    out_token->u.onechar = ' ';

    return c;
}

static int r_exename(int prev_ch, struct Token *out_token) {
    return 0;
}

static int r_litname(int prev_ch, struct Token *out_token) {
    return 0;
}

static int r_opencurly(int prev_ch, struct Token *out_token) {
    return 0;
}

static int r_closecurly(int prev_ch, struct Token *out_token) {
    return 0;
}

static int r_eof(int prev_ch, struct Token *out_token) {
    out_token->ltype = END_OF_FILE;
    return EOF;
}

static int r_unknown(int prev_ch, struct Token *out_token) {
    return 0;
}

typedef int (*reader_t)(int prev_ch, struct Token *out_token);
static reader_t Reader[] = {
    r_number,
    r_space,
    r_exename,
    r_litname,
    r_opencurly,
    r_closecurly,
    r_eof,
    r_unknown,
};

enum LexicalType gettype(int head_ch)
{
    if (isdigit(head_ch)) return NUMBER;
    if (head_ch == ' ') return SPACE;
    if (isalpha(head_ch)) return EXECUTABLE_NAME;
    if (head_ch == '/') return LITERAL_NAME;
    if (head_ch == '{') return OPEN_CURLY;
    if (head_ch == '}') return CLOSE_CURLY;
    if (head_ch == EOF) return END_OF_FILE;
    return UNKNOWN;
}

int parse_one(int prev_ch, struct Token *out_token) {
    /* for the first call */
    if (prev_ch == EOF) {
        prev_ch = cl_getc();
    }

    return Reader[gettype(prev_ch)](prev_ch, out_token);
}

void parser_print_all() {
    int ch = EOF;
    struct Token token = {
        UNKNOWN,
        {0}
    };

    do {
        ch = parse_one(ch, &token);
        if(token.ltype != UNKNOWN) {
            switch(token.ltype) {
                case NUMBER:
                    printf("num: %d\n", token.u.number);
                    break;
                case SPACE:
                    printf("space!\n");
                    break;
                case OPEN_CURLY:
                    printf("Open curly brace '%c'\n", token.u.onechar);
                    break;
                case CLOSE_CURLY:
                    printf("Close curly brace '%c'\n", token.u.onechar);
                    break;
                case EXECUTABLE_NAME:
                    printf("EXECUTABLE_NAME: %s\n", token.u.name);
                    break;
                case LITERAL_NAME:
                    printf("LITERAL_NAME: %s\n", token.u.name);
                    break;

                default:
                    printf("Unknown type %d\n", token.ltype);
                    break;
            }
        }
    }while(ch != EOF);
}





static void test_parse_one_number() {
    char *input = "123";
    int expect = 123;

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);

    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == NUMBER);
    assert(expect == token.u.number);
}

static void test_parse_one_number2() {
    char *input = "123 456";
    int expect_ch[] = {' ', '4', EOF};
    struct Token expect_token[] = {{NUMBER, {123}}, {SPACE, {' '}}, {NUMBER, {456}}};
    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);

    ch = parse_one(EOF, &token);
    assert(ch == expect_ch[0]);
    assert(token.ltype == expect_token[0].ltype);
    assert(token.u.number == expect_token[0].u.number);

    ch = parse_one(ch, &token);
    assert(ch == expect_ch[1]);
    assert(token.ltype == expect_token[1].ltype);
    assert(token.u.onechar == expect_token[1].u.onechar);

    ch = parse_one(ch, &token);
    assert(ch == expect_ch[2]);
    assert(token.ltype == expect_token[2].ltype);
    assert(token.u.number == expect_token[2].u.number);
}

static void test_parse_one_empty_should_return_END_OF_FILE() {
    char *input = "";
    int expect = END_OF_FILE;

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);
    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == expect);
}

static void test_parse_one_executable_name() {
    char* input = "add";
    int expect_type = EXECUTABLE_NAME;
    char* expect_name = "add";

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);
    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == expect_type);
    assert(strcmp(token.u.name, expect_name) == 0);
}

static void test_parse_one_literal_name() {
    char* input = "/add";
    int expect_type = LITERAL_NAME;
    char* expect_name = "add";

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);
    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == expect_type);
    assert(strcmp(token.u.name, expect_name) == 0);
}

static void test_parse_one_open_curly() {
    char* input = "{";
    int expect_type = OPEN_CURLY;
    char expect_ch = '{';

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);
    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == expect_type);
    assert(token.u.onechar == expect_ch);
}

static void test_parse_one_close_curly() {
    char* input = "}";
    int expect_type = CLOSE_CURLY;
    char expect_ch = '}';

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);
    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == expect_type);
    assert(token.u.onechar == expect_ch);
}

static void unit_tests() {
    test_parse_one_empty_should_return_END_OF_FILE();
    test_parse_one_number();
    test_parse_one_number2();
    test_parse_one_executable_name();
    test_parse_one_literal_name();
    test_parse_one_open_curly();
    test_parse_one_close_curly();
}

int main() {
    unit_tests();

    //cl_getc_set_src("123 45 add /some { 2 3 add } def");
    //parser_print_all();
    return 0;
}
