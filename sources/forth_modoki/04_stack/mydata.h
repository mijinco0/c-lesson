#ifndef _MYDATA_H_
#define _MYDATA_H_

enum MyDataType {
    INTEGER,
    STRING,
    UNKNOWN,
};

typedef struct MyData {
    enum MyDataType type;
    void *data;
} mydata_t;

mydata_t *mydata_alloc(void);
void mydata_free(mydata_t *d);

#endif
