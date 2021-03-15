// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//  �� �� ��:  logger.cpp
//  ��    ��:  Jiaxing Shao
//  �� �� ��:  1.0
//  ����ʱ��:  2020��10��19��
//  Compiler:  g++
//  ��    ��:  
// =====================================================================================

#include "logger.h"

Logger *Logger::create()
{
	return new Logger();
}

void Logger::init(const char *log_name)
{
	filename = new char[LOG_NAME_LEN];
	memset(filename, 0, LOG_NAME_LEN);
	memcpy(filename, log_name);
}

void Logger::destroy()
{
	if(filename != nullptr) {
		delete [] filename;
		filename = nullptr;
	}
}

void Logger::debug()
{
}

void Logger::info()
{
}

void Logger::warn()
{
}

void Logger::error()
{
}


