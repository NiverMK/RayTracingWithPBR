#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <functional>
#include <future>


class ThreadPool
{
public:

	ThreadPool();
	~ThreadPool();

	//add function to queue
	void addFunctionToQueue(const std::function<void()> &func);

	void resumeAllThreads();

	void pauseAllThreads();

	void terminateAllThreads();
	
	bool isThreadFinished();

	void clearQueue();

protected:

	//queue of functions
	std::queue<std::function<void()>> functionsQueue;

	bool isWorking;		//thread execution condition

	std::mutex queue_mutex;		//queue access mutex

	uint32_t num_of_cores;		//number of threads

	std::future<bool> *isTerminated;	//is thread terminated

	/*condition variable to pause threads*/
	std::condition_variable cv_thread;

	std::mutex *mutex_render;	//mutexes to block threads

	std::atomic<bool> *isFinished;		//whether the thread finished executing its function

	bool infinteFunction(uint32_t threadNumber);		//infinite function of thread
};

