// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//  文 件 名:  main.cpp
//  作    者:  Jiaxing Shao
//  版 本 号:  1.0
//  创建时间:  2020年10月16日
//  Compiler:  g++
//  描    述:  
// =====================================================================================

#include "kafka_producer.h"

#include <iostream>
using namespace std;


std::string get_current_time()
{   
    time_t timer = time(nullptr);
    char curTime[32] = { 0 };  
    struct tm local;
#if defined(HAVE_UNISTD_H)
    localtime_r(&timer, &local);
#elif defined(HAVE_WINDOWS_H)
    localtime_r($timer, &local);
#endif
    int size = sprintf(curTime, "%04d-%02d-%02d %02d:%02d:%02d", local.tm_year + 1900, local.tm_mon + 1,
			                        local.tm_mday, local.tm_hour, local.tm_min, local.tm_sec);
	std::string strTime = curTime;
	return strTime;
}

 
int main(int argc, char *argv[]) 
{ 
    std::string kafka_brokers = "192.168.23.100:9092";
	std::string kafka_topic = "scats";
	ProducerKafka *kafka_producer = new ProducerKafka(); 
			    
	if (PRODUCER_INIT_SUCCESS == kafka_producer->producerInit(0, kafka_brokers.c_str(), kafka_topic.c_str())) { 
		printf("producer init success"); 
	} else { 
		printf("producer init failed"); 
		return -1; 
	}
	
	while(true){ 
		string strtime = get_current_time(); 
		if(PUSH_DATA_SUCCESS == kafka_producer->pushDataToKafka(strtime.c_str(), strtime.length())){ 
			printf("push data success %s", strtime.c_str()); 
		} else { 
			printf("push data failed %s", strtime.c_str()); 
		} 
		sleep(3); 
	} 
	return 0; 
} 
