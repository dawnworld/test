#!/usr/bin/env python
# encoding: utf-8
import socket
print(' i am a normal client')
c = socket.socket()
c.connect(('172.16.16.199', 23456))
data = c.recv(512);
print(data);
c.send('hi, server')
c.close()

