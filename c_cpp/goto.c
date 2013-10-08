#include <stdio.h>

int main()
{
    int type = 2;
    switch(type)
    {
        case 2:
            printf(" this is 2\n");
            goto END_MAIN;
            printf(" can you get here ?\n");
            break;
        case 1:
            printf("this is 1\n");
    }
END_MAIN:
    printf(" after switch \n");
    {
        printf(" this is block\n");
    }
    printf(" after block\n");
    return 0;
}
