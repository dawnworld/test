#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2015 qiyl <qiyl@certusnet.com.cn>
#
# Distributed under terms of the CERTUSNET license.

import time
import struct
import socket

SENDERIP = '192.168.247.1'
SENDERPORT = 1501
MYPORT = 1234
MYGROUP = '224.1.1.1'
MYTTL = 1 # Increase to reach other networks
def receiver():
    #create a UDP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
    #allow multiple sockets to use the same PORT number
    sock.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
    #Bind to the port that we know will receive multicast data
    sock.bind(('0.0.0.0',MYPORT))
    #tell the kernel that we are a multicast socket
    sock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, 255)
    #Tell the kernel that we want to add ourselves to a multicast group
    #The address for the multicast group is the third param
    status = sock.setsockopt(socket.IPPROTO_IP,
        socket.IP_ADD_MEMBERSHIP,
        socket.inet_aton(MYGROUP) + socket.inet_aton(SENDERIP));
    sock.setblocking(0)
    while 1:
        try:
            data, addr = sock.recvfrom(1024)
            ts = time.time()
        except socket.error, e:
            pass
        else:
            print "Receive data!"
            print "TIME:" , ts
            print "FROM: ", addr
            print "DATA: ", data

if __name__ == "__main__":
    receiver()
