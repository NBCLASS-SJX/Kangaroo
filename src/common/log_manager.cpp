// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//  文 件 名:  log_manager.cpp
//  作    者:  Jiaxing Shao
//  版 本 号:  1.0
//  创建时间:  2020年10月19日
//  Compiler:  g++
//  描    述:  
// =====================================================================================

#include "log_manager.h"

bool LogManager::logManagerSatrt(const char *log_path)
{
	log_directory = new char[512];
	memset(log_directory, 0, 512);
	memcpy(log_directory, 0, sizeof(log_path));
	log_queue_count = 0;
	
#if defined(__GNUC__)
	pthread_create(&thread_id, 0, )
#elif defined(_MSC_VER)

#endif
}

bool LogManager::logManagerStop()
{
	if(log_directory != nullptr) {
		delete [] log_directory;
		log_directory = nullptr;
	}
}

Logger *LogManager::getLogger(const char *log_name)
{
	Logger *logger =  Logger::create();
	logger->init(log_name);
}

bool LogManager::delLogger(Logger *&logger)
{
	logger->destroy();
}
	
void *LogManager::logWorkerThread(void *arg)
{
	while(true) {
	}
}

bool LogManager::checkDirectory(const char *directory)
{
#if defined(__GNUC__)
	DIR *dir = opendir(directory);
	if(dir == nullptr) {
		mkdir(directory, S_IRWXU);
	} else {
		closedir(dir);
	}
#elif defined(_MSC_VER)
	if(_access(directory, 0) == -1) {
		CreateDirectory(directory, nullptr);
	}
#endif
	return true;
}

bool LogManager::logFileCreate(Logger *logger)
{
}

