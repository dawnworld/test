#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2015 qiyl <qiyl@certusnet.com.cn>
#
# Distributed under terms of the CERTUSNET license.

SENDERIP = '172.16.24.164'
SENDERPORT = 1501
MYPORT = 1234
MYGROUP = '224.1.1.1'
MYTTL = 255 # Increase to reach other networks
import time
import struct
import socket
def sender():

    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM,socket.IPPROTO_UDP)
    s.bind((SENDERIP,SENDERPORT))
    # Set Time-to-live (optional)
    ttl_bin = struct.pack('@i', MYTTL)
    s.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, ttl_bin)
    status = s.setsockopt(socket.IPPROTO_IP,
        socket.IP_ADD_MEMBERSHIP,
        socket.inet_aton(MYGROUP) + socket.inet_aton(SENDERIP))
    while True:
        data = 'cisco'
        s.sendto(data + '\0', (MYGROUP, MYPORT))
        time.sleep(10)
if __name__ == "__main__":
    sender()
