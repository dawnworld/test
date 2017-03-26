#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SSG_PORT 554
#define BUF_SIZE 2048
#define RTSP_OK "RTSP/1.0 200 OK"
int c_seq = 0;

int rtsp_desc(int sock_fd)
{
    char desc_request[BUF_SIZE] = "DESCRIBE rtsp://172.16.34.131/hp.ts RTSP/1.0\r\nCSeq: 1\r\nUser-Agent: SS5\r\nAccept: application/sdp\r\n\r\n";
    char desc_response[BUF_SIZE];
    write(sock_fd, desc_request, BUF_SIZE);
    write(1, desc_request, BUF_SIZE);
    size_t size = 0;
    while(1){
        size = read(sock_fd, desc_response, BUF_SIZE);

        if(size > 0){
            printf("Response:\n%s\n", desc_response);
            /* printf(strncmp(desc_response, RTSP_OK, sizeof(RTSP_OK))); */
            if(strstr(desc_response, RTSP_OK) != NULL)
                printf("Congratulations to you, rtsp ok!!!!!!\n");
            break;
        }
    }
    return 0;
}

int rtsp_setup(int sock_fd)
{
    char setup_request[BUF_SIZE] = "SETUP rtsp://172.16.34.131/hp.ts/trackID=1 RTSP/1.0\r\nCSeq: 2\r\nUser-Agent: SS5\r\nTransport: RTP/AVP;unicast;client_port=2740-2741";
    char setup_response[BUF_SIZE];
    write(sock_fd, setup_request, BUF_SIZE);
    write(1, setup_request, BUF_SIZE);
    size_t size = 0;
    while(1){
        size = read(sock_fd, setup_response, BUF_SIZE);

        if(size > 0){
            printf("Response:\n%s\n", setup_response);
            /* printf(strncmp(desc_response, RTSP_OK, sizeof(RTSP_OK))); */
            if(strstr(setup_response, RTSP_OK) != NULL)
                printf("Congratulations to you, rtsp ok!!!!!!\n");
            break;
        }
    }
    return 0;
}

int main(int argc, char** argv)
{
    printf("ready to request ssg\n");

    int sockfd;
    struct sockaddr_in ssg_addr;
    int error;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        printf("socket error\n");
        return -1;
    }

    bzero(&ssg_addr, sizeof(ssg_addr));
    ssg_addr.sin_family = AF_INET;
    ssg_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    ssg_addr.sin_port = htons(SSG_PORT);

    inet_pton(AF_INET, "172.16.34.131", &ssg_addr.sin_addr);
    connect(sockfd, (struct sockaddr*) &ssg_addr, sizeof(struct sockaddr));

    rtsp_desc(sockfd);
    close(sockfd);
    return 0;
}
