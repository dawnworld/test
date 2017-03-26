/*
 * *********************************************************************
 * select_clint.c
 * Create 2014-06-07 14:58 By Author: qiyl <qiyl@certusnet.com.cn>
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

static void* recv_thread(void* data)
{

    struct fd_set fds;
    struct timeval timeout = {3,0};
    int max_fd = 0;
    char recv_buf[512] = "";
    
    int *m_socket = malloc(sizeof(int));
    *m_socket = *(int*)data;
    int s_socket = *m_socket;
    int len;

    while(1) {
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
        }
    }

    free(m_socket); m_socket = NULL;
    return NULL;
}

int main(int argc, char** argv)
{
    int port = 0;
    int c_socket = 0;
    struct fd_set fds;
    struct timeval timeout = {3,0};
    int max_fd = 0;
    int num;
    int len;
    char send_buf[512] = "";
    pthread_t pid;

    if(argc < 3) {
        printf("usage: ./bin address port\n");
        return -1;
    }
    port = atoi(argv[2]);

    struct sockaddr_in server_in, client_in;
    int socket_buf = 1024 * 1024;
    if((c_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("create server socket error\n");
        return -1;
    }
    memset(&server_in, 0, sizeof(struct sockaddr_in));
    server_in.sin_family = AF_INET;
    server_in.sin_addr.s_addr = htonl(INADDR_ANY);
    server_in.sin_port = htons(port);

    inet_pton(AF_INET, argv[1], &server_in.sin_addr);
    setsockopt(c_socket, SOL_SOCKET, SO_RCVBUF, (char *)&socket_buf, sizeof(int));
    if(-1 == connect(c_socket, (struct sockaddr *)&server_in, sizeof(struct sockaddr))) {
        printf("connect error\n");
        return -1;
    }

    pthread_create(&pid, NULL, recv_thread, &c_socket);

    while(1) {
        scanf("%d", &num);
        printf("[Client]: You just input number:%d\n", num);
        if(-1 == num) break;
        bzero(send_buf, sizeof(send_buf));
        len = snprintf(send_buf, sizeof(send_buf), "%d", num);
        len = send(c_socket, send_buf, len, 0);
        printf("Client: has sent:%d\n", len);
    }

    close(c_socket);
    return 0;
}


