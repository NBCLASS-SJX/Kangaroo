// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//
//  文 件 名:  kafka_producer.cpp
//  作    者:  Jiaxing Shao, 13315567369@163.com
//  版 本 号:  1.0
//  创建时间:  2020年03月02日
//  Compiler:  g++
//  描    述:  
// =====================================================================================

#include "kafka_producer.h"

int ProducerKafka::producerInit(int partition, const char *brokers, const char *topic)
{
	char tmp[16] = { 0 };
	char errstr[512] = { 0 };

	partition_ = partition;
	conf_ = rd_kafka_conf_new();
	rd_kafka_conf_set_log_cb(conf_, log_callback);

	snprintf(tmp, sizeof(tmp), "%i", SIGIO);
	rd_kafka_conf_set(conf_, "internal.termination.signal", tmp, NULL, 0);

	topic_conf_ = rd_kafka_topic_conf_new();
	if (!(handler_  = rd_kafka_new(RD_KAFKA_PRODUCER, conf_, errstr, sizeof(errstr)))) {
		printf("*****Failed to create new producer: %s*******",errstr);
		return PRODUCER_INIT_FAILED;
	}
	rd_kafka_set_log_level(handler_, LOG_DEBUG);

	if (rd_kafka_brokers_add(handler_, brokers) == 0) {
		printf("****** No valid brokers specified********");
		return PRODUCER_INIT_FAILED;       
	}	

	topic_ = rd_kafka_topic_new(handler_, topic, topic_conf_);
	return PRODUCER_INIT_SUCCESS;
}

int ProducerKafka::pushDataToKafka(const char *buf, const int len)
{
	if(buf == NULL) return PUSH_DATA_FAILED;

	char errstr[512] = { 0 };
	int ret = rd_kafka_produce(topic_, partition_, RD_KAFKA_MSG_F_COPY, (void*)buf, (size_t)len, NULL, 0, NULL);
	if(ret == -1){
		printf("****Failed to produce to topic %s partition %i: %s*****", rd_kafka_topic_name(topic_), partition_, 
				rd_kafka_err2str(rd_kafka_errno2err(errno)));
		rd_kafka_poll(handler_, 0);
		return PUSH_DATA_FAILED;
	}
	printf("***Sent %d bytes to topic:%s partition:%i*****\n", len, rd_kafka_topic_name(topic_), partition_);
	rd_kafka_poll(handler_, 0);
	return PUSH_DATA_SUCCESS;
}

void ProducerKafka::producerDestroy()
{
	rd_kafka_topic_destroy(topic_);
	rd_kafka_destroy(handler_);
}

void ProducerKafka::log_callback(const rd_kafka_t *rk, int level,const char *fac, const char *buf) 
{
	printf("RDKAFKA-%i-%s: %s: %s", level, fac, rk ? rd_kafka_name(rk) : NULL, buf);
}

