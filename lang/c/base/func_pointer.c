#include <stdio.h>

int my_fun(int x);
void err_fun(int y);
typedef int (*FUNC_TYPE)(int);

int main(int argc ,char ** argv)
{
    FUNC_TYPE type;
    type = &my_fun;
    type(10);
    type = &err_fun;
    type(1);
    return 0;
}

int my_fun(int x)
{
    printf(" my func args x: %d\n", x);
    return 0;
}

void err_fun(int y, int x)
{
    printf(" this can not called by func type\n");
}
