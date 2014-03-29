/*
 * *********************************************************************
 * pcap_utils.h
 * Create 2014-03-28 09:55 By Author: qiyl <qiyl@certusnet.com.cn>
 * Distributed under terms of the CERTUSNET license.
 * *********************************************************************
 */

#ifndef __PCAP_UTILS_H__
#define __PCAP_UTILS_H__

#pragma pack(push, 1)

typedef struct _ip_header {
    short           m_ver;
    short           m_len;
    int             m_id_more;
    int             m_ttl_more;
    int             m_src_ip;
    int             m_dst_ip;
}IpHeader;

typedef struct _packet_frame {
    FILE *          m_fp;
    char *          m_data;
    int             m_data_len;
    int             m_real_len;
    struct timeval  m_time;
    IpHeader        m_ip_header;
}PacketFrame;

int write_to_pcap(PacketFrame * frame);

#pragma pack(pop)
#endif /* !__PCAP_UTILS_H__ */

