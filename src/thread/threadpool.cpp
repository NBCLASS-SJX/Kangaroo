// =====================================================================================
//	Copyright (C) 2019 by Jiaxing Shao.All rights reserved.
//
//	文 件 名:  threadpool.cpp
//	作    者:  Jiaxing Shao, 13315567369@163.com
//	版 本 号:  1.0
//	创建时间:  2019年04月17日 18时13分04秒
//	Compiler:  g++
//	描    述:  
// =====================================================================================

#include "threadpool.h"

#ifdef __GNUC__
pthread_t *tid = NULL;
#else
HANDLE *tid = NULL;
#endif
std::mutex threadpool_lock;
std::condition_variable threadpool_cond;
std::list<t_threadpool_task*> listTasks;
int nThreadNum = 0;
int nTaskMaxSize;
bool bShutdown;

t_threadpool_task *create_threadpool_task(void (*callback)(void *), void *arg)
{
	t_threadpool_task *task = (t_threadpool_task*)malloc(sizeof(t_threadpool_task));
	task->callback = callback;
	task->arg = arg;
	task->release = false;
	return task;
}

t_threadpool_task *create_threadpool_task(void (*callback)(void *), void *arg, bool release)
{
	t_threadpool_task *task = create_threadpool_task(callback, arg);
	task->release = release;
	return task;
}

void free_threadpool_task(t_threadpool_task *&task)
{
	if(task != NULL) 
	{
		if(task->release) 
		{
			free(task->arg);
			task->arg = NULL;
		}
		free(task);
		task = NULL;
	}
}

int threadpool_add_task(t_threadpool_task *task)
{
	if(task == NULL) {
		return -1;
	}
	
	std::lock_guard<std::mutex> guard(threadpool_lock);
	if(listTasks.size() == nTaskMaxSize) {
		return -1;
	}
	listTasks.push_back(task);
	threadpool_cond.notify_one();
	return 0;
}

#ifdef __GNUC__
static void *threadpool_func(void *arg)
#else
static unsigned int __stdcall threadpool_func(void *arg)
#endif
{
	while(true)
	{
		std::unique_lock<std::mutex> guard(threadpool_lock);
		while(listTasks.size() == 0 && !bShutdown) {
			threadpool_cond.wait(guard);
		}
		
		if(bShutdown) {
			return NULL;
//			pthread_exit(NULL);
		}

		t_threadpool_task *task = listTasks.front();
		listTasks.pop_front();
		guard.unlock();
		task->callback(task->arg);
		free_threadpool_task(task);
	}
	return NULL;
}

void threadpool_start(int thread_num, int max_task_num)
{
	// 线程池参数信息初始化
#ifdef __GNUC__
	tid = (pthread_t*)malloc(thread_num * sizeof(pthread_t));
#else
	tid = (HANDLE*)malloc(thread_num * sizeof(HANDLE));
#endif

	nThreadNum = thread_num;
	nTaskMaxSize = max_task_num;
	bShutdown = false;

	for(int i = 0; i < nThreadNum; i++) 
	{
#ifdef __GNUC__
		pthread_create(&tid[i], NULL, threadpool_func, NULL);
#else
		tid[i] = (HANDLE)_beginthreadex(NULL, 0, threadpool_func, NULL, 0, NULL);
#endif
	}
}

void threadpool_stop()
{
	bShutdown = true;
	threadpool_cond.notify_all();
	for(int i = 0; i < nThreadNum; i++) {
#ifdef __GNUC__
		pthread_join(tid[i], NULL);
#else
		WaitForSingleObject(tid[i], INFINITE);
#endif
	}

	if(tid != NULL)
	{
		free(tid);
		tid = NULL;
	}

	std::lock_guard<std::mutex> guard(threadpool_lock);
	std::list<t_threadpool_task*>::iterator iter;
	for(iter = listTasks.begin(); iter != listTasks.end(); iter++) {
		free_threadpool_task(*iter);
	}
}

