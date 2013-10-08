/**
  介绍下开发环境
  linux 2.6.x内核
  redhat 5
  gcc 4.4
  httpserver.c
 **/
/*-----------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <errno.h>
#include "wrap.h"
#include "httpserver.h"
#define MAX_LINE 512
#define DEF_DIR "/home/tyrael"
static struct sockaddr_in host_addr;
static int hostfd;
static char err404[512] = "HTTP/1.1 404 ERROR\r\nConnectlon:close\r\n";
static char context[8192] = {0};
typedef struct request_line {
    char request_type[10];
    char path[512];
    char http_version[10];
} request_line_t;
char* _strstrip(char* str) {
    int len = strlen(str);
    if (!len)
      return str;
    char* tmp = str + len -1;
    while (isspace(*tmp)) {
        *tmp = 0;
        tmp--;
    }
    while (isspace(*str)) {
        *str = 0;
        str++;
    }
    return str;
}
void read_requestline(const char* src,struct request_line *req_line) {
    char* point = NULL;
    char* spc = NULL;
    char* start = NULL;
    char* end = NULL;
    if (src && req_line) {
        if ((start = spc = strchr(src,' '))) {
            strncpy(req_line->request_type,src,spc - src);
            req_line->request_type[spc - src] = 0;
        }
        if ((end = spc = strrchr(src,' '))) {
            strncpy(req_line->http_version,spc + 1,src + strlen(src) - spc);
        }
        if (start && end) {
            strncpy(req_line->path,start + 1,end - start);
            req_line->path[end - start] = 0;
        }
    } else 
      return ;
}

int start_http(const char* ip, int port) {
    int clientfd,clientlen,status;
    struct sockaddr_in clientaddr;
    char buf[MAX_LINE * 100] = {0}; 
    if (!ip)
      err_exit("IP MUST DON'T NULL");
    if (port <=0 || port >65535)
      err_exit("PORT MUST >0 AND <= 65535");
    Set_addr_port(&host_addr,NULL,port);

    hostfd = Socket(AF_INET,SOCK_STREAM,0);

    int f = 1;
    setsockopt(hostfd,SOL_SOCKET,SO_REUSEADDR,&f,sizeof(f));
    Bind(hostfd,(struct sockaddr *)&host_addr,sizeof(host_addr));
    Listen(hostfd,20);
    char* point = NULL;
    char* tmp_store = buf;
    char request_line[MAX_LINE] = {0};
    while (1) {
        clientlen = sizeof(clientaddr);
        clientfd = Accept(hostfd,(struct sockaddr *)&clientaddr,&clientlen);
        printf("%s\n",inet_ntoa(clientaddr.sin_addr));

        //status = Readline(clientfd,buf,MAX_LINE);
        while (1) {
            status = Read(clientfd,tmp_store,512);
            if (status == -1)
              break;
            printf("%s",buf);
            if (status < 512)
              break;
            tmp_store += status;
            *tmp_store = 0;
        }
        if (strlen(buf) > 0) {
            point = strstr(buf,"\r\n");
            if (point) {
                strncpy(request_line,buf,point - buf);
                request_line[point-buf] = 0;
                struct request_line rl;
                memset(&rl,0,sizeof(request_line));
                read_requestline(request_line,&rl);
                printf("#%s\n",rl.request_type);
                if (!strncmp(rl.request_type,"GET",3) ||!strncmp(rl.request_type,"POST",4) || !strncmp(rl.request_type,"HEAD",4)) {
                    if (strlen(rl.path)) {
                        char* _c = strrchr(rl.path,'.');
                        int len = strlen(rl.path);
                        if (_c) {
                            if (!strncasecmp(_c + 1,"jpg",3)) {
                                strcpy(context,"HTTP/1.1 200 OK\r\nContext-Type: image/jpg\r\n\r\n");
                                printf("#jpg\n");
                            } else {
                                strcpy(context,"HTTP/1.1 200 OK\r\nContext-Type: text/html\r\n\r\n");
                                printf("#html\n");
                            }
                        } else {
                            strcpy(context,"HTTP/1.1 200 OK\r\nContext-Type: text/html\r\n\r\n");
                            printf("#other\n");
                        }
                        int filefd;
                        char fileaddr[256] = {0};
                        strcpy(fileaddr,DEF_DIR);
                        strcat(fileaddr,_strstrip(rl.path));
                        filefd = open(fileaddr,O_RDONLY);
                        printf("#%s#\n",fileaddr);
                        if (filefd == -1) {
                            perror("err");
                            printf("#%s\n",fileaddr);
                            memset(fileaddr,0,strlen(fileaddr));
                            strcpy(fileaddr,DEF_DIR);
                            strcat(fileaddr,"/err.html");
                            filefd = open(fileaddr,O_RDONLY);
                            memset(context,0,8192);
                            strcpy(context,"HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n<html><body>request file not found</body></html>");
                            Write(clientfd,context,strlen(context));
                        } else {

                            char test[512] = {0};
                            int co = 0;
                            char* cont = context;
                            int count_img = 0;
                            while (1) {
                                co = read(filefd,test,512);
                                count_img += co;
                                if (co <=0) {
                                    if (co == -1)
                                      perror("error:");
                                    break;
                                }
                                //strcat(context,test);
                                memcpy(cont,test,512);
                                cont += 512;
                                memset(test,0,512);
                            }
                            int c = Write(clientfd,context,count_img);
                            printf("%d\n",c);
                        }
                    } else {
                        int c = Write(clientfd,err404,strlen(err404));
                        printf("%d\n",c);
                    }
                }
                close(clientfd);
                printf("#%s$\n",rl.path);
                printf("#%s\n",rl.http_version);
            }
        }

    }
}
