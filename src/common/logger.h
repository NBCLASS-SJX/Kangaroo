// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//  �� �� ��:  logger.h
//  ��    ��:  Jiaxing Shao
//  �� �� ��:  1.0
//  ����ʱ��:  2020��10��19��
//  Compiler:  g++
//  ��    ��:  
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

