#include "ace/OS.h"
#include "ace/Task.h"
#include "ace/Future.h"
#include "ace/Activation_Queue.h"
#include "ace/Method_Request.h"

class Logger: public ACE_Task<ACE_MT_SYNCH>
{
public:
	Logger();
	virtual ~Logger(void);
	virtual int open(void *);
	virtual int close(u_long flags = 0);

	// The entry point for all threads created in the Logger
	virtual int svc(void);

	ACE_Future<u_long> logMsg(const char* msg);

	ACE_Future<const char*> name(void);

	u_long logMsg_i(const char* msg);
	const char* name_i();

private:
	char* name_;
	ACE_Activation_Queue activation_queue_;
};

class logMsg_MO: public ACE_Method_Request
{
public:
	logMsg_MO(Logger *logger, const char* msg, ACE_Future<u_long> &future_result);
	virtual ~logMsg_MO();
	virtual int call(void);

private:
	Logger *logger_;
	const char* msg_;
	ACE_Future<u_long> future_result_;
};

class name_MO: public ACE_Method_Request
{
public:
	name_MO(Logger *logger, ACE_Future<const char*> &future_result);
	virtual ~name_MO();
	virtual int call(void);

private:
	Logger *logger_;
	ACE_Future<const char*> future_result_;
};

logMsg_MO::logMsg_MO(Logger *logger, const char* msg, ACE_Future<u_long> &future_result)
{
	ACE_DEBUG((LM_INFO, "(%t) logMsg invoked \n"));
}

logMsg_MO::~logMsg_MO()
{
	ACE_DEBUG((LM_DEBUG, "(%t) logMsg object deleted.\n"));
}

int logMsg_MO::call(void)
{
	return this->future_result_.set(this->logger_->logMsg_i(this->msg_));
}

name_MO::name_MO(Logger *logger, ACE_Future<const char*> &future_result):
	logger_(logger), future_result_(future_result)
{
	ACE_DEBUG((LM_DEBUG, "(%t) name() invoked \n"));
}

name_MO::~name_MO()
{
	ACE_DEBUG((LM_DEBUG, "(%t) name object deleted. \n"));
}

int name_MO::call(void)
{
	return this->future_result_.set(this->logger_->name_i());
}

Logger::Logger()
{
	this->name_ = new char[sizeof("Worker")];
	ACE_OS::strcpy(name_, "Worker");
}

Logger::~Logger(void)
{
	delete this->name_;
}

int Logger::open(void *)
{
	ACE_DEBUG((LM_DEBUG, "(%t) Logger %s open\n", this->name_));
	return this->activate(THR_NEW_LWP);
}

int Logger::close(u_long flags)
{
	ACE_DEBUG((LM_DEBUG, "Closing Logger\n"));
	return 0;
}

int Logger::svc(void)
{
	while(1)
	{
		auto_ptr<ACE_Method_Request> mo(this->activation_queue_.dequeue());
		ACE_DEBUG((LM_DEBUG, "(%t) calling method object\n"));

		if(mo->call() == -1)
			break;
	}
	return 0;
}

ACE_Future<u_long> Logger::logMsg(const char* msg)
{
	ACE_Future<u_long> resultant_future;
	this->activation_queue_.enqueue(new logMsg_MO(this, msg, resultant_future));
	return resultant_future;
}

ACE_Future<const char*> Logger::name(void)
{
	ACE_Future<const char*> resultant_future;

	// Create and enqueue onto the activation queue
	
	this->activation_queue_.enqueue(new name_MO(this, resultant_future));
	return resultant_future;
}

u_long Logger::logMsg_i(const char* msg)
{
	ACE_DEBUG((LM_DEBUG, "Logged: %s\n", msg));
	ACE_OS::sleep(2);

	return 10;
}

const char* Logger::name_i()
{
	ACE_OS::sleep(2);
	return name_;
}

int main(int argc, char* argv[])
{
	Logger *logger = new Logger;

	int n_loops = 10;
	ACE_Future<u_long> logresult;
	ACE_Future<const char*> name;

	logger->open(0);

	for (size_t i = 0; i < n_loops; i++)
	{
		char* msg = new char[50];
		ACE_DEBUG((LM_DEBUG, "Issuing a non-blocking logging call\n"));
		ACE_OS::sprintf(msg, "This is iteration %d", i);
		logresult = logger->logMsg(msg);
	}

	ACE_DEBUG((LM_DEBUG, "(%t) Invoked all the log calls and can now continue with other work \n"));

	name = logger->name();
	if(name.ready())
		ACE_DEBUG((LM_DEBUG, "Name is ready! \n"));
	else
		ACE_DEBUG((LM_DEBUG, "Blocking till I get the result of that call \n"));

	const char* task_name;
	name.get(task_name);
	ACE_DEBUG((LM_DEBUG, "(%t)==> The name of the task is: %s\n\n\n", task_name));
	ACE_Thread_Manager::instance()->wait();
}
