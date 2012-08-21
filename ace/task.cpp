#include "ace/OS.h"
#include "ace/Task.h"

class TaskOne: public ACE_Task<ACE_MT_SYNCH>
{
public:
	int open(void*)
	{
		ACE_DEBUG((LM_DEBUG, "(%t) Active Object opened\n"));
		activate();
		return 0;
	}

	int close(u_long)
	{
		ACE_DEBUG((LM_DEBUG, "(%t) Active Object being closed down \n"));
		return 0;
	}

	int svc(void)
	{
		ACE_DEBUG((LM_DEBUG, "(%t) This is being done in a saperate thread\n"));
		return 0;
	}
};

int main(int argc, char *argv[])
{
	TaskOne *one = new TaskOne;

	// start up the task
	
	one->open(0);

	ACE_Thread_Manager::instance()->wait();
	ACE_DEBUG((LM_DEBUG, "(%t) Main Task ends \n"));
	return 0;
}
