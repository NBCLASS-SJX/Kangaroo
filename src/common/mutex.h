// =====================================================================================
//  Copyright (C) 2021 by Jiaxing Shao. All rights reserved
//  文 件 名:  mutex.h
//  作    者:  Jiaxing Shao
//  版 本 号:  1.0
//  创建时间:  2021年03月15日
//  Compiler:  g++
//  描    述:  
// =====================================================================================

#ifndef _ROO_MUTEX_H_H_H
#define _ROO_MUTEX_H_H_H

#if defined(_MSC_VER)
	#ifdef _WIN32
    	#ifndef WIN32_LEAN_AND_MEAN
        	#define WIN32_LEAN_AND_MEAN
	    #endif
    	#include <winsock2.h>
	    #include <windows.h>
    	#pragma comment(lib, "Ws2_32.lib")
	    #undef WIN32_LEAN_AND_MEAN
	#endif
#endif

namespace roo {
	class mutex
	{
	public:
		mutex(){
#if defined(_MSC_VER)
			InitializeCriticalSection(&m_cs);
#endif
		}
	    
		~mutex(){
#if defined(_MSC_VER)
			DeleteCriticalSection(&m_cs);
#endif
		}
	        
		void lock(){
#if defined(_MSC_VER)
			EnterCriticalSection(&m_cs);
#endif
		}
	        
		void unlock(){
#if defined(_MSC_VER)
			LeaveCriticalSection(&m_cs);
#endif
		}
	private:
#if defined(_MSC_VER)
		CRITICAL_SECTION  m_cs;
#endif
	};

	template<typename _Mutex>
    class lock_guard
    {    
	public:
		lock_guard(_Mutex *section) :m_section(section){
			m_section->lock();
		}
		virtual ~lock_guard(){
			m_section->unlock();
		}
	private:
		_Mutex *m_section;
	};

	template<typename _Mutex>
    class unique_lock
    {
	public:
		unique_lock(_Mutex *section) :m_section(section) {
			m_section->lock();
		}
		
		virtual ~unique_lock() {
			m_section->unlock();
		}
	        
		void lock() {
			m_section->lock();
		}
	        
		void unlock(){
			m_section->unlock();
		}
	private:
		_Mutex *m_section;
	};
}
#endif

