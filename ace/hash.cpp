#include <stdio.h>
#include "ace/Hash_Map_Manager.h"
#include "ace/Malloc_T.h"
#include "ace/Null_Mutex.h"
#include "ace/INET_Addr.h"


int show_addr(ACE_INET_Addr &addr);
int main()
{
	ACE_Hash_Map_Manager_Ex<int, ACE_INET_Addr, ACE_Hash<int>, ACE_Equal_To<int>, ACE_Null_Mutex> relation_tbl; 
	printf("test hash manager\n");

	ACE_INET_Addr addr, to_find_addr;
	int status;
	addr.set("172.16.16.170:8080", AF_INET);

	show_addr(addr);

	relation_tbl.open(10);
	relation_tbl.bind(100, addr);	

	// find from hash table
	
	status = relation_tbl.find(100, to_find_addr);
	if(!status)
	{
		printf("now i got you, lucky girl\n");
		show_addr(to_find_addr);
	}
	else
	{
		printf("where is my girl\n");
	}
	return 0;
}
int show_addr(ACE_INET_Addr &addr)
{
	char str_addr[64] = {0};
	addr.addr_to_string(str_addr, sizeof(str_addr));
	printf("you set addr is %s\n", str_addr);
	return 0;
}
