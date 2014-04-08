/*
 * *********************************************************************
 * pcap_main.c
 * Create 2014-03-28 13:06 By Author: qiyl <qiyl@certusnet.com.cn>
 * Distributed under terms of the CERTUSNET license.
 * *********************************************************************
 */

#include <stdio.h>
#include <sys/time.h>

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

    char data[512];

    IpHeader header;
    header.m_len = sizeof(data);
    header.m_src_ip = 0x0AF130D1;
    header.m_dst_ip = 0x0AF12AF2;
    header.m_id_more = 0x40CA57;
    header.m_ttl_more = 0x46B284FD;

    PacketFrame frame;
    frame.m_fp = fp;
    frame.m_data = data;
    frame.m_data_len = sizeof(data) + sizeof(IpHeader) + 14 /* Ethernet */;
    frame.m_real_len = sizeof(data) + sizeof(IpHeader) + 14 /* Ethernet */;
    frame.m_ip_header = header;
    
    gettimeofday(&frame.m_time, NULL);

    write_to_pcap(&frame);

    fclose(fp);

    return res;
}

