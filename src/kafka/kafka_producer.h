// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//
//  文 件 名:  kafka_producer.h
//  作    者:  Jiaxing Shao, 13315567369@163.com
//  版 本 号:  1.0
//  创建时间:  2020年03月02日
//  Compiler:  g++
//  描    述:  
// =====================================================================================


#ifndef _KAFKA_PRODUCER_H_H_H
#define _KAFKA_PRODUCER_H_H_H

#include <ctype.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <syslog.h>
#include <sys/time.h>
#include <errno.h>
 
#include "../common/logger.h"
#include <librdkafka/rdkafka.h>
 
const int PRODUCER_INIT_FAILED = -1;
const int PRODUCER_INIT_SUCCESS = 0;
const int PUSH_DATA_FAILED = -1;
const int PUSH_DATA_SUCCESS = 0;

class ProducerKafka
{
public:
	ProducerKafka(){};
	~ProducerKafka(){};
public:
	int ProducerInit(int partition, const char *brokers, const char *topic);
	int PushDataToKafka(const char *buf, const int len);
	void ProducerDestroy();
private:
	int partition_;
	rd_kafka_t *handler_;
	rd_kafka_conf_t *conf_;
	rd_kafka_topic_t *topic_;
	rd_kafka_topic_conf_t *topic_conf_;
};

#endif

