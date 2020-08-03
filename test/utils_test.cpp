// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//
//  文 件 名:  utils_test.cpp
//  作    者:  Jiaxing Shao, 13315567369@163.com
//  版 本 号:  1.0
//  创建时间:  2020年03月24日
//  Compiler:  g++
//  描    述:  
// =====================================================================================

#include "../src/common/kgr_utils.h"
#include "../src/common/kgr_logger.h"

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	logger_worker_thread_start("./logs");
	log_queue_t *logger = logger_queue_create("test");
	string config = GetConfigureString("EXAMPLE", "测试");
	for(int i = 0; i < 10000; i++){
		log_debug("%s:%d", config.c_str(), i + 1);
		log_info(logger, "%s:%d", config.c_str(), i + 1);
	}
	sleep(3);
	logger_worker_thread_stop();
	return 0;
}
