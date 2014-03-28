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

int write_to_pcap(FILE* fp, char* data, int data_len);

#pragma pack(pop)
#endif /* !__PCAP_UTILS_H__ */

