// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//  文 件 名:  logger.cpp
//  作    者:  Jiaxing Shao
//  版 本 号:  1.0
//  创建时间:  2020年10月19日
//  Compiler:  g++
//  描    述:  
// =====================================================================================

#include "logger.h"
#include <mutex>
#include <io.h>
#include <sstream>

#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #endif

    #include <winsock2.h>
    #include <windows.h>
    #pragma comment(lib, "Ws2_32.lib")
    #undef WIN32_LEAN_AND_MEAN
#endif

// set logger print level
static int log_record_level = LOG_LEVEL_DEBUG;
// log root directory
static char log_root_directory[MAX_LOG_PATH_LEN];
// logger queue size
static int log_queue_count = 0;
// logger queue
static log_queue_t *log_queue_list[MAX_LOG_QUEUE_COUNT];
// logger lock
static std::mutex log_queue_lock;
// keep log days
static int log_keep_days;
// last record datetime
static time_t last_datetime;
// log stop flag
static bool log_stop_flag = false;

#if defined(__GNUC__)
#include <dirent.h>

static pthread_t log_thread_id;
#elif defined(_MSC_VER)
#include <process.h>

static HANDLE log_thread_id;
#endif

void clean_logs_directory(const char *directory)
{
#if defined(__GNUC__)
#elif defined(_MSC_VER)
    struct _finddata_t fileinfo;
    char tmp_path[MAX_LOG_PATH_LEN] = {0};
    sprintf_s(tmp_path, "%s\\*.*", directory);
    long handle = _findfirst(tmp_path, &fileinfo);

    if (handle == -1) {
        return;
    }

    do {
        if (strcmp(fileinfo.name, ".") == 0 || strcmp(fileinfo.name, "..") == 0) {
            continue;
        }

        if (fileinfo.attrib & FILE_ATTRIBUTE_DIRECTORY) {
            if (last_datetime - fileinfo.time_create >= 86400 * log_keep_days) {
                char filepath[MAX_LOG_PATH_LEN] = { 0 };
                sprintf_s(filepath, "%s\\%s", directory, fileinfo.name);
                remove_directory(filepath);
            }
        }
    } while (!_findnext(handle, &fileinfo));
#endif   
}

void logger_file_open(log_queue_t *logger, const char *log_file_path)
{
    char log_file_name[MAX_LOG_PATH_LEN] = {0};
#if defined(__GNUC__)
    sprintf(log_file_name, "%s/%s.txt", log_file_path, log_queue->logname);
#elif defined(_MSC_VER)
    sprintf_s(log_file_name, MAX_LOG_PATH_LEN, "%s\\%s.txt", log_file_path, logger->logname);
#endif
    logger->file.open(log_file_name, std::ios::in | std::ios::out | std::ios::app);
}

#if defined(__GNUC__)
static void *log_worker_func(void *arg)
#elif defined(_MSC_VER)
static unsigned int __stdcall log_worker_func(void *arg)
#endif
{
    while (true) {
        if (log_stop_flag) {
            break;
        }

        time_t current_time = time(nullptr);
        if (current_time - last_datetime > 86400) {
            std::lock_guard<std::mutex> guard(log_queue_lock);
            char log_file_path[MAX_LOG_PATH_LEN] = {0};
            last_datetime = current_time - current_time % 86400 - 28800;
        #if defined(__GNUC__)
            struct tm local;
            localtime_r(&last_datetime, &local);
            sprintf(log_file_path, "%s/%0.4d%0.2d%0.2d", log_root_directory,
                local.tm_year + 1900, local.tm_mon + 1, local.tm_mday);
        #elif defined(_MSC_VER)
            struct tm local;
            localtime_s(&local, &last_datetime);
            sprintf_s(log_file_path, MAX_LOG_PATH_LEN, "%s\\%0.4d%0.2d%0.2d", log_root_directory,
                local.tm_year + 1900, local.tm_mon + 1, local.tm_mday);
        #endif
            clean_logs_directory(log_root_directory);
            mkdir_directory(log_file_path);
            
            for (int i = 0; i < log_queue_count; i++) {
                log_queue_t *logger = log_queue_list[i];
                logger->file.close();
                logger_file_open(logger, log_file_path);
            }
        }

        int count = 0;
        for (int i = 0; i < log_queue_count; i++) {
            log_queue_t *logger = log_queue_list[i];
            log_t *log = GET_CONSUMER(logger);
            if (log == NULL) {
                count++;
                continue;
            }
            logger->file.write(log->buf, strlen(log->buf));
            logger->file.flush();
            PUT_CONSUMER(logger);
        }

        if (log_queue_count == count) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
#if defined(_MSC_VER)
    return 0;
#endif
}

void logger_worker_thread_start(const char *root_dir, int keep_days, int log_level)
{
    time_t current_time = time(nullptr);
    last_datetime = current_time - 28800 - current_time % 86400;
    log_keep_days = keep_days;
    log_record_level = log_level;
    mkdir_directory(root_dir);
    memset(log_root_directory, 0, MAX_LOG_PATH_LEN);
    memcpy(log_root_directory, root_dir, strlen(root_dir));
#if defined(__GNUC__)
    pthread_create(&log_thread_id, 0, log_worker_func, NULL);
#elif defined(_MSC_VER)
    log_thread_id = (HANDLE)_beginthreadex(nullptr, 0, log_worker_func, nullptr, 0, nullptr);
#endif
}

void logger_worker_thread_stop(bool clear = true)
{
    log_stop_flag = true;
#if defined(__GNUC__)
    pthread_join(log_thread_id, NULL);
#elif defined(_MSC_VER)
    WaitForSingleObject(log_thread_id, INFINITE);
#endif
    if (clear) {
        for (int i = 0; i < log_queue_count; i++) {
            logger_queue_free(log_queue_list[i]);
        }
        log_queue_count = 0;
    }
}

log_queue_t *logger_queue_create(const char *logName)
{
    std::lock_guard<std::mutex> lock(log_queue_lock);
    if (log_queue_count >= MAX_LOG_QUEUE_COUNT) {
        return nullptr;
    }
    log_queue_t *logger = new log_queue_t;
    if (logger == nullptr) {
        return nullptr;
    }

    logger->producer = 0;
    logger->consumer = 0;
    memset(logger->logname, 0, MAX_LOG_PATH_LEN);
    memset(logger->items, 0, MAX_LOG_QUEUE_SIZE * sizeof(log_t));
    logger->size = MAX_LOG_QUEUE_SIZE;
    memcpy(logger->logname, logName, strlen(logName));

    time_t timer = time(nullptr);
    logger->lasttime = timer - (timer % 86400);

    char log_file_path[MAX_LOG_PATH_LEN] = {0};
#if defined(__GNUC__)
    struct tm local;
    localtime_r(&last_datetime, &local);
    sprintf(log_file_path, "%s/%0.4d%0.2d%0.2d", log_root_directory,
        local.tm_year + 1900, local.tm_mon + 1, local.tm_mday);
#elif defined(_MSC_VER)
    struct tm local;
    localtime_s(&local, &last_datetime);
    sprintf_s(log_file_path, MAX_LOG_PATH_LEN, "%s\\%0.4d%0.2d%0.2d", log_root_directory,
        local.tm_year + 1900, local.tm_mon + 1, local.tm_mday);
#endif
    mkdir_directory(log_file_path);

    logger_file_open(logger, log_file_path);
    log_queue_list[log_queue_count++] = logger;
    return logger;
}

void logger_queue_free(log_queue_t *&logger)
{
    if (logger == nullptr) {
        return;
    }
    logger->file.close();
    delete logger;
    logger = nullptr;
}

bool write_log_record(int level, log_queue_t *logger, const char *format, ...)
{
    if (log_record_level < level) {
        return true;
    }

    log_t *log = GET_PRODUCER(logger);
    if (log == nullptr) {
        return false;
    }

    va_list args;
    va_start(args, format);
#if defined(__GNUC__)
    vsprintf(log->buf, format, args);
#elif defined(_MSC_VER)
    vsprintf_s(log->buf, format, args);
#endif

#ifdef _DEBUG
    printf(log->buf);
#endif

    va_end(args);
    PUT_PRODUCER(logger);
    return true;
}
