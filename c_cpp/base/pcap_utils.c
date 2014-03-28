/*
 * *********************************************************************
 * pcap_utils.c
 * Create 2014-03-28 09:56 By Author: qiyl <qiyl@certusnet.com.cn>
 * Distributed under terms of the CERTUSNET license.
 * *********************************************************************
 */

#include <stdio.h>

#include "pcap_utils.h"
#include "linux_types.h"

#define SWAP_BYTE_SEQ_32(A) (((A & 0xFF000000) >> 24) | ((A & 0x00FF0000) >> 8)| \
                             ((A & 0x0000FF00) << 8) | ((A & 0x000000FF) << 24))

#define SWAP_BYTE_SEQ_16(A) (((A & 0xFF00) >> 8) | ((A & 0x00FF) << 8))

typedef struct _pcap_header {
    __u32   m_magic;
    __u16   m_major_version;
    __u16   m_minor_version;
    __u32   m_zone;
    __u32   m_sig_figs;
    __u32   m_snap_len;
    __u32   m_link_type;
}PcapHeader;

int cpu_endian = -1;

/**
 * check local cpu endian
 * if return 0, big endian
 * if return 1, small endian
 */
static int check_cpu_endian()
{
    if(cpu_endian != -1) return cpu_endian;

    union {
        unsigned long int i;
        unsigned char s[4];
    } c;

    c.i = 0x12345678;
    cpu_endian = (0x12 == c.s[0]);

    return cpu_endian;
}

static int get_big_endian_int(int num)
{
    if(!check_cpu_endian()) return num;

    return SWAP_BYTE_SEQ_32(num);
}

static int get_little_endian_int(int num)
{
    if(!check_cpu_endian()) return num;

    return SWAP_BYTE_SEQ_32(num);
}

static short get_big_endian_short(short num)
{
    if(!check_cpu_endian()) return num;

    return SWAP_BYTE_SEQ_16(num);
}

static short get_little_endian_short(short num)
{
    if(!check_cpu_endian()) return num;

    return SWAP_BYTE_SEQ_16(num);
}

static int send_to_write(FILE *fp, char* data, int data_len)
{
    int res = 0;

    res = fwrite(data, 1, data_len, fp);

    return res;
}

static int write_pcap_header(FILE * fp)
{
    int res = 0;

    PcapHeader header;
    header.m_magic = get_big_endian_int(0xA1B2C3D4);
    header.m_major_version = get_big_endian_short(0x02);
    header.m_minor_version = get_big_endian_short(0x04);
    header.m_zone = 0;
    header.m_sig_figs = 0;
    header.m_snap_len = get_big_endian_int(2048);
    header.m_link_type = get_big_endian_int(1);

    res = send_to_write(fp, (char *)&header, sizeof(PcapHeader));

    return res;
}

static int modify_pcap_len(FILE *fp)
{
    int res = 0;
    return res;
}

int write_to_pcap(FILE * fp, char* data, int data_len)
{
    int res = 0;

    if(NULL == fp) return -1;

    if(0 == ftell(fp)) {
        write_pcap_header(fp);
    }

    return res;
}
