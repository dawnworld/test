#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <unistd.h>

int process_conn_server(int c_socket)
{
    int result = 0;
    char recv_buf[1024 * 1024];
    int length = 0;
    while(1)
    {
        length = recv(c_socket, recv_buf, sizeof(recv_buf), 0);
//        printf("recv length :%d\n", length);
    }
    return result;
}

int main(int argc, char** argv)
{
    int port = 0;
    int s_socket = 0, c_socket = 0;
    int pid = 0;

    if(argc < 2) return -1;
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
    while(1)
    {
        int addr_len = sizeof(struct sockaddr);
        c_socket = accept(s_socket, (struct sockaddr *)&client_in, &addr_len);

        if(c_socket <= 0) continue;

        printf(" accept client\n");

        pid = fork();
        if( pid == 0 )
        {
            close(s_socket);
            process_conn_server(c_socket);
        }
        else
            close(c_socket);

        sleep(1);
    }
}
