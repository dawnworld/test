#include <ace/Log_Msg.h>
int ACE_TMAIN(int, ACE_TCHAR *[])
{
	ACE_DEBUG((LM_INFO, ACE_TEXT("HELLO ACE\n")));
	ACE_DEBUG((LM_INFO, ACE_TEXT("test my auto pair\n")));
	return 0;
}
