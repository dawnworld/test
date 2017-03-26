#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2015 qiyl <qiyl@certusnet.com.cn>
#
# Distributed under terms of the CERTUSNET license.

"""

"""
import sys
import time
import socket
import binascii
import struct
import ctypes

class RawIP:
    DEFAULT_IP_LEN = 20

    def __init__(self):
        self.struct     = None
        self.buf        = None

        # proto field
        self.src_ip     = None
        self.dst_ip     = None
        self.check_sum  = None
        self.proto      = None
        self.ttl        = None
        self.total_len  = None

    def __do_checksum(self):
        self.check_sum = 0
        print 'do checksum'

    def __values(self):
        i_src_ip = socket.inet_aton(self.src_ip)
        i_dst_ip = socket.inet_aton(self.dst_ip)
        return (i_src_ip, i_dst_ip)

    def set_data_len(self, data_len):
        self.total_len = self.DEFAULT_IP_LEN + data_len;
        self.buf = bytearray(self.total_len)
        self.buf[:] = [0x45, 0x00]
        print binascii.hexlify(self.buf)

    def packed(self):
        self.__values()
        self.struct = struct.Struct('B B')

class RawPacket:
    def __init__(self):
        self.dst_mac        = None 
        self.src_mac        = None 
        self.interface      = None 
        self.sock           = None
        self.packet_data    = None

    def set_interface(self, interface):
        self.interface = interface

    def set_dst_mac(self, dst_mac):
        self.dst_mac = dst_mac

    def set_src_mac(self, src_mac):
        self.src_mac = src_mac

    def set_packet_data(self, payload):
        self.packet_data = payload

    def prepare(self):
        self.sock = socket.socket(socket.AF_PACKET, socket.SOCK_RAW)
        self.sock.bind(self.interface, 0)
        return True

    def send(self):
        self.sock.send('')

def sendeth(ethernet_packet, payload, interface = "eth0"):
  """Send raw Ethernet packet on interface."""
  s = socket.socket(socket.AF_PACKET, socket.SOCK_RAW)

  # From the docs: "For raw packet
  # sockets the address is a tuple (ifname, proto [,pkttype [,hatype]])"
  s.bind((interface, 0))
  return s.send(ethernet_packet + payload)

def pack(byte_sequence):
  """Convert list of bytes to byte string."""
  return b"".join(map(chr, byte_sequence))

def main():
    print '--------------- Main -----------------'
    ethernet_packet = [0x52, 0x54, 0x00, 0x12, 0x35, 0x02, 0xfe, 0xed, 0xfa,
                       0xce, 0xbe, 0xef, 0x08, 0x00]

    # src=10.0.2.15, dst=195.88.54.16 (vg.no), checksum, etc.
    ipv4_header = [0x45, 0x00, 0x00, 0x54, 0x05, 0x9f, 0x40, 0x00, 0x40, 0x01,
                   0x2f, 0x93, 0x0a, 0x00, 0x02, 0x0f, 0xc3, 0x58, 0x36, 0x10]

    # echo (ping) request, checksum 2b45, etc
    icmp_ping = [0x08, 0x00, 0x2b, 0x45, 0x11, 0x22, 0x00, 0x02, 0xa9, 0xf4, 0x5c,
                 0x53, 0x00, 0x00, 0x00, 0x00, 0xf5, 0x7b, 0x01, 0x00, 0x00, 0x00,
                 0x00, 0x00, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
                 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23,
                 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e,
                 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37]

    payload = b"".join(map(chr, ipv4_header + icmp_ping))
    print 'Packed Value   :', binascii.hexlify(payload)
    # Construct Ethernet packet with an IPv4 ICMP PING request as payload
    # r = sendeth(pack(ethernet_packet), pack(ipv4_header + icmp_ping))
    # print("Sent Ethernet w/IPv4 ICMP PING payload of length %d bytes" % r)

def test():
    print '--------------- Test -----------------'

    raw_ip = RawIP()
    raw_ip.src_ip = '172.16.16.143' 
    raw_ip.dst_ip = '235.0.0.1'
    raw_ip.check_sum = 0
    raw_ip.proto = 3

    raw_ip.set_data_len(9)
    #values = raw_ip.values()
    # print values
    # s = struct.Struct('B B')
    # packed_data = s.pack(*values)
    # print binascii.hexlify(packed_data)
    raw_ip.packed()

if __name__ == "__main__":
    # main()
    test()
