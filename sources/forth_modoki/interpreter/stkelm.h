#ifndef _STKELM_H_
#define _STKELM_H_

#include <stddef.h>

typedef enum StkelmType {
    SE_INTEGER,
    SE_STRING,
    SE_UNKNOWN,
} stkelm_type_t;

typedef struct Stkelm {
    enum StkelmType type;
    void *data;
} stkelm_t;

stkelm_t *stkelm_new_integer(int i);
stkelm_t *stkelm_set_integer(stkelm_t *e, int i);
stkelm_t *stkelm_new_string(char *e);
stkelm_t *stkelm_set_string(stkelm_t *e, char *s);
stkelm_t *stkelm_new(void);
void stkelm_free(stkelm_t *e);
stkelm_t *stkelm_duplicate(stkelm_t *src, stkelm_t *dst);
char *stkelm_tostr(char *out, stkelm_t *e, size_t n);
char *stkelm_get_type(stkelm_t *e);
void stkelm_print(stkelm_t *e, char *prefix, char *suffix);

/* free e, then set NULL */
#define stkelm_delete(e) { \
    stkelm_free(e); \
    e = NULL; \
}

#endif
