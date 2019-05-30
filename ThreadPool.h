#include <iostream>
#include <thread>
#include <atomic>
#include <functional>
#include "thread_safe_queue.h"

using namespace std;

template <typename FT, typename IT>
class ThreadPool
{
	struct thread_pool_task
	{
		FT function;
		IT info;
	};

	atomic_bool done;
	vector<thread> threads;
	ThreadSafeQueue<thread_pool_task> work_queue;
	
	void worker_thread()
	{
		cout << "Thread id " << this_thread::get_id() << " start working" << endl;
		while(!done)
		{
			thread_pool_task task;
			work_queue.pop(task);
			task.function(task.info);
		}
	}

	public:
		ThreadPool(): done(false) 
		{
			unsigned const thread_count = thread::hardware_concurrency();
			cout << "spawn " << thread_count << " threads" << endl;
			try
			{
				for (unsigned i = 0; i < thread_count; i++)
				{
					threads.push_back(thread(&ThreadPool::worker_thread, this));
				}
			}
			catch(...)
			{
				cout << "something went wrong with the ThreadPool ctor" << endl;
				done = true;
				throw;
			}
		}

		~ThreadPool()
		{
			cout << "Thread pool dtor called" << endl;
			done = true;
		}

		void submit(FT f, IT info)
		{
			thread_pool_task task;
			task.function = f;
			task.info = info;
			work_queue.push(task);
		}
};
