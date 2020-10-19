// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//  �� �� ��:  log_manager.h
//  ��    ��:  Jiaxing Shao
//  �� �� ��:  1.0
//  ����ʱ��:  2020��10��19��
//  Compiler:  g++
//  ��    ��:  
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

