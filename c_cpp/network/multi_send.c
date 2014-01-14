/*
 * multi_send.c
 * Copyright (C) 2014 mac <mac@Dawnworld>
 *
 * Distributed under terms of the MIT license.
 */


#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>

#define MULTI_ADDR "224.1.1.20"
#define MULTI_PORT 5998
#define MAX_BUF    1024
#define LOCAL_IP   "127.0.0.1"

int main()
{
    int result = 0;
    struct sockaddr_in dst_multi;
    struct in_addr local_addr;
    int snd_socket;
    char buffer[MAX_BUF];
    int max_buf;

    printf(" ready to send data to multi address %s:%d\n", MULTI_ADDR, MULTI_PORT);

    snd_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    inet_aton(LOCAL_IP, &local_addr); 
    setsockopt(snd_socket, IPPROTO_IP,IP_MULTICAST_IF, &local_addr, sizeof(struct in_addr));

    max_buf = MAX_BUF;
    setsockopt(snd_socket, SOL_SOCKET, SO_SNDBUF, (char *)&max_buf, sizeof(max_buf));
    
    dst_multi.sin_family = AF_INET;
    dst_multi.sin_port = htons(MULTI_PORT);

    inet_aton(MULTI_ADDR, &dst_multi.sin_addr);

    int i;
    for(i = 0; i < MAX_BUF; i++)
        buffer[i] = 'a';

    while(1)
    {
        result = sendto(snd_socket, buffer, sizeof(buffer), 0, 
                       (struct sockaddr*)&dst_multi, sizeof(struct sockaddr_in));
        /* printf(" has sent :%d\n", result); */
        sleep(1);
        if(result > 0) continue;

        printf(" sendto error:%d \n", result);
        break;
    }


    return result;
}

