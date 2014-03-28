/*
 * *********************************************************************
 * pcap_main.c
 * Create 2014-03-28 13:06 By Author: qiyl <qiyl@certusnet.com.cn>
 * Distributed under terms of the CERTUSNET license.
 * *********************************************************************
 */

#include <stdio.h>

#include "pcap_utils.h"


int main(int argc, char** argv)
{
    int res = 0;

    FILE * fp = NULL;

    fp = fopen("a.pcap", "wb+");

    if(NULL == fp) {
        printf(" Open file a.pcap error\n");
        return -1;
    }

    write_to_pcap(fp, NULL, 0);

    fclose(fp);

    return res;
}

