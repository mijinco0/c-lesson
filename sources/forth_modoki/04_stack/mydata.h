
#ifndef _MYDATA_H_
#define _MYDATA_H_

typedef enum MyDataType {
    INTEGER,
    STRING,
    UNKNOWN,
} mydata_type_t;

typedef struct MyData {
    enum MyDataType type;
    void *data;
} mydata_t;

mydata_t *mydata_init_integer(int i);
mydata_t *mydata_init_string(char *s);
mydata_t *mydata_alloc(void);
void mydata_free(mydata_t *d);
char *mydata_tostr(char *out, mydata_t *d, size_t n);

#endif
