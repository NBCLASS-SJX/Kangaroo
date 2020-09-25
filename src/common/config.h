// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//
//  文 件 名:  config.h
//  作    者:  Jiaxing Shao
//  版 本 号:  1.0
//  创建时间:  2020年08月03日
//  Compiler:  g++
//  描    述:  
// =====================================================================================

#ifndef KG_CONFIG_H
#define KG_CONFIG_H

	#if defined(__GNUC__)
		#define HAVE_UNISTD_H
	#elif defined(_MSC_VER)
		#define HAVE_WINDOWS_H
	#endif

#endif
