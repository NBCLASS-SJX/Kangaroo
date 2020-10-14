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

log_queue_t *ProducerKafka::logger = NULL;

ProducerKafka::ProducerKafka()
{
}

ProducerKafka::~ProducerKafka()
{
}


int ProducerKafka::init_kafka(int partition, const char *brokers, const char *topic)
{
	logger = logger_queue_create("kafka");
	char tmp[16] = { 0 };
	char errstr[512] = { 0 };
	partition_ = partition;
	conf_ = rd_kafka_conf_new();
	rd_kafka_conf_set_log_cb(conf_, log_callback);

	snprintf(tmp, sizeof(tmp), "%i", SIGIO);
	rd_kafka_conf_set(conf_, "internal.termination.signal", tmp, NULL, 0);

	topic_conf_ = rd_kafka_topic_conf_new();
	if (!(handler_  = rd_kafka_new(RD_KAFKA_PRODUCER, conf_, errstr, sizeof(errstr)))) {
		log_error(logger, "*****Failed to create new producer: %s*******",errstr);
		return PRODUCER_INIT_FAILED;
	}
	rd_kafka_set_log_level(handler_, LOG_DEBUG);

	if (rd_kafka_brokers_add(handler_, brokers) == 0) {
		log_error(logger, "****** No valid brokers specified********");
		return PRODUCER_INIT_FAILED;       
	}	

	topic_ = rd_kafka_topic_new(handler_, topic, topic_conf_);
	return PRODUCER_INIT_SUCCESS;
}

int ProducerKafka::push_data_to_kafka(const char *pBuf, const int nLen)
{
	if(pBuf == NULL){
		return -1;
	}
	char errstr[512] = { 0 };
	int ret = rd_kafka_produce(topic_, partition_, RD_KAFKA_MSG_F_COPY, (void*)pBuf, (size_t)nLen, NULL, 0, NULL);
	if(ret == -1){
		log_error(logger, "****Failed to produce to topic %s partition %i: %s*****", 
				rd_kafka_topic_name(topic_), partition_, 
				rd_kafka_err2str(rd_kafka_errno2err(errno)));
		rd_kafka_poll(handler_, 0);
		return PUSH_DATA_FAILED;
	}
	log_info(logger, "***Sent %d bytes to topic:%s partition:%i*****\n", nLen, rd_kafka_topic_name(topic_), partition_);
	rd_kafka_poll(handler_, 0);
	return PUSH_DATA_SUCCESS;
}

void ProducerKafka::destroy()
{
	rd_kafka_topic_destroy(topic_);
	rd_kafka_destroy(handler_);
}

void ProducerKafka::log_callback(const rd_kafka_t *rk, int level,const char *fac, const char *buf) 
{
	log_error(logger, "RDKAFKA-%i-%s: %s: %s", level, fac, rk ? rd_kafka_name(rk) : NULL, buf);
}

