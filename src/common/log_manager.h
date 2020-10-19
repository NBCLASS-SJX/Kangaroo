// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//  文 件 名:  log_manager.h
//  作    者:  Jiaxing Shao
//  版 本 号:  1.0
//  创建时间:  2020年10月19日
//  Compiler:  g++
//  描    述:  
// =====================================================================================

#ifndef _LOG_MANAGER_H_H_H
#define _LOG_MANAGER_H_H_H

#include "singleton.h"
#include "logger.h"

#define LIST_MAX_SIZE 512

class LogManager: public Singleton
{
public:
	static bool logManagerInit();
	static bool logManagerDestroy();
	static Logger *getLogger(const char *log_name);
	static bool delLogger(Logger *&logger);

private:
	Logger *logger_list[];
};

#endif

