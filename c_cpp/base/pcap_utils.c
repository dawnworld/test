/*
 * *********************************************************************
 * pcap_utils.c
 * Create 2014-03-28 09:56 By Author: qiyl <qiyl@certusnet.com.cn>
 * Distributed under terms of the CERTUSNET license.
 * *********************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "pcap_utils.h"
#include "linux_types.h"

#define SWAP_BYTE_SEQ_32(A) (((A & 0xFF000000) >> 24) | ((A & 0x00FF0000) >> 8)| \
                             ((A & 0x0000FF00) << 8) | ((A & 0x000000FF) << 24))

#define SWAP_BYTE_SEQ_16(A) (((A & 0xFF00) >> 8) | ((A & 0x00FF) << 8))

#define BIG_INT(A)          get_big_endian_int(A);
#define BIG_SHORT(A)        get_big_endian_short(A);
#define LIT_INT(A)          get_little_endian_int(A);
#define LIT_SHORT(A)        get_little_endian_short(A);

typedef struct _pcap_header {
    __u32   m_magic;
    __u16   m_major_version;
    __u16   m_minor_version;
    __u32   m_zone;
    __u32   m_sig_figs;
    __u32   m_snap_len;
    __u32   m_link_type;
}PcapHeader;

typedef struct _packet_header {
    __u32   m_utc_sec;
    __u32   m_micro_sec;
    __u32   m_real_len;
    __u32   m_data_len;
}PacketHeader;

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
    if(check_cpu_endian()) return num;

    return SWAP_BYTE_SEQ_32(num);
}

static short get_big_endian_short(short num)
{
    if(!check_cpu_endian()) return num;

    return SWAP_BYTE_SEQ_16(num);
}

static short get_little_endian_short(short num)
{
    if(check_cpu_endian()) return num;

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
    header.m_magic = BIG_INT(0xA1B2C3D4);
    header.m_major_version = BIG_SHORT(0x02);
    header.m_minor_version = BIG_SHORT(0x04);
    header.m_zone = 0;
    header.m_sig_figs = 0;
    header.m_snap_len = BIG_INT(2048);
    header.m_link_type = BIG_INT(1);

    res = send_to_write(fp, (char *)&header, sizeof(PcapHeader));

    return res;
}

static int write_packet_header(PacketFrame * frame)
{
    int res = 0;

    PacketHeader header;
    header.m_utc_sec = BIG_INT(frame->m_time.tv_sec);
    header.m_micro_sec = BIG_INT(frame->m_time.tv_usec);
    header.m_real_len = BIG_INT(frame->m_real_len);
    header.m_data_len = BIG_INT(frame->m_data_len);

    res = send_to_write(frame->m_fp, (char *)&header, sizeof(PacketHeader));

    return res;
}

static int write_packet_data(PacketFrame * frame)
{
    int res = 0;
    int data_len;

    char ethernet[] = "\x00\xD0\xD0\xA1\x47\xB9\x4C\xB1\x6C\x91\x11\x61\x08\x00"; 

    IpHeader *pHeader = &frame->m_ip_header;
    data_len = pHeader->m_len;

    pHeader->m_ver = LIT_SHORT(0x45B8);
    pHeader->m_len = LIT_SHORT(pHeader->m_len);
    pHeader->m_id_more = BIG_INT(0x40CA57);
    pHeader->m_ttl_more = BIG_INT(0x46B284FD);
    pHeader->m_src_ip = LIT_INT(pHeader->m_src_ip);
    pHeader->m_dst_ip = LIT_INT(pHeader->m_dst_ip);

    res = send_to_write(frame->m_fp, ethernet, sizeof(ethernet) - 1);
    res = send_to_write(frame->m_fp, (char*)pHeader, sizeof(IpHeader));

    // data_len = frame->m_real_len - sizeof(ethernet) -1 - sizeof(IpHeader);

    res = send_to_write(frame->m_fp, frame->m_data, data_len);

    return res;
}

static int modify_pcap_len(FILE *fp)
{
    int res = 0;
    return res;
}

int write_to_pcap(PacketFrame * frame)
{
    int res = 0;

    if(NULL == frame || NULL == frame->m_fp) return -1;

    if(0 == ftell(frame->m_fp)) {
        write_pcap_header(frame->m_fp);
    }

    res = write_packet_header(frame);
    res = write_packet_data(frame);

    return res;
}
