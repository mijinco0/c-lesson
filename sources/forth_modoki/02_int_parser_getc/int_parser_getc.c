#include "clesson.h"
#include <assert.h>

/*
cc cl_getc.c int_parser_getc.c
*/

enum LexicalType {
    NUMBER,
    SPACE,
};

int parse_one(int prev_ch, enum LexicalType *out_type, int *out_val);
enum LexicalType gettype(int c);
int ctoi(int c);
int isdigit(int c);

// for unit test
void test_parse_one_123();
void test_parse_one_123_456();

int main() {
    int answer1 = 0;
    int answer2 = 0;

    // for unit test
    test_parse_one_123();
    test_parse_one_123_456();

    cl_getc_set_src(" 123  456");

    // write something here.
    int c, v, prev_ch = '\0';
    enum LexicalType t;
    int ans[2], i = 0;

    while (1) {
        c = parse_one(prev_ch, &t, &v);
        if (t == NUMBER) {
            ans[i++] = v;
        }
        if (c == EOF) break;
        prev_ch = c;
    }

    answer1 = ans[0];
    answer2 = ans[1];

    // verity result.
    assert(answer1 == 123);
    assert(answer2 == 456);

    return 0;
}

int parse_one(int prev_ch, enum LexicalType *out_type, int *out_val)
{
    if (prev_ch == '\0') {
        prev_ch = cl_getc();
    }

    int val = 0;
    int c = prev_ch;
    enum LexicalType prevt = gettype(prev_ch);

    do {
        if (gettype(c) != prevt) break;

        switch (prevt) {
        case NUMBER:
            val = (val * 10) + ctoi(c);
            break;
        case SPACE:
            val = ' ';
            break;
        default:
            break;
        }
    } while ((c = cl_getc()) != EOF);

    *out_type = prevt;
    *out_val = val;

    return c;
}

enum LexicalType gettype(int c)
{
    return isdigit(c) ? NUMBER : SPACE;
}

int ctoi(int c)
{
    return isdigit(c) ? c - '0' : -1;
}

int isdigit(int c)
{
    return ((c >= '0') && (c <= '9')) ? 1 : 0;
}

void test_parse_one_123()
{
    cl_getc_set_src("123");

    int prev_ch = '\0';
    enum LexicalType t;
    int ans = 0;

    parse_one(prev_ch, &t, &ans);
    assert(ans == 123);
}

void test_parse_one_123_456()
{
    cl_getc_set_src("123 456");

    int prev_ch = '\0';
    enum LexicalType t;
    int token;

    prev_ch = parse_one(prev_ch, &t, &token);
    assert(prev_ch == ' ');
    assert(t == NUMBER);
    assert(token == 123);

    prev_ch = parse_one(prev_ch, &t, &token);
    assert(prev_ch == '4');
    assert(t == SPACE);
    assert(token == ' ');

    prev_ch = parse_one(prev_ch, &t, &token);
    assert(prev_ch == EOF);
    assert(t == NUMBER);
    assert(token == 456);
}
