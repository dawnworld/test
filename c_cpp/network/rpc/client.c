/*
 * *********************************************************************
 * client.c
 * Create 2014-06-05 16:13 By Author: qiyl <qiyl@certusnet.com.cn>
 * Distributed under terms of the CERTUSNET license.
 * *********************************************************************
 */

#include "square.h"

#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv)
{
    CLIENT *cl;
    square_in in;
    square_out *outp;

    if(argc != 3) {
        printf("usage: clent <hostname> <integer-value>\n");
        return -1;
    }

    cl = clnt_create(argv[1], SQUARE_PROG, SQUARE_VERS, "tcp");

    in.arg1 = atol(argv[2]);
    if((outp = squareproc_1(&in, cl)) == NULL) {
        printf("error:%s\n", clnt_sperror(cl, argv[1]));
        return -2;
    }
    printf("result: %ld\n", outp->res1);
    return 0;
}

