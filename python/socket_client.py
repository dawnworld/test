#coding=utf-8
__author__ = 'mac'

import socket

HOST = '10.211.55.3'
PORT = 9001
BUFSIZE = 1024
ADDR = (HOST, PORT)

tcpCliSock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
tcpCliSock.connect(ADDR)

while True:
    data = raw_input('> ')
    if not data:
        break
    tcpCliSock.send(data)
    data = tcpCliSock.recv(BUFSIZE)
    if not data:
        break
    print data

tcpCliSock.close()

'''
import time
time.sleep(2)
sock.send('1')
print sock.recv(1024)
sock.close()
'''