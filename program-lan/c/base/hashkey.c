#include <stdio.h>

int hashkey(int first, int second);
int main(int argc, char** argv)
{
	if(argv + 1 == NULL || argv + 2 == NULL)
	{
		printf(" it is falt , please check again\n");
		return -1;
	}
	printf("first arg:%s, second arg:%s\n", argv[1], argv[2]);
	int first = atoi(argv[1]);
	int second = atoi(argv[2]);

	printf("hash two number: %d\n", hashkey(first, second));

	return 0;
}
int hashkey(int first, int second)
{
	unsigned int ret1;
	unsigned short  ret2;
	unsigned char ret3;

	ret1 = first ^ second;
	ret2 = (ret1 & 0x0000ffff) ^ ((ret1 & 0xffff0000) >> 16);
	ret3 = (ret2 & 0x00ff) ^ ((ret2 & 0xff00) >> 8);
	return (ret3 % 200);
}
