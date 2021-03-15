// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//  �� �� ��:  log_manager.cpp
//  ��    ��:  Jiaxing Shao
//  �� �� ��:  1.0
//  ����ʱ��:  2020��10��19��
//  Compiler:  g++
//  ��    ��:  
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

