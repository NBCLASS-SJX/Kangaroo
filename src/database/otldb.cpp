// =====================================================================================
//	Copyright (C) 2019 by Jiaxing Shao.All rights reserved.
//
//	文 件 名:  otldb.cpp
//	作    者:  Jiaxing Shao, 13315567369@163.com
//	版 本 号:  1.0
//	创建时间:  2019年04月10日 20时02分11秒
//	Compiler:  g++
//	描    述:  
// =====================================================================================

#include "otldb.h"
static bool isInitialize = false;

OtlControl::OtlControl()
{
	m_connFlag = false;
	m_stopFlag = false;
}

OtlControl::~OtlControl()
{
}

void OtlControl::database_init(const char *logName, const char *strConn)
{
	char conn_str[OTLCONTROL_CONNSTR_LEN] = { 0 };
	logger = logger_queue_create(logName);
#if defined(__GNUC__)
	sprintf(conn_str, "%s", strConn);
#elif defined(_MSC_VER)
	sprintf_s(conn_str, 256, "%s", strConn);
#endif
	memcpy(m_connStr, conn_str, OTLCONTROL_CONNSTR_LEN);
}

bool OtlControl::database_start()
{
	if(!isInitialize){
		try {
			otl_connect::otl_initialize();
		} catch (otl_exception &p) {
			log_error(logger, "otl initialize error: %s", p.msg);
			return false;
		}
		isInitialize = true;
	}

	std::lock_guard<std::mutex> guard(m_dbLock);
	try {
		m_otlConn.rlogon(m_connStr, 1);
		log_info(logger, "connect database success");
	} catch (otl_exception &p) {
		log_warn(logger, "connect database error:\r\n(msg: %s)\r\n[stm_text]:%s", p.msg, p.stm_text);
		return false;
	}

#if defined(__GNUC__)
	pthread_create(&m_tid, NULL, database_thread_func, this);
#elif defined(_MSC_VER)
	m_tid = (HANDLE)_beginthreadex(NULL, 0, database_thread_func, this, 0, NULL);
#endif
	return true;
}

bool OtlControl::database_stop()
{
	if (!m_connFlag) {
		log_warn(logger, "database worker thread already stoped");
		return false;
	}

	m_stopFlag = true;
	std::unique_lock<std::mutex> guard(m_queueLock);
	m_queueCond.notify_one();
	guard.unlock();
#if defined(__GNUC__)
	pthread_join(m_tid, NULL);
#elif defined(_MSC_VER)
	WaitForSingleObject(m_tid, INFINITE);
#endif

	try {
		m_otlConn.logoff();
	} catch (otl_exception &p) {
		log_warn(logger, "close database error:\r\n(msg: %s)\r\n[stm_text]:%s", p.msg, p.stm_text);
		return false;
	}
	log_info(logger, "database worker thread stoped");
	return true;
}

#if defined(__GNUC__)
void *OtlControl::database_thread_func(void *arg)
#elif defined(_MSC_VER)
unsigned int OtlControl::database_thread_func(void *arg)
#endif
{
	OtlControl *pthis = (OtlControl*)arg;
	while (true) {
		// 线程退出
		if (pthis->m_stopFlag) {
			break;
		}

		// 数据库重连
		if (!pthis->m_connFlag) {
			std::lock_guard<std::mutex> guard(pthis->m_dbLock);
			if (!pthis->m_stopFlag) {
				try {
					pthis->m_otlConn.rlogon(pthis->m_connStr, 1);
				} catch (otl_exception &p) {
					log_warn(pthis->logger, "Connect database error:\r\n(msg: %s)\r\n[stm_text]:%s", p.msg, p.stm_text);
				}
			}
		}

		while (true) {
			std::unique_lock<std::mutex> guard(pthis->m_queueLock);
			while (pthis->m_sqlQueue.size() == 0) {
				pthis->m_queueCond.wait(guard);
				if (pthis->m_stopFlag) {
					return 0;
				}
			}

			std::string sql = pthis->m_sqlQueue.front();
			pthis->m_sqlQueue.pop();
			guard.unlock();

			std::lock_guard<std::mutex> lock(pthis->m_dbLock);
			try {
				otl_cursor::direct_exec(pthis->m_otlConn, sql.c_str());
				pthis->m_otlConn.commit();
			} catch (otl_exception &p) {
				log_warn(pthis->logger, "execute sql error:\r\n(msg: %s)\r\n[stm_text]:%s", p.msg, p.stm_text);
			}
		}
	}
	return 0;
}

bool OtlControl::database_put_sql(char *sql)
{
	std::string sqlStr = sql;
	std::lock_guard<std::mutex> guard(m_queueLock);
	m_sqlQueue.push(sqlStr);
	m_queueCond.notify_one();
	return true;
}

/*
int OtlControl::select_data(char *conn)
{
	try {
		char buffer[128] = { 0 };
		otl_stream select(1, sql, conn->conn);
		while(!select.eof())
		{
			memset(buffer, 0, 128);
			select >> buffer;
			if(select.is_null())
				strcpy(buffer, "null");
			log_debug("%s", buffer);
		}
		select.close();
	} catch(otl_exception &p) {
		return -1;
	}
	return 0;
}
*/

