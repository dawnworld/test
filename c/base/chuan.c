#include <stdio.h>

static int count = 0;
#define MALLOC_COUNT(A) (++count, malloc(A))

int main()
{
    while(count < 10)
    {

        char * a = MALLOC_COUNT(sizeof(char));
        printf("a addr:0x%x, and count:%d\n", a, count);
    }
    return 0;
}
