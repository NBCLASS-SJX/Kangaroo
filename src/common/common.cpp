// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//
//  文 件 名:  common.cpp
//  作    者:  Jiaxing Shao
//  版 本 号:  1.0
//  创建时间:  2020年08月03日
//  Compiler:  g++
//  描    述:  
// =====================================================================================

#include "common.h"
#include "time.h"
#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <iomanip>

int sprintf_safe(char *dest, int size, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int n = vsprintf(dest, fmt, args);
	va_end(args);
	return n;
}

std::string get_current_time()
{
	time_t timer = time(nullptr);
	char curTime[32] = { 0 };
	struct tm local;
#if defined(HAVE_UNISTD_H)
	localtime_r(&timer, &local);
#elif defined(HAVE_WINDOWS_H)
	localtime_r($timer, &local);
#endif
	int size = sprintf_safe(curTime, 32, "%04d-%02d-%02d %02d:%02d:%02d", local.tm_year + 1900, local.tm_mon + 1, 
						local.tm_mday, local.tm_hour, local.tm_min, local.tm_sec);
	std::string strTime = curTime;
	return strTime;
}


std::string get_config_string(const char *value_name, const char *default_value)
{
#if defined(HAVE_UNISTD_H)
	const char filename[] = "./application.conf";
#elif defined(HAVE_WINDOWS_H)
	const char filename[] = ".\\application.conf";
#endif
	get_config_string(value_name, default_value, filename);
}

std::string get_config_string(const char *value_name, const char *default_value, const char *filename)
{
	int position = 0;
	std::string value;
	std::ifstream iFile(filename, std::ios::in);

	if(!iFile){
		char property[512] = { 0 };
		std::string cur_time = get_current_time();
		std::ofstream oFile(filename, std::ios::out | std::ios::app);
#if defined(HAVE_UNISTD_H)
		printf("%s [error] configure file not found %s\n", cur_time.c_str(), filename);
		sprintf_safe(property, 512, "%s = %s\n", value_name, default_value);
#elif defined(HAVE_WINDOWS_H)
		printf("%s [error] configure file not found %s\r\n", cur_time.c_str(), filename);
		sprintf_safe(property, 512, "%s = %s\r\n", value_name, default_value);
#endif
		oFile.write(property, strlen(property));
		oFile.close();
	}else{
		while(!iFile.eof()){
			std::string s;
			getline(iFile, s);

			if(s.length() <= 0 || s[0] == '#'){
				continue;
			}
							
			position = s.find("=");						
			if (position == s.npos) {
				continue;
			}
			std::string key = s.substr(0, position);
			value = s.substr(position + 1, s.npos);
			if(strncmp(value_name, key.c_str(), strlen(value_name)) != 0) {
				continue;
			}
			iFile.close();
			string_trim(value, ' ');
			return value;
		}
	}
	value = default_value;
	return value;
}

void string_trim(std::string &str, const char ch)
{
	while(str.length() > 0) {
		if(str[0] == ch) {
			str.erase(0, 1);
		} else {
			break;
		}
	}
	
	while(str.length() > 0) {
		if(str[str.length() - 1] == ch) {
			str.erase(str.length()-1,str.length());
		} else {
			break;
		}
	}
}

std::string get_hex_string(const char *buf, unsigned int len)
{
	if(buf == nullptr || len < 0){
		return "";
	}

	std::ostringstream oss;
	oss << std::setfill('0') << std::uppercase;
	for(int i = 0; i < len; i++) {
		oss << std::setw(2) << std::hex << static_cast<int> (buf[i] & 0xFF) << std::dec  << ' ';
	}
	std::string str = oss.str().c_str();
	return str;
}

