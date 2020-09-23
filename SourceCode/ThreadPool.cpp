#include "ThreadPool.h"

ThreadPool::ThreadPool()
{
	coresNumber = std::thread::hardware_concurrency();

	renderMutex = new std::mutex[coresNumber];
	wasExecuted = new std::atomic<bool>[coresNumber];
	wasTerminated = new std::future<bool>[coresNumber];

	isWorking.store(true);

	/* create threads */
	for (uint32_t i = 0; i < coresNumber; i++)
	{
		wasExecuted[i].store(0);

		/* start async threads. If isTerminated == 1, then thread was terminated */
		wasTerminated[i] = std::async(std::launch::async, &ThreadPool::infinteFunction, this, i);
	}
}

ThreadPool::~ThreadPool()
{
	delete[] wasTerminated;
	delete[] renderMutex;
	delete[] wasExecuted;
}

bool ThreadPool::infinteFunction(uint32_t threadNumber)
{
	while (isWorking.load())
	{
		while (!functionsQueue.empty())
		{
			/* block queue mutex to get first function */
			queueMutex.lock();

			/* if queue is not empty, then get its first function and remove it */
			if (!functionsQueue.empty())
			{
				/* get function */
				auto func = functionsQueue.front();
				/* remove function from queue */
				functionsQueue.pop();
				/* unlock mutex */
				queueMutex.unlock();
				/* execute received function */
				func();
			} 
			else 
			{
				/* if queue is empty, then unlock its mutex */
				queueMutex.unlock();
			}
		}
	
		/* thread executed its functions. Next it will fall asleep */
		wasExecuted[threadNumber].store(true);

		/* block thread's mutex and pause it */
		std::unique_lock<std::mutex> lock(renderMutex[threadNumber]);

		/* false awake protection */
		while (wasExecuted[threadNumber].load())
		{
			/* waiting for 2 conditions : wasExecuted[threadNumber] == true, cv_thread.notify_all() */
			threadConVar.wait(lock);
		}
	}

	return true;
}

bool ThreadPool::isThreadFinished()
{
	for (uint32_t i = 0; i < coresNumber; i++)
	{
		if (!wasExecuted[i])
			return false;
	}

	return true;
}

void ThreadPool::addFunctionToQueue(const std::function<void()>& func)
{
	queueMutex.lock();
	functionsQueue.push(func);
	queueMutex.unlock();
}

void ThreadPool::clearQueue()
{
	std::queue<std::function<void()>> _queue;

	queueMutex.lock();

	/* if current queue is not empty, then swap it with empty one */
	if (!functionsQueue.empty())
		functionsQueue.swap(_queue);

	queueMutex.unlock();
}

void ThreadPool::resumeAllThreads() 
{
	/* unpause threads */
	for (uint32_t i = 0; i < coresNumber; i++)
	{
		wasExecuted[i].store(false);
	}

	threadConVar.notify_all();
}

void ThreadPool::pauseAllThreads()
{
	clearQueue();

	/* waiting while threads will complete their functions and pause */
	for (uint32_t i = 0; i < coresNumber; i++)
	{
		while (!wasExecuted[i].load()) 
		{
		}
	}
}

void ThreadPool::terminateAllThreads()
{
	if (isWorking.load())
	{
		pauseAllThreads();

		/* stop infinite thread cycle */
		isWorking.store(false);

		/* threads will be terminated */
		resumeAllThreads();

		/* block main thread by .get() and wait until other threads will be terminated */
		for (uint32_t i = 0; i < coresNumber; i++)
		{
			wasTerminated[i].get();
		}
	}
}