// =====================================================================================
//	Copyright (C) 2019 by Jiaxing Shao.All rights reserved.
//
//	文 件 名:  logger.cpp
//	作    者:  Jiaxing Shao, 13315567369@163.com
//	版 本 号:  1.0
//	创建时间:  2019年02月15日 15时43分08秒
//	Compiler:  g++
//	描    述:  
// =====================================================================================

#include "logger.h"

#include <dirent.h>
#include <stdarg.h>
#include <sys/stat.h>

#include <mutex>
#include <thread>
#include <cstring>

static char log_root_directory[MAX_LOG_PATH_LEN];
static log_queue_t *log_queues[MAX_LOG_QUEUE_COUNT];
/* logger queue counter */
static int log_queue_count = 0;
/* log worker stop flag*/
static bool log_stop_flag = false;
static std::mutex log_queue_lock;
#if defined(__GNUC__)
static pthread_t log_thread_id;
#elif defined(_MSC_VER)
static HANDLE log_thread_id;
#endif

void check_directory(const char *directory)
{
#if defined(__GNUC__)
	DIR *dir = opendir(directory);
	if(dir == NULL){
		mkdir(directory, S_IRWXU);
	}else{
		closedir(dir);
	}
#elif defined(_MSC_VER)
	if (_access(directory, 0) == -1) {
		CreateDirectoryA(directory, NULL);
	}
#endif
}

void logfile_open(log_queue_t *logger)
{
	char log_file_path[MAX_LOG_PATH_LEN] = { 0 };
	char log_file_name[MAX_LOG_PATH_LEN] = { 0 };
	struct tm local;
#if defined(__GNUC__)
	localtime_r(&logger->lasttime, &local);
	sprintf(log_file_path, "%s/%0.4d%0.2d%0.2d", log_root_directory, local.tm_year + 1900, local.tm_mon + 1, local.tm_mday);
	sprintf(log_file_name, "%s/%s.log", log_file_path, logger->logname);
#elif defined(_MSC_VER)
	localtime_s(&local, &log_queue->lasttime);
	sprintf_s(log_file_path, MAX_LOG_PATH_LEN, "%s\\%0.4d%0.2d%0.2d", log_root_directory, local.tm_year + 1900, local.tm_mon + 1, local.tm_mday);
	sprintf_s(log_file_name, MAX_LOG_PATH_LEN, "%s\\%s.log", log_file_path, log_queue->logname);
#endif
	check_directory(log_file_path);
	logger->file.open(log_file_name, std::ios::in | std::ios::out | std::ios::app);
}

#if defined(__GNUC__)
static void *log_worker_func(void *arg)
#elif defined(_MSC_VER)
static unsigned int __stdcall log_worker_func(void *arg)
#endif
{
	while(true)
	{
		if(log_stop_flag){
			break;
		}

		time_t curtime = time(NULL);
		int count = 0;
		for(int i = 0; i < log_queue_count; i++) {
			log_queue_t *queue = log_queues[i];
			log_t *log = GET_CONSUMER(queue);
			if(log == NULL){
				count++;
				continue;
			}

			if(curtime - queue->lasttime >= 86400) {
				queue->file.close();
				queue->lasttime = curtime - (curtime % 86400);
				logfile_open(queue);
			}
			queue->file.write(log->buf, strlen(log->buf));
			queue->file.flush();
			PUT_CONSUMER(queue);
		}

		if(log_queue_count == count){
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
	}
#if defined(_MSC_VER)
	return 0;
#endif
}


void logger_worker_thread_start(const char *rootDir)
{
	check_directory(rootDir);
	memset(log_root_directory, 0, MAX_LOG_PATH_LEN);
	memcpy(log_root_directory, rootDir, strlen(rootDir));
#if defined(__GNUC__)
	pthread_create(&log_thread_id, 0, log_worker_func, NULL);
#elif defined(_MSC_VER)
	log_thread_id = (HANDLE)_beginthreadex(NULL, 0, log_worker_func, NULL, 0, NULL);
#endif
}

void logger_worker_thread_stop()
{
	log_stop_flag = true;
#if defined(__GNUC__)
	pthread_join(log_thread_id, NULL);
#elif defined(_MSC_VER)
	WaitForSingleObject(log_thread_id, INFINITE);
#endif
	for(int i = 0; i < log_queue_count; i++){
		logger_queue_free(log_queues[i]);
	}
	log_queue_count = 0;
}

log_queue_t *logger_queue_create(const char *logName)
{
	std::lock_guard<std::mutex> lock(log_queue_lock);
	if(log_queue_count >= MAX_LOG_QUEUE_COUNT){
		return NULL;
	}
	log_queue_t *logger = new log_queue_t;
	if(logger == NULL){
		return NULL;
	}

	logger->producer = 0;
	logger->consumer = 0;
	memset(logger->logname, 0, MAX_LOG_PATH_LEN);
	memset(logger->items, 0, MAX_LOG_QUEUE_SIZE * sizeof(log_t));
	logger->size = MAX_LOG_QUEUE_SIZE;
	memcpy(logger->logname, logName, strlen(logName));
	time_t timer = time(NULL);
	logger->lasttime = timer - (timer % 86400);
	logfile_open(logger);
	log_queues[log_queue_count++] = logger;
	return logger;
}

void logger_queue_free(log_queue_t *&logger)
{
	if(logger == NULL){
		return;
	}
	logger->file.close();
	delete logger;
	logger = NULL;
}

bool logger_record(log_queue_t *logger, const char *format, ...)
{
	log_t *log = GET_PRODUCER(logger);
	if(log == NULL) {
		return false;
	}

	va_list args;
	va_start(args, format);
#if defined(__GNUC__)
	vsprintf(log->buf, format, args);
#elif defined(_MSC_VER)
	vsprintf_s(log->buf, format, args);
#endif
	va_end(args);
	PUT_PRODUCER(logger);
	return true;
}

