#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "clesson.h"
#include "parser.h"

static int ctoi(int c);
static int is_namechar(int c);
static enum LexicalType gettype(int head_ch);

/* reader functions */
static int r_number(int prev_ch, struct Token *out_token);
static int r_space(int prev_ch, struct Token *out_token);
static int r_exename(int prev_ch, struct Token *out_token);
static int r_litname(int prev_ch, struct Token *out_token);
static int r_opencurly(int prev_ch, struct Token *out_token);
static int r_closecurly(int prev_ch, struct Token *out_token);
static int r_eof(int prev_ch, struct Token *out_token);
static int r_unknown(int prev_ch, struct Token *out_token);

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

struct Token *parser_alloc_token()
{
    struct Token *t = (struct Token *)malloc(sizeof(struct Token));
    if (!t) return NULL;

    //printf("allc: %lx, parser_alloc_token, t\n", (unsigned long)t);

    t->ltype = UNKNOWN;
    t->u.name = NULL;
    t->_name_ptr = NULL;

    return t;
}

void parser_free_token(struct Token *t)
{
    if (!t) return;
    if (t->_name_ptr) {
        //printf("free: %lx, parser_free_token, t->_name_ptr\n", (unsigned long)t->_name_ptr);
        free(t->_name_ptr);
    }
    //printf("free: %lx, parser_free_token, t\n", (unsigned long)t);
    free(t);
}

int parse_one(int prev_ch, struct Token *out_token)
{
    /* for the first call */
    if (prev_ch == EOF) {
        prev_ch = cl_getc();
    }

    return Reader[gettype(prev_ch)](prev_ch, out_token);
}

static int ctoi(int c)
{
    return isdigit(c) ? c - '0' : -1;
}

static int is_namechar(int c)
{
    return (isalpha(c) || isdigit(c)) ? 1 : 0;
}

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

static int r_number(int prev_ch, struct Token *out_token)
{
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

static int r_space(int prev_ch, struct Token *out_token)
{
    int c;

    while (1) {
        c = cl_getc();
        if ((c != ' ') || (c == EOF)) break;
    }

    out_token->ltype = SPACE;
    out_token->u.onechar = ' ';

    return c;
}

static int r_exename(int prev_ch, struct Token *out_token)
{
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

static int r_litname(int prev_ch, struct Token *out_token)
{
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

static int r_opencurly(int prev_ch, struct Token *out_token)
{
    out_token->ltype = OPEN_CURLY;
    out_token->u.onechar = prev_ch;

    return cl_getc();
}

static int r_closecurly(int prev_ch, struct Token *out_token)
{
    out_token->ltype = CLOSE_CURLY;
    out_token->u.onechar = prev_ch;

    return cl_getc();
}

static int r_eof(int prev_ch, struct Token *out_token)
{
    out_token->ltype = END_OF_FILE;
    return EOF;
}

static int r_unknown(int prev_ch, struct Token *out_token)
{
    out_token->ltype = UNKNOWN;
    return EOF;
}
