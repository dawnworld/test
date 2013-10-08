#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

void urldecode2(char *dst, const char *src)
{
        char a, b;
        while (*src) {
                if ((*src == '%') &&
                    ((a = src[1]) && (b = src[2])) &&
                    (isxdigit(a) && isxdigit(b))) {
                        if (a >= 'a')
                                a -= 'A'-'a';
                        if (a >= 'A')
                                a -= ('A' - 10);
                        else
                                a -= '0';
                        if (b >= 'a')
                                b -= 'A'-'a';
                        if (b >= 'A')
                                b -= ('A' - 10);
                        else
                                b -= '0';
                        *dst++ = 16*a+b;
                        src+=3;
                } else {
                        *dst++ = *src++;
                }
        }
        *dst++ = '\0';
}

int main()
{
    char out[256];
    char *in = "%7B%22message%22%3A%7B%22path%22%3A%22task%2Fssi%2Flive%22%2C%22date%22%3A%2220120725%22%2C%22endTime%22%3A%221350529600%22%2C%22taskid%22%3A998724866%2C%22beginTime%22%3A%221350527200%22%2C%22snapshotInterval%22%3A%22-1%22%2C%22ssgaddr%22%3A%22172.16.7.137%22%2C%22ssgport%22%3A554%7D%2C%22eventType%22%3A%7B%22eventCode%22%3A3%2C%22subEvent%22%3A0%7D%7D";
    urldecode2(out, in);
    printf(" out data:%s\n", out);
    return 0;
}
