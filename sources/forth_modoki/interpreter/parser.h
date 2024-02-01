#ifndef _PARSER_H_
#define _PARSER_H_

enum LexicalType {
    NUMBER = 0,
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

struct Token *parser_token_new();
void parser_token_delete(struct Token *t);
int parse_one(int prev_ch, struct Token *out_token);

#endif
