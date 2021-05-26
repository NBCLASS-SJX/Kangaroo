// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//
//  文 件 名:  singleton.h
//  作    者:  Jiaxing Shao
//  版 本 号:  1.0
//  创建时间:  2020年08月04日
//  Compiler:  g++
//  描    述:  
// =====================================================================================

#ifndef _SINGLETON_H_H_H
#define _SINGLETON_H_H_H

#include <mutex>

template <typename T>
class Singleton
{
public:
	static T *getInstance() {
		if (_instance == nullptr) {
			std::lock_guard<std::mutex> guard(mtxInstance);
			if (_instance == nullptr) {
				_instance = new T();
			}
		}
		return _instance;
	}

	static void destroy() {
		std::lock_guard<std::mutex> guard(mtxInstance);
		if (_instance != nullptr) {
			delete _instance;
		}
		_instance = nullptr;
	}
protected:
	Singleton() noexcept(true) {};
	virtual ~Singleton() noexcept(true) {};
private:
	Singleton(const Singleton &instance) {};
	Singleton& operator = (const Singleton &instance) {};
private:
	static std::mutex mtxInstance;
	static T *_instance;
};

template <typename T> std::mutex Singleton<T>::mtxInstance;
template <typename T> T *Singleton<T>::_instance = nullptr;

#endif

