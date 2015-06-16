#include "ace/OS.h"
#include "ace/Task.h"
#include "ace/Message_Block.h"

class Consumer: public ACE_Task<ACE_MT_SYNCH>
{
public:
	int open(void*)
	{
		ACE_DEBUG((LM_INFO, "(%t) Producer task opened \n"));

		activate(THR_NEW_LWP, 1);
		return 0;
	}

	int svc(void)
	{
		ACE_Message_Block * mb = 0;
		do
		{
			mb = 0;

			getq(mb);
			ACE_DEBUG((LM_DEBUG, "(%t) Got message:%d from remote task\n", *mb->rd_ptr() ));
			ACE_DEBUG((LM_DEBUG, "the rd_ptr is %d\n", *mb->rd_ptr()));
		}while(*mb->rd_ptr() < 10);
		return 0;
	}

	int close(u_long)
	{
		ACE_DEBUG((LM_DEBUG, "Consumer closes down \n"));
		return 0;
	}
};

class Producer: public ACE_Task<ACE_MT_SYNCH>
{
public:
	Producer(Consumer * consumer):consumer_(consumer), data_(0)
	{
		mb_ = new ACE_Message_Block((char*) &data_, sizeof(data_));
	}

	int open(void*)
	{
		ACE_DEBUG((LM_DEBUG, "(%t) Producer task opened \n"));
		activate(THR_NEW_LWP, 1);
		return 0;
	}

	int svc(void)
	{
		while(data_ < 11)
		{
			ACE_DEBUG((LM_DEBUG, "(%t) Sending message: %d to remote task\n", data_));
			consumer_->putq(mb_);
			ACE_OS::sleep(1);
			data_++;
		}
		return 0;
	}

	int close(u_long)
	{
		ACE_DEBUG((LM_DEBUG, "Producer close down\n"));
		return 0;
	}

private:
	char data_;
	Consumer * consumer_;
	ACE_Message_Block * mb_;
};

int main(int argc, char* argv[])
{
	Consumer *consumer = new Consumer;
	Producer *producer = new Producer(consumer);

	producer->open(0);
	consumer->open(0);

	ACE_Thread_Manager::instance()->wait();
	return 0;
}
