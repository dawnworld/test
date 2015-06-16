#include "ace/Synch.h"
#include "ace/Thread.h"
#include <list>
//Arguments that are to be passed to the worker thread are passed
//through this struct.
struct Args
{
public:
    Args(int iterations): mutex_(),iterations_(iterations){} 
    ACE_Thread_Mutex * mutex_;
    int iterations_;
    std::list<int> * num_list;

};
//The starting point for the worker threads
static void* worker(void*arguments)
{
    Args *arg= (Args*) arguments;
    ACE_Guard<ACE_Thread_Mutex> guard(*(arg->mutex_));
    std::list<int>::iterator iter;
    for(iter = arg->num_list->begin(); iter != arg->num_list->end(); iter++)
    {
        ACE_DEBUG((LM_DEBUG, " (%t) iter number: %d\n", *iter));
        ACE_OS::sleep(1);
    }
//    for(int i=0;i<arg->iterations_;i++)
//    {
//        ACE_DEBUG((LM_DEBUG,
//                        "(%t) Trying to get a hold of this iteration\n"));
//        //This is our critical section
//        //arg->mutex_.acquire();
//        ACE_DEBUG((LM_DEBUG,"(%t) This is iteration number %d\n",i));
//        //simulate critical work
//        //arg->mutex_.release();
//    }
    return 0;
}
int main(int argc, char*argv[])
{
    if(argc<2) {
        ACE_OS::printf("Usage: %s <number_of_threads> <number_of_iterations>\n", argv[0]);
        ACE_OS::exit(1);
    }
    int it_max = ACE_OS::atoi(argv[2]);
    Args arg(it_max);
    std::list<int> *my_list = new std::list<int>;
    ACE_Thread_Mutex * my_mutex = new ACE_Thread_Mutex;
    for(int i = 0; i < it_max; i ++)
        my_list->push_back(i);
    arg.num_list = my_list;
    arg.mutex_ = my_mutex;
    //Setup the arguments
    int n_threads = ACE_OS::atoi(argv[1]);
    //determine the number of threads to be spawned.
    ACE_thread_t *threadID = new ACE_thread_t[n_threads+1]; 
    ACE_hthread_t *threadHandles = new ACE_hthread_t[n_threads+1]; 
    if(ACE_Thread::spawn_n(threadID, //id’s for each of the threads
                    n_threads, //number of threads to spawn 
                    (ACE_THR_FUNC)worker, //entry point for new thread 
                    &arg, //args to worker 
                    THR_JOINABLE | THR_NEW_LWP, //flags 
                    ACE_DEFAULT_THREAD_PRIORITY,
                    0, 0, threadHandles)==-1)
        ACE_DEBUG((LM_DEBUG,"Error in spawning thread\n"));
    //spawn n_threads
    for(int i=0; i<n_threads; i++)
      ACE_Thread::join(threadHandles[i]);
    //Wait for all the threads to exit before you let the main fall through
    //and have the process exit.
    return 0;
}

