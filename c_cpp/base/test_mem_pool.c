/*
 * test_mem_pool.c
 * Copyright (C) 2014 mac <mac@Dawnworld>
 *
 * Distributed under terms of the MIT license.
 */

#include <stdio.h>

#include "mem_pool.h"

#define MAX_TEST 1024

extern long use_count;

typedef struct s_test_1
{
    int a;
    char b;
    char *obc;
}Test1;

typedef struct s_test_2
{
    char *a1;
    char data[110];
    int b;
}Test2;

typedef struct s_test_3
{
    char *a2;
    int num[40];
}Test3;

int main(int argc, char** argv)
{
    int result = 0;
    int i;
    printf(" Test Memory Pool\n");

    printf("struct size: [1:%ld] [2:%ld] [3:%ld]\n", sizeof(Test1), sizeof(Test2), sizeof(Test3));

    for(i = 0; i < MAX_TEST; i++)
    {
        Test1 *p1 = pool_alloc(1, sizeof(Test1)); 
        Test2 *p2 = pool_alloc(1, sizeof(Test2));
        Test3 *p3 = pool_alloc(1, sizeof(Test3));
        p1->a = 1;
        p2->b = 2;
        char abc[1] = {'a'};
        p3->a2 = abc;

    } 
    printf(" use count:%ld\n", use_count);
    destroy_pool();
    printf(" use count:%ld\n", use_count);

    // printf("[1:obc]:%x, [2:a1]:%x, [3:a2]:%x\n", p1->obc, p2->a1, p3->a2); 
    // pool_free(p1, sizeof(Test1));                                          
    // pool_free(p2, sizeof(Test2));                                          
    // pool_free(p3, sizeof(Test3));                                          


    return result;
}

