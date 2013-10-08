#include <stdio.h>

typedef struct _test_struct
{
    int rab_size;
    char rab_id[16];
} test_struct;
int main()
{
    test_struct * p_struct;
    p_struct = (test_struct *)malloc(sizeof(test_struct));
    printf(" rab_id size is:%d\n", sizeof(p_struct->rab_id));
    return 0;
}
