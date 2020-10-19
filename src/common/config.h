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

#ifndef _CONFIG_H_H_H
#define _CONFIG_H_H_H

#if defined(KGR_CPP_14_ON)
	#define KGR_CPP_11_ON
#endif

#if defined(KGR_CPP_17_ON)
	#define KGR_CPP_11_ON
	#define KGR_CPP_14_ON
#endif



#if defined(__clang__) || defined(__GNUC__)
	/** clang or g++, -std=
	 * c++98: #define __cplusplus  199711L
	 * c++03: #define __cplusplus  199711L
	 * c++11: #define __cplusplus  201103L
	 * c++14: #define __cplusplus  201402L
	 * c++17: #define __cplusplus  201500L
	 * */
	#if defined(__cplusplus) && (__cplusplus==201103L)
		#if !defined(OTL_CPP_11_ON)
			#define OTL_CPP_11_ON
		#endif
	#elif defined(__cplusplus) && (__cplusplus==201402L)
		#if !defined(OTL_CPP_11_ON)
			#define OTL_CPP_11_ON
		#endif
		#if !defined(OTL_CPP_14_ON)
			#define OTL_CPP_14_ON
		#endif
	#elif defined(__cplusplus) && (__cplusplus>201402L)
		#if !defined(OTL_CPP_11_ON)
			#define OTL_CPP_11_ON
		#endif
		#if !defined(OTL_CPP_14_ON)
			#define OTL_CPP_14_ON
		#endif
		#if !defined(OTL_CPP_17_ON)
			#define OTL_CPP_17_ON
		#endif
	#endif

	#define __GNUC_VERSION__ (__GNUC__*100+__GNUC_MINOR__)
	#define HAVE_UNISTD_H
	#define CRLF \n
#elif defined(_MSC_VER)
	/** micorsoft
	 * MS VC++ 15.0 _MSC_VER = 1910 (Visual Studio 2017)
	 * MS VC++ 14.0 _MSC_VER = 1900 (Visual Studio 2015)
	 * MS VC++ 12.0 _MSC_VER = 1800 (VisualStudio 2013)
	 * MS VC++ 11.0 _MSC_VER = 1700 (VisualStudio 2012)
	 * MS VC++ 10.0 _MSC_VER = 1600(VisualStudio 2010)
	 * MS VC++ 9.0 _MSC_VER = 1500(VisualStudio 2008)
	 * MS VC++ 8.0 _MSC_VER = 1400(VisualStudio 2005)
	 * MS VC++ 7.1 _MSC_VER = 1310(VisualStudio 2003)
	 * MS VC++ 7.0 _MSC_VER = 1300(VisualStudio .NET)
	 * MS VC++ 6.0 _MSC_VER = 1200(VisualStudio 98)
	 * MS VC++ 5.0 _MSC_VER = 1100(VisualStudio 97)
	 * */
	#if (_MSC_VER==1600)
		#define KGR_CPP_11_ON
	#elif (_MSC_VER >= 1700)
		#define KGR_CPP_11_ON
	#elif (_MSC_VER >= 1800)
		#define KGR_CPP_11_ON
	#elif (_MSC_VER >= 1900 && !defined(_MSVC_LANG))
		#if !defined(KGR_CPP_11_ON)
			#define KGR_CPP_11_ON
		#endif
	#endif

	#define HAVE_WINDOWS_H
	#define CRLF \r\n

	#include <stdint.h>
#endif

