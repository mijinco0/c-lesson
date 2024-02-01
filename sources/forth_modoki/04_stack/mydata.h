
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

mydata_t *mydata_new_integer(int i);
mydata_t *mydata_new_string(char *s);
mydata_t *mydata_new(void);
void mydata_delete(mydata_t *d);
char *mydata_tostr(char *out, mydata_t *d, size_t n);
char *mydata_get_type(mydata_t *d);

#endif
