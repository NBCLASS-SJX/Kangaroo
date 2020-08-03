// =====================================================================================
//	Copyright (C) 2019 by Jiaxing Shao.All rights reserved.
//
//	文 件 名:  threadpool.h
//	作    者:  Jiaxing Shao, 13315567369@163.com
//	版 本 号:  1.0
//	创建时间:  2019年04月17日 18时13分18秒
//	Compiler:  g++
//	描    述:  
// =====================================================================================

#ifndef __THREAD_POOL_H_H_H
#define __THREAD_POOL_H_H_H

#include <stdlib.h>

#include <set>
#include <list>
#include <string>
#include <mutex>
#include <condition_variable>


#if defined(__GNUC__)
#include <pthread.h>
#elif defined(_MSC_VER)
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif //!WIN32_LEAN_AND_MEAN
#include <Winsock2.h>
#include <Windows.h>
#include <process.h>
#pragma comment(lib, "Ws2_32.lib")
#undef WIN32_LEAN_AND_MEAN
#endif //!_WIN32
#endif

// 线程池任务
typedef struct
{
	// 要执行的函数
	void (*callback)(void *arg);
	// 参数
	void *arg;
	// 参数内存是否需要释放, 默认为false
	bool release;
}t_threadpool_task;

// 创建任务
t_threadpool_task *create_threadpool_task(void (*callback)(void *), void *arg);
t_threadpool_task *create_threadpool_task(void (*callback)(void *), void *arg, bool release);
// 释放任务
void free_threadpool_task(t_threadpool_task *&task);
// 添加任务到队列
int threadpool_add_task(t_threadpool_task *task);
// 线程池初始化
void threadpool_start(int num, int max_task_num);
// 停止线程池任务
void threadpool_stop();

#endif

