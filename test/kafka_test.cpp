// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//
//  文 件 名:  kafka_test.cpp
//  作    者:  Jiaxing Shao, 13315567369@163.com
//  版 本 号:  1.0
//  创建时间:  2020年03月24日
//  Compiler:  g++
//  描    述:  
// =====================================================================================

#include "../src/kafka/kafka_producer.h"
#include "../src/common/kgr_logger.h"

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	std::string kafka_brokers = GetConfigureString("kafka.brokers", "192.168.23.100:9092");
	std::string kafka_topic = GetConfigureString("kafka.topic", "scats");

	ProducerKafka *kafka_producer = new ProducerKafka();
	if (PRODUCER_INIT_SUCCESS == kafka_producer->init_kafka(0, kafka_brokers.c_str(), kafka_topic.c_str())) {
		log_debug("producer init success");
	} else {
		 log_debug("producer init failed");
		 return -1;
	}

	while(true){
		string strtime = GetSystemTime();
		if(PUSH_DATA_SUCCESS == kafka_producer->push_data_to_kafka(strtime.c_str(), strtime.length())){
			log_debug("push data success %s", strtime.c_str());
		} else {
			log_debug("push data failed %s", strtime.c_str());
		}
		TimeSleep(3);
	}
	return 0;
}

