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
stkelm_t *stkelm_set_integer(stkelm_t *d, int i);
stkelm_t *stkelm_new_string(char *s);
stkelm_t *stkelm_set_string(stkelm_t *d, char *s);
stkelm_t *stkelm_new(void);
void stkelm_delete(stkelm_t *d);
stkelm_t *stkelm_duplicate(stkelm_t *src);
char *stkelm_tostr(char *out, stkelm_t *d, size_t n);
char *stkelm_get_type(stkelm_t *d);

#endif
