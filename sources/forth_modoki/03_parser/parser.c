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
    char *_name_ptr;    /* to implicitly keep name string */
};

#define NAME_SIZE 256

int ctoi(int c)
{
    return isdigit(c) ? c - '0' : -1;
}

int is_namechar(int c) {
    return (isalpha(c) || isdigit(c)) ? 1 : 0;
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
    char *p = out_token->_name_ptr;
    if (!p) {
        p = (char *)malloc(sizeof(char) * NAME_SIZE);
        out_token->_name_ptr = p;
        //printf("allc: %lx, r_exename, p\n", (unsigned long)p);
    }

    int c = prev_ch;

    while (1) {
        *p++ = (char)c;
        c = cl_getc();
        if (!is_namechar(c)) break;
    }
    *p = '\0';

    out_token->ltype = EXECUTABLE_NAME;
    out_token->u.name = out_token->_name_ptr;

    return c;
}

static int r_litname(int prev_ch, struct Token *out_token) {
    char *p = out_token->_name_ptr;
    if (!p) {
        p = (char *)malloc(sizeof(char) * NAME_SIZE);
        out_token->_name_ptr = p;
        //printf("allc: %lx, r_litname, p\n", (unsigned long)p);
    }

    /* check if 2nd char is alpha */
    int c = cl_getc();
    if (!isalpha(c)) {
        out_token->ltype = UNKNOWN;
        return EOF;
    }

    while (1) {
        *p++ = (char)c;
        c = cl_getc();
        if (!is_namechar(c)) break;
    }
    *p = '\0';

    out_token->ltype = LITERAL_NAME;
    out_token->u.name = out_token->_name_ptr;

    return c;
}

static int r_opencurly(int prev_ch, struct Token *out_token) {
    out_token->ltype = OPEN_CURLY;
    out_token->u.onechar = prev_ch;

    return cl_getc();
}

static int r_closecurly(int prev_ch, struct Token *out_token) {
    out_token->ltype = CLOSE_CURLY;
    out_token->u.onechar = prev_ch;

    return cl_getc();
}

static int r_eof(int prev_ch, struct Token *out_token) {
    out_token->ltype = END_OF_FILE;
    return EOF;
}

static int r_unknown(int prev_ch, struct Token *out_token) {
    out_token->ltype = UNKNOWN;
    return EOF;
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

struct Token *parser_alloc_token() {
    struct Token *t = (struct Token *)malloc(sizeof(struct Token));
    if (!t) return NULL;

    //printf("allc: %lx, parser_alloc_token, t\n", (unsigned long)t);

    t->ltype = UNKNOWN;
    t->u.name = NULL;
    t->_name_ptr = NULL;

    return t;
}

void parser_free_token(struct Token *t) {
    if (!t) return;
    if (t->_name_ptr) {
        //printf("free: %lx, parser_free_token, t->_name_ptr\n", (unsigned long)t->_name_ptr);
        free(t->_name_ptr);
    }
    //printf("free: %lx, parser_free_token, t\n", (unsigned long)t);
    free(t);
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





static void test_parse_one_number() {
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

static void test_parse_one_number2() {
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

static void test_parse_one_empty_should_return_END_OF_FILE() {
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

static void test_parse_one_executable_name() {
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

static void test_parse_one_literal_name() {
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

static void test_parse_one_open_curly() {
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

static void test_parse_one_close_curly() {
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

static void test_parse_one_unknown() {
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

int main() {
    unit_tests();

    cl_getc_set_src("123 45 add /some { 2 3 add } def");
    parser_print_all();
    return 0;
}
