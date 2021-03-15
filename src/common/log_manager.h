// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//  �� �� ��:  log_manager.h
//  ��    ��:  Jiaxing Shao
//  �� �� ��:  1.0
//  ����ʱ��:  2020��10��19��
//  Compiler:  g++
//  ��    ��:  
// =====================================================================================

#ifndef _LOG_MANAGER_H_H_H
#define _LOG_MANAGER_H_H_H

#include "singleton.h"
#include "logger.h"

#define LOGGER_QUEUE_MAX 512

class LogManager: public Singleton
{
public:
	bool logManagerSatrt(const char *log_path);
	bool logManagerStop();
	static Logger *getLogger(const char *log_name);
	static bool delLogger(Logger *&logger);
	static void *logWorkerThread(void *arg);
private:
	bool checkDirectory(const char *directory);
	bool logFileCreate(Logger *logger);
private:
	char *log_directory;
	int log_queue_count;
	Logger *logger_queue[LOGGER_QUEUE_MAX];
	std::mutex lock;
#if defined(__GNUC__)
	pthread_t thread_id;
#elif defined(_MSC_VER)
	HANDLE thread_id;
#endif
};

#endif

