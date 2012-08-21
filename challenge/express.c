#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define EXPRESS_SIZE 1024
char* handle_express(const char* h
int main(int argc, char** argv)
{
	if(argv[1] != "")
	  printf("you have input, %s\n", argv[1]);

	char express[EXPRESS_SIZE];
	memset(express, 0 , EXPRESS_SIZE);
	strncpy(express, argv[1], sizeof(argv[1]));
	

	return 0;
}
