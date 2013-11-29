#coding=utf-8
_author__ = 'mac'

import socket
import time

HOST = ''
PORT = 9001
BUFSIZE = 1024
ADDR = (HOST,PORT)

tcpSvrSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
tcpSvrSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
tcpSvrSocket.bind(ADDR)
tcpSvrSocket.listen(5)

while True:
    print 'waiting for connection.....'
    tcpCliSocket, addr = tcpSvrSocket.accept()
    print '... connected from:', addr

    while True:
        data = tcpCliSocket.recv(BUFSIZE)
        if not data:
            break
        tcpCliSocket.send('[%s] %s' %(time.ctime(), data))
        print [time.ctime()], ':', data

    time.sleep(1)

tcpCliSocket.close()
tcpSvrSocket.close()