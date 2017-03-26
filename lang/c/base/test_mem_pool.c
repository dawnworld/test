/*
 * test_mem_pool.c
 * Copyright (C) 2014 mac <mac@Dawnworld>
 *
 * Distributed under terms of the MIT license.
 */

#include <stdio.h>
#include <time.h>

#include "mem_pool.h"

#define MAX_TEST 5000 * 10000

extern long use_count;

time_t pool_start_time;
time_t pool_end_time;
time_t sys_start_time;
time_t sys_end_time;

typedef struct s_test_1
{
    int a;
    char b;
    char *obc;
}Test1;

typedef struct s_test_2
{
    char *a1;
    char data[100];
    int b;
}Test2;

typedef struct s_test_3
{
    char *a2;
    int num[40];
}Test3;

Test2* k[MAX_TEST];
Test3* h[MAX_TEST];
int main(int argc, char** argv)
{
    int result = 0;
    int i;
    printf(" Test Memory Pool\n");

    printf("struct size: [1:%ld] [2:%ld] [3:%ld]\n", sizeof(Test1), sizeof(Test2), sizeof(Test3));

    srand(time(NULL));
    pool_start_time = time(NULL);
    for(i = 0; i < MAX_TEST; i++)
    {
        Test1 *p1 = pool_alloc(1, rand() % 126 + 128);
        p1->a = 1;
        pool_free(p1, 0);
//        Test2 *p2 = pool_alloc(1, sizeof(Test2));
//        p2->b = 2;
//        //k[i] = p2;
//        
//        pool_free(p1, sizeof(Test1));
//        pool_free(p2, 0);
//        
//        
//        Test3 *p3 = pool_alloc(1, sizeof(Test3));
//        h[i] = p3;
//        char abc[1] = {'a'};
//        p3->a2 = abc;
//        pool_free(p3, 0);

    }
    pool_end_time = time(NULL);
    
    sys_start_time = time(NULL);
    for(i = 0; i < MAX_TEST; i++)
    {
        Test1 *p1 = calloc(1, rand() % 126 + 128);
        p1->a = 1;
        free(p1);

//        Test1 *p1 = (Test1 *)calloc(1, sizeof(Test1));
//        p1->a = 1;
//        
//        Test2 *p2 = calloc(1, sizeof(Test2));
//        p2->b = 2;
//        //k[i] = p2;
//        
//        free(p1);
//        free(p2);
//        
//        
//        Test3 *p3 = pool_alloc(1, sizeof(Test3));
//        h[i] = p3;
//        char abc[1] = {'a'};
//        p3->a2 = abc;
//        free(p3);
        
    }
    sys_end_time = time(NULL);
    
    printf(" pool use time:%d\n", pool_end_time - pool_start_time);
    printf(" sys use time:%d\n", sys_end_time - sys_start_time);
    
//    for(i = 0; i < MAX_TEST; i++)
//    {
//        Test3 *p3 = h[i];
//        Test2 *p2 = k[i];
//        
//        pool_free(p2, 0);
//        pool_free(p3, 0);
//    }
    
    printf(" use count:%ld\n", use_count);
    destroy_pool();
    printf(" use count:%ld\n", use_count);

    // printf("[1:obc]:%x, [2:a1]:%x, [3:a2]:%x\n", p1->obc, p2->a1, p3->a2); 
    // pool_free(p2, sizeof(Test2));                                          
    // pool_free(p3, sizeof(Test3));                                          


    return result;
}

