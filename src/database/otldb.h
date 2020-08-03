// =====================================================================================
//	Copyright (C) 2019 by Jiaxing Shao.All rights reserved.
//
//	文 件 名:  otldb.h
//	作    者:  Jiaxing Shao, 13315567369@163.com
//	版 本 号:  1.0
//	创建时间:  2019年04月10日 20时02分03秒
//	Compiler:  g++
//	描    述:  需在引入otlv4.h头文件前声明宏定义
//             #define OTL_ORA11G_R2
//             #define OTL_ODBC_MYSQL
//             有引入 Windows.h 头文件时,在otlv4之前引入
// =====================================================================================

#ifndef _OTL_CONTROL_H_H_H
#define _OTL_CONTROL_H_H_H


#define OTL_ORA11G_R2
#define OTLCONTROL_CONNSTR_LEN (256)

#include "../common/kgr_utils.h"
#include "../common/kgr_logger.h"
#include "otlv4.h"

class OtlControl
{
public:
	OtlControl();
	virtual ~OtlControl();
public:
	void database_init(const char *logName, const char *strConn);
	bool database_start();
	bool database_stop();
	bool database_put_sql(char *sql);

protected:
#if defined(__GNUC__)
	static void *database_thread_func(void *arg);
#elif defined(_MSC_VER)
	static unsigned int __stdcall database_thread_func(void *arg);
#endif

private:
#if defined(__GNUC__)
	pthread_t m_tid;
#elif defined(_MSC_VER)
	HANDLE m_tid;
#endif
	char m_connStr[OTLCONTROL_CONNSTR_LEN];
	bool m_connFlag;
	bool m_stopFlag;
	otl_connect m_otlConn;
	std::mutex m_dbLock;

	std::mutex m_queueLock;
	std::condition_variable m_queueCond;
	std::queue<std::string> m_sqlQueue;
	log_queue_t *logger;
};

#endif

