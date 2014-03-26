/*
 * *********************************************************************
 * base.c
 * Create 2014-02-25 16:22 By Author: qiyl <qiyl@certusnet.com.cn>
 * Distributed under terms of the CERTUSNET license.
 * *********************************************************************
 */

#include "base.h"
#include <stdio.h>


int parseTest(int i, int j)
{
    printf("call static lib successfully\n");
    printf("input param, i:%d, j:%d\n", i, j);
    return 0;
}

