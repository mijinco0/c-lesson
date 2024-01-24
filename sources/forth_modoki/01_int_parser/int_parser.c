#include <stdio.h>
#include <assert.h>

static const char* const input = "123 456  1203";

int _split(char **out, const char *in, char sep);
int _memcpy(char *out, char *in, int n);
int _strtoi(char *s) ;

int main() {
    int answer1 = 0;
    int answer2 = 0;
    int answer3 = 0;

    // write something here.
    char buf[3][10];
    char *bp[3];

    bp[0] = buf[0];
    bp[1] = buf[1];
    bp[2] = buf[2];

    int n = _split(bp, input, ' ');
    answer1 = _strtoi(buf[0]);
    answer2 = _strtoi(buf[1]);
    answer3 = _strtoi(buf[2]);

    // verity result.
    assert(answer1 == 123);
    assert(answer2 == 456);
    assert(answer3 == 1203);

    return 0;
}

int _split(char **out, const char *in, char sep)
{
    char buf[16];
    char *bp = buf;
    int n = 0;
    int len;

    while (1) {
        len = bp - buf;
        if (*in >= '0' && *in <= '9') {
            if (len < sizeof(buf)) {
                *bp = *in;
                bp++;
            }
        } else if ((*in == sep) || (*in == '\0')) {
            if (len > 0) {
                *bp = '\0';
                _memcpy(out[n], buf, len + 1);
                n++;
            }
            if (*in == '\0') break;
            bp = buf;
        } else {
            // do nothing
        }

        in++;
    }

    return n;
}

int _memcpy(char *out, char *in, int n)
{
    while (n--) *out++ = *in++;
    return 0;
}

int _strtoi(char *s)
{
    int i = 0;
    int mul10 = 1;
    char *p = s;

    while (*p != '\0') p++;

    while (--p >= s) {
        i += (*p - '0') * mul10;
        mul10 *= 10;
    }

    return i;
}
