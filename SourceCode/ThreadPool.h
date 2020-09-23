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

	/* add function to queue */
	void addFunctionToQueue(const std::function<void()>& func);
	/* resume threads */
	void resumeAllThreads();
	/* pause threads */
	void pauseAllThreads();
	/* stop and delete all threads */
	void terminateAllThreads();
	/* did thread finished its function */
	bool isThreadFinished();
	/* remove all functions from queue */
	void clearQueue();

protected:
	/* infinite threads function */
	bool infinteFunction(uint32_t threadNumber);

	/* queue from functions to execute */
	std::queue<std::function<void()>> functionsQueue;
	/* condition of execution for all threads */
	std::atomic<bool> isWorking;
	/* number of logical cores */
	uint32_t coresNumber;
	/* = 1 if thread was terminated*/
	std::future<bool>* wasTerminated;
	/* conditional variable to pause all threads */
	std::condition_variable threadConVar;
	/* mutex for queue access */
	std::mutex queueMutex;
	/* mutexes to block each thread */
	std::mutex* renderMutex;
	/* did thread executed its functions */
	std::atomic<bool>* wasExecuted;
};

