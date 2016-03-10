#!/usr/bin/env python
# encoding: utf-8
import socket
print(" i will create a stream client")
s = socket.socket()
s.bind(("172.16.16.199", 23456))
s.listen(5)
cs, address = s.accept()
print(' get connected from %s' % {address})
cs.send(' bye')
ra = cs.recv(512)
print(ra)
cs.close
