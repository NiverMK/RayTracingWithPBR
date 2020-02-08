#include "ThreadPool.h"


ThreadPool::ThreadPool() {
	
	num_of_cores = std::thread::hardware_concurrency();

	mutex_render = new std::mutex[num_of_cores];
	isFinished = new std::atomic<bool>[num_of_cores];
	isTerminated = new std::future<bool>[num_of_cores];

	isWorking = 1;

	//run async threads
	for (unsigned int i = 0; i < num_of_cores; i++) {
		isFinished[i].store(0);
		isTerminated[i] = (std::async(std::launch::async, &ThreadPool::infinteFunction, this, i));
	}
}



ThreadPool::~ThreadPool() {
	delete[] isTerminated;
	delete[] mutex_render;
	delete[] isFinished;
}



bool ThreadPool::infinteFunction(uint32_t threadNumber) {
	while (isWorking) {

		while (!functionsQueue.empty()) {

			queue_mutex.lock();	

			if (!functionsQueue.empty()) {
				
				auto func = functionsQueue.front();		//extract the first function from the queue

				functionsQueue.pop();	//delete it

				queue_mutex.unlock();

				func();		//run extracted function

			} else {

				queue_mutex.unlock();

			}
		}
	
		/*thread completed its function. Next, the thread will fall asleep until an awakening command arrives.*/
		isFinished[threadNumber].store(1);


		std::unique_lock<std::mutex> lock(mutex_render[threadNumber]);

		//protection against false awakening
		while (isFinished[threadNumber].load()) {
			cv_thread.wait(lock);
		}
	}

	return 1;
}



bool ThreadPool::isThreadFinished() {
	for (unsigned int i = 0; i < num_of_cores; i++) {
		if (!isFinished[i])
			return 0;
	}
	return 1;
}



void ThreadPool::addFunctionToQueue(const std::function<void()> &func) {
	queue_mutex.lock();	
	functionsQueue.push(func);
	queue_mutex.unlock();
}



void ThreadPool::clearQueue() {

	std::queue<std::function<void()>> _queue;	//empty queue

	queue_mutex.lock();

	if (!functionsQueue.empty())
		functionsQueue.swap(_queue);	//swap the empty container and the container with the queue

	queue_mutex.unlock();
}



void ThreadPool::resumeAllThreads() {
	//resume threads
	for (unsigned int i = 0; i < num_of_cores; i++) {
		isFinished[i].store(0);
	}
	cv_thread.notify_all();
}



void ThreadPool::pauseAllThreads() {

	clearQueue();

	//waiting while threads execute their functions
	for (unsigned int i = 0; i < num_of_cores; i++) {
		while (!isFinished[i].load()) {}
	}
}



void ThreadPool::terminateAllThreads() {
	if (isWorking) {
		
		clearQueue();

		//waiting while threads execute their functions
		for (unsigned int i = 0; i < num_of_cores; i++) {
			while (!isFinished[i].load()) {}
		}

		isWorking = 0;		//stop infinite while cycle

		resumeAllThreads();
		
		for (unsigned int i = 0; i < num_of_cores; i++) {
			/*main thread is blocked until the threads return a number.
			This will mean that the threads have successfully completed their functions*/
			isTerminated[i].get();
		}
	}
}
