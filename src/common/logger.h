// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//  文 件 名:  logger.h
//  作    者:  Jiaxing Shao
//  版 本 号:  1.0
//  创建时间:  2020年10月19日
//  Compiler:  g++
//  描    述:  
// =====================================================================================

#ifndef _LOGGER_H_H_H
#define _LOGGER_H_H_H

#include "common.h"
#include <fstream>
#include <string>

#define MAX_LOG_QUEUE_COUNT (1024)
#define MAX_LOG_QUEUE_SIZE 	(1024 * 8)
#define MAX_LOG_LINE_LENGTH	(1024 * 4)
#define MAX_LOG_PATH_LEN (1024)

#define LOG_LEVEL_FATAL  -1
#define LOG_LEVEL_ERROR   0
#define LOG_LEVEL_WARN    1
#define LOG_LEVEL_INFO    2
#define LOG_LEVEL_DEBUG   3

#define GET_CONSUMER(row) ((row)->producer != (row)->consumer) ? &(row)->items[(row)->consumer] : NULL
#define PUT_CONSUMER(row) (row)->consumer = ((row)->consumer + 1) % (row)->size
#define GET_PRODUCER(row) (((row)->producer + 1) % (row)->size != (row)->consumer) ? &(row)->items[(row)->producer] : NULL
#define PUT_PRODUCER(row) (row)->producer = ((row)->producer + 1) % (row)->size

#if defined(__GNUC__)
#define log_debug(_logger, _fmt, args...) do{                                                                                      \
    if(_logger == nullptr) break;                                                                                                  \
    std::string _log_time = get_current_time();                                                                                    \
    bool ret_ = write_log_record(LOG_LEVEL_DEBUG, _logger, "%s [debug]: " _fmt "\n", _log_time.c_str(), ##args);                   \
    if (ret_) break;                                                                                                               \
} while(true)

#define log_info(_logger, _fmt, args...) do{                                                                                       \
    if(_logger == nullptr) break;                                                                                                  \
    std::string _log_time = get_current_time();                                                                                    \
    bool ret_ = write_log_record(LOG_LEVEL_INFO, _logger, "%s [info]: " _fmt "\n", _log_time.c_str(), ##args);                     \
    if (ret_) break;                                                                                                               \
} while(true)

#define log_warn(_logger, _fmt, args...) do{                                                                                       \
    if(_logger == nullptr) break;                                                                                                  \
    std::string _log_time = get_current_time();                                                                                    \
    bool ret_ = write_log_record(LOG_LEVEL_WARN, _logger, "%s [warn]: " _fmt "\n", _log_time.c_str(), ##args);                     \
    if (ret_) break;                                                                                                               \
} while(true)

#define log_error(_logger, _fmt, args...) do{                                                                                      \
    if(_logger == nullptr) break;                                                                                                  \
    std::string _log_time = get_current_time();                                                                                    \
    bool ret_ = write_log_record(LOG_LEVEL_ERROR, _logger, "%s [error]: " _fmt "\n", _log_time.c_str(), ##args);                   \
    if (ret_) break;                                                                                                               \
} while(true)

#elif defined(_MSC_VER)
#define log_debug(_logger, _fmt, ...) do {                                                                                         \
    if(_logger == nullptr) break;                                                                                                  \
    std::string _log_time = get_current_time();                                                                                    \
    bool ret_ = write_log_record(LOG_LEVEL_DEBUG, _logger, "%s [debug]: " _fmt "\n", _log_time.c_str(), ##__VA_ARGS__);            \
    if (ret_) break;                                                                                                               \
} while (true)

#define log_info(_logger, _fmt, ...) do {                                                                                          \
    if(_logger == nullptr) break;                                                                                                  \
    std::string _log_time = get_current_time();                                                                                    \
    bool ret_ = write_log_record(LOG_LEVEL_INFO, _logger, "%s [info]: " _fmt "\n", _log_time.c_str(), ##__VA_ARGS__);              \
    if (ret_) break;                                                                                                               \
} while (true)

#define log_warn(_logger, _fmt, ...) do {                                                                                          \
    if(_logger == nullptr) break;                                                                                                  \
    std::string _log_time = get_current_time();                                                                                    \
    bool ret_ = write_log_record(LOG_LEVEL_WARN, _logger, "%s [warn]: " _fmt "\n", _log_time.c_str(), ##__VA_ARGS__);              \
    if (ret_) break;                                                                                                               \
} while (true)

#define log_error(_logger, _fmt, ...) do {                                                                                         \
    if(_logger == nullptr) break;                                                                                                  \
    std::string _log_time = get_current_time();                                                                                    \
    bool ret_ = write_log_record(LOG_LEVEL_ERROR, _logger, "%s [error]: " _fmt "\n", _log_time.c_str(), ##__VA_ARGS__);            \
    if (ret_) break;                                                                                                               \
} while (true)

#endif


typedef struct
{
    char buf[MAX_LOG_LINE_LENGTH];
} log_t;

typedef struct
{
    std::fstream file;
    int size;
    int producer;
    int consumer;
    time_t lasttime;
    char logname[MAX_LOG_PATH_LEN];
    log_t items[MAX_LOG_QUEUE_SIZE];
} log_queue_t;


/* start logger worker thread */
void logger_worker_thread_start(const char *root_dir, int keep_days, int log_level);

/* stop logger worker thread */
void logger_worker_thread_stop(bool clear);

/* create logger */
log_queue_t *logger_queue_create(const char *log_name);

/* free logger */
void logger_queue_free(log_queue_t *&logger);

/* write log */
bool write_log_record(int level, log_queue_t *logger, const char *format, ...);

#endif
