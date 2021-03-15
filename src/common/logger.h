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

#define LOG_NAME_LEN 512

class Logger
{
private:
	Logger() {};
	Logger(const Logger &logger);
	Logger& operator = (const Logger &logger) {};
public:
	~Logger() {};
public:
	static Logger *create();
	void init(const char *log_name);
	void destroy();
public:
	void debug();
	void info();
	void warn();
	void error();
private:
	void log_record();
private:
	char *filename;
};

#endif

