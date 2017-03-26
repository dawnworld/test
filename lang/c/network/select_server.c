/*
 * *********************************************************************
 * select_server.c
 * Create 2014-06-06 17:35 By Author: qiyl <qiyl@certusnet.com.cn>
 * Distributed under terms of the CERTUSNET license.
 * *********************************************************************
 */

#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <strings.h>
#include <pthread.h>


static int square_int(int v)
{
    return v * v;
}

static void* handle_thread(void* data)
{
    struct fd_set fds;
    struct timeval timeout = {3,0};
    int max_fd = 0;
    char recv_buf[512] = "";
    char send_buf[512] = "";
    
    int *m_socket = malloc(sizeof(int));
    *m_socket = *(int*)data;
    int s_socket = *m_socket;
    int len;

    while(1)
    {
        FD_ZERO(&fds);
        FD_SET(s_socket, &fds);
        max_fd = s_socket + 1;
        if(select(max_fd, &fds, NULL, NULL, &timeout) <= 0)
            continue;
        if(FD_ISSET(s_socket, &fds)) {
            bzero(recv_buf, sizeof(recv_buf));
            len = recv(s_socket, recv_buf, sizeof(recv_buf), 0);

            if(len <=0 ) break;
            printf("[Server]: has recv bytes:%d, content:%s\n", len, recv_buf);

            int value = atoi(recv_buf);
            int res = square_int(value);
            bzero(send_buf, sizeof(send_buf));
            len = snprintf(send_buf, sizeof(send_buf), "%d", res);
            len = send(s_socket, send_buf, len, 0);
            printf("Has sent result:%d, len:%d\n", res, len);
        }
    }
    free(m_socket); m_socket = NULL;
    return NULL;
}

int main(int argc, char** argv)
{
    int port = 0;
    int s_socket = 0, c_socket = 0;
    pthread_t pid;
    int len;

    if(argc < 2) {
        printf("usage: ./bin port\n");
        return -1;
    }
    port = atoi(argv[1]);

    struct sockaddr_in server_in, client_in;
    int socket_buf = 1024 * 1024;
    if((s_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("create server socket error\n");
        return -1;
    }
    memset(&server_in, 0, sizeof(struct sockaddr_in));
    memset(&client_in, 0, sizeof(struct sockaddr_in));
    server_in.sin_family = AF_INET;
    server_in.sin_addr.s_addr = htonl(INADDR_ANY);
    server_in.sin_port = htons(port);

    setsockopt(s_socket, SOL_SOCKET, SO_RCVBUF, (char *)&socket_buf, sizeof(int));
    if(bind(s_socket, (struct sockaddr *)&server_in, sizeof(struct sockaddr_in)) < 0)
    {
        printf("server bind port: %d error\n", port);
        return -1;
    }
    if(listen(s_socket, 128) < 0)
    {
        printf("listen socket error\n");
        return -1;
    }
    printf("server listen port:%d\n", port);
    while(1) {
        len = sizeof(struct sockaddr);
        c_socket = accept(s_socket, (struct sockaddr*)&client_in, (socklen_t *)&len);
        if(c_socket < 0) continue;
        pthread_create(&pid, NULL, handle_thread, &c_socket);
        printf("[Server]: Accept Client:%d, pid:%d\n", c_socket, (int)pid);
        pthread_detach(pid);
    }
    return 0;
}
