// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//
//  文 件 名:  time.cpp
//  作    者:  Jiaxing Shao
//  版 本 号:  1.0
//  创建时间:  2020年08月03日
//  Compiler:  g++
//  描    述:  
// =====================================================================================

#include "time.h"

#if defined(HAVE_UNISTD_H)
	#include <unistd.h>
	#include <time.h>
#elif defined(HAVE_WINDOWS_H)
	#include <windows.h>
	#include <time.h>
#endif

void kg_usleep(unsigned usec)
{
#if defined(HAVE_UNISTD_H)
	usleep(usec);
#elif defined(HAVE_WINDOWS_H)
	Sleep(usec * 1000)
#endif
}

