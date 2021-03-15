// =====================================================================================
//  Copyright (C) 2021 by Jiaxing Shao. All rights reserved
//  文 件 名:  condition_variable.h
//  作    者:  Jiaxing Shao
//  版 本 号:  1.0
//  创建时间:  2021年03月15日
//  Compiler:  g++
//  描    述:  
// =====================================================================================

#ifndef _ROO_CONDITION_VARIABLE_H_H_H
#define _ROO_CONDITION_VARIABLE_H_H_H

namespace roo {
    class condition_variable
    {
	public:
		condition_variable() {
#if defined(_MSC_VER)
			InitializeCriticalSection(&m_cs);
#endif
		}
	        
		~condition_variable() {
#if defined(_MSC_VER)
			DeleteCriticalSection(&m_cs);
#endif
		}

		void wait(roo::unique_lock &guard) {
#if defined(_MSC_VER)
			HANDLE handle = CreateEvent(nullptr, false, false, nullptr);

			EnterCriticalSection(&m_cs);
			m_eventList.push_back(handle);
			LeaveCriticalSection(&m_cs);
			guard.unlock();
			WaitForSingleObject(handle, INFINITE);
			guard.lock();
#endif
		}
	
		void notify_one() {
#if defined(_MSC_VER)
			EnterCriticalSection(&m_cs);
			HANDLE handle = m_eventList.front();
			m_eventList.pop_front();
			LeaveCriticalSection(&m_cs);
			SetEvent(handle);
			CloseHandle(handle);
#endif
		}

		void notify_all() {
#if defined(_MSC_VER)
			EnterCriticalSection(&m_cs);
			std::list<HANDLE>::iterator iter = m_eventList.begin();
			for (iter = m_eventList.begin(); iter != m_eventList.end(); ) {
				HANDLE handle = (*iter);
				SetEvent(handle);
				CloseHandle(handle);
				m_eventList.erase(iter++);
			}
			LeaveCriticalSection(&m_cs);
#endif
		}
	private:
#if defined(_MSC_VER)
		std::list<HANDLE> m_eventList;
		CRITICAL_SECTION  m_cs;
#endif
	};
}

#endif

