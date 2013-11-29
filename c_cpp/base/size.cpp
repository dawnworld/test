#include <stdio.h>

typedef struct
{
    int i;
    char a[10];
}Test_Struct;

int main()
{
    Test_Struct my;
    char array[10] = "";
    char * p_array = array;
    printf(" struct size is :%d\n", sizeof(Test_Struct));
    printf(" my var size is :%d\n", sizeof(my));
    printf(" array size is :%d\n", sizeof(array));
    printf(" p_array size is :%d\n", sizeof(p_array));
    return 0;
}
