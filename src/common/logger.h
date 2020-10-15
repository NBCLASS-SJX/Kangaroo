// =====================================================================================
//	Copyright (C) 2019 by Jiaxing Shao.All rights reserved.
//
//	文 件 名:  logger.h
//	作    者:  Jiaxing Shao, 13315567369@163.com
//	版 本 号:  1.0
//	创建时间:  2019年02月15日 15时43分21秒
//	Compiler:  g++
//	描    述:  高速写入日志时,会有丢失日志的情况
// =====================================================================================

#ifndef _KGR_LOGGER_H_H_H
#define _KGR_LOGGER_H_H_H

#include "common.h"

#include <fstream>

#define MAX_LOG_QUEUE_COUNT (1024)
#define MAX_LOG_QUEUE_SIZE 	(1024 * 16)
#define MAX_LOG_LINE_LENGTH	(2048)
#define MAX_LOG_PATH_LEN (1024)

#define GET_CONSUMER(row) ((row)->producer != (row)->consumer) ? &(row)->items[(row)->consumer] : NULL
#define PUT_CONSUMER(row) (row)->consumer = ((row)->consumer + 1) % (row)->size
#define GET_PRODUCER(row) (((row)->producer + 1) % (row)->size != (row)->consumer) ? &(row)->items[(row)->producer] : NULL
#define PUT_PRODUCER(row) (row)->producer = ((row)->producer + 1) % (row)->size


#if defined(__GNUC__)
#define logger_format_string()

#define log_record(logger, format, args...) do{ if(std::string log_format_time = get_current_time();\
		if(logger_record(lo))
		} while(0)

#endif


#if defined(__GNUC__)
#define log_debug(format, args...) do { std::string log_debug_time = GetSystemTime();\
	printf("%s [%s] [line: %d] [debug]: " format "\n", log_debug_time.c_str(), __FILE__, __LINE__, ##args); } while(0)
#define log_info(logger, format, args...) do{ if(logger == NULL) break; std::string log_time_str = GetSystemTime(); \
	if(logger_record(logger, "%s [info]: " format "\n", log_time_str.c_str(), ##args)) break; } while(true)
#define log_warn(logger, format, args...) do{ if(logger == NULL) break; std::string log_time_str = GetSystemTime(); \
	if(logger_record(logger, "%s [warn]: " format "\n", log_time_str.c_str(), ##args)) break; } while(true)
#define log_error(logger, format, args...) do{ if(logger == NULL) break; std::string log_time_str = GetSystemTime(); \
	if(logger_record(logger, "%s [error]: " format "\n", log_time_str.c_str(), ##args)) break; } while(true)
#elif defined(_MSC_VER)




#define log_debug(format, ...) do { std::string log_debug_time = GetSystemTime();\
	printf("%s [debug]: " format "\n", log_debug_time.c_str(), ##__VA_ARGS__);} while(0)
#define log_info(logger, format, ...) do{ if(logger == NULL) break; std::string log_time_str = GetSystemTime(); \
	if(logger_record(logger "%s [info]: " format "\n", log_time_str.c_str(), ##__VA_ARGS__)) break; }while(true)
#define log_warn(logger, format, ...) do{ if(logger == NULL) break; std::string log_time_str = GetSystemTime(); \
	if(logger_record(logger, "%s [warn]: " format "\n", log_time_str.c_str(), ##__VA_ARGS__)) break; }while(true)
#define log_error(logger, format, ...) do{ if(logger == NULL) break; std::string log_time_str = GetSystemTime(); \
	if(logger_record(logger, "%s [error]: " format "\n", log_time_str.c_str(), ##__VA_ARGS__)) break; }while(true)
#endif

typedef enum {
	LOG_LEVEL_DEBUG = 1,
	LOG_LEVEL_INFO = 2,
	LOG_LEVEL_WARN = 3,
	LOG_LEVEL_TYPE_ERROR = 4
}log_level_e;

typedef struct {
	char buf[MAX_LOG_LINE_LENGTH];
} log_t;

typedef struct {
	std::fstream file;
	int size;
	int producer;
	int consumer;
	time_t lasttime;
	char logname[MAX_LOG_PATH_LEN];
	log_t items[MAX_LOG_QUEUE_SIZE];
} log_queue_t;


void logger_worker_thread_start(const char *rootDir);
void logger_worker_thread_stop();
log_queue_t *logger_queue_create(const char *logName);
void logger_queue_free(log_queue_t *&logger);

bool logger_record(log_queue_t *logger, const char *format, ...);

#endif

