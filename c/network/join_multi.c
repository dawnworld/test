#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>

#define MAXBUF 256

void help(char* bin_name)
{
    printf("Help: %s Interface Group Port\n", bin_name);
}

int main(int argc, char** argv)
{
	int fd, n, r;
    unsigned short recv_port = 0;
	struct sockaddr_in srv, cli;
	struct ip_mreq mreq;
    struct ifreq ifr;

	char buf[MAXBUF]    = {0};
    char group_addr[16] = {0};
    char if_addr[16]   = {0};

    if(argc < 4)
    {
        help(argv[0]);
        return 0;
    }

    recv_port               = atoi(argv[3]);
    ifr.ifr_addr.sa_family  = AF_INET;
    strncpy(group_addr, argv[2], sizeof(group_addr));
    strncpy(ifr.ifr_name, argv[1], IFNAMSIZ-1);

	if((fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
		printf("create socket error\n");
		return -1;
	}

    ioctl(fd, SIOCGIFADDR, &ifr);
    inet_ntop(AF_INET, &((struct sockaddr_in *)&ifr.ifr_ifru.ifru_addr)->sin_addr.s_addr,
                       if_addr, sizeof(if_addr));

    printf("interface:%x\n", ((struct sockaddr_in *)&ifr.ifr_ifru.ifru_addr)->sin_addr.s_addr);
    printf("Interface[%s] %s:%hu\n", if_addr, group_addr, recv_port);

	memset(&srv, 0, sizeof(struct sockaddr_in));
	memset(&cli, 0, sizeof(struct sockaddr_in));
	memset(&mreq, 0, sizeof(struct ip_mreq));

    mreq.imr_interface.s_addr = ((struct sockaddr_in *)&ifr.ifr_ifru.ifru_addr)->sin_addr.s_addr;

	srv.sin_family      = AF_INET;
	srv.sin_port        = htons(recv_port);
    srv.sin_addr.s_addr = mreq.imr_interface.s_addr;

    cli.sin_family      = AF_INET;
    cli.sin_port        = htons(recv_port);
    cli.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(fd, (struct sockaddr *)&srv, sizeof(srv))){
		printf("bind error\n");
		return -1;
	}

	if(inet_aton(group_addr, &mreq.imr_multiaddr) < 0)
	{
		printf("inet aton");
		return -1;
	}

	if(setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
	{
		perror("setsockopt\n");
		return -1;
	}

	n = sizeof(cli);

	while(1)
	{
		if((r = recvfrom(fd, buf, MAXBUF, 0, (struct sockaddr *)&cli, (socklen_t *)&n)) < 0 )
		{
			perror("recvfrom\n");
		}
		else
		{
			buf[r] = 0;
			fprintf(stdout, "Message %s: %s", inet_ntoa(cli.sin_addr), buf);
		}
	}

error:
    return -1;
}
