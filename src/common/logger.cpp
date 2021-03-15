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


