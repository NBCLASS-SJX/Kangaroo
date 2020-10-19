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


#if defined(HAVE_UNISTD_H)
	const static char config_filename[] = "./application.conf";
#elif defined(HAVE_WINDOWS_H)
	const static char config_filename[] = ".\\application.conf";
#endif

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
	char format_time[32] = { 0 };
	time_t timer = time(nullptr);
	struct tm local;
#if defined(HAVE_UNISTD_H)
	localtime_r(&timer, &local);
#elif defined(HAVE_WINDOWS_H)
	localtime_s($local, &timer);
#endif
	int size = sprintf_safe(format_time, 32, "%04d-%02d-%02d %02d:%02d:%02d", local.tm_year + 1900, local.tm_mon + 1, 
						local.tm_mday, local.tm_hour, local.tm_min, local.tm_sec);
	std::string string_time = format_time;
	return string_time;
}

std::string get_config_string(const char *value_name, const char *default_value, const char *filename)
{
	int position = 0;
	std::string value;
	std::fstream file;
	file.open(filename, std::ios::app);
	while(file.eof()){
		std::string s;
		getline(file, s);

		if(s.length() <= 0 || s[0] == '#') continue;
		position = s.find("=");
		if(position == s,npos) continue;
		std::string key = s.substr(0, position);
		value = s.substr(position + 1, s.npos);
		if(strncmp(value_name, key.c_str(), strlen(value_name)) != 0) continue;
		file.close();
		string_trim(value, ' ');
		return value;
	}

	printf("%s [warn] config properties not found %s, using default value %s" CRLF, cur_time.c_str(), value_name, default_value);
	sprintf_safe(property, 512, "%s = %s" CRLF, value_name, default_value);
	file.write(property, strlen(property));
	file.close();
	value = default_value;
	return value;
}

std::string get_config_string(const char *value_name, const char *default_value)
{
	return get_config_string(value_name, default_value, config_filename);
}

int get_config_prop_int(const char *value_name, const char *default_value, const char *filename)
{
	std::string config_value = get_config_prop_string(value_name, default_value, filename);
	return atoi(config_value.c_str());
}

int get_config_prop_int(const char *value_name, const char *default_value)
{
	return get_config_prop_int(value_name, default_value, config_filename);
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

std::string *get_split_strings(std::string msg, const char *separator, int &count)
{
}

bool split_strings_free(std::string *&parts)
{
}
bool string_replace(std::string &str, const char *src, const char *dst);
void string_trim(std::string &str, const char ch);
std::string string_find(const char *src, const char *head, const char *tail);



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

std::string *GetSplitStrings(std::string msg, std::string separator, int &count)
{
	std::list<std::string> list_str;
	int pos = 0;
	count = 0;
	while(true)
	{
		int n = msg.find(separator, pos);
		std::string str = msg.substr(pos, n - pos);
		if(str != "") {
			list_str.push_back(str);
			count += 1;
		}
		if(n == -1)
			break;
		pos = n + separator.length();
	}
	std::string *strs = new std::string[count];
	std::list<std::string>::iterator iter;
	int i = 0;
	for(iter = list_str.begin(); iter != list_str.end(); iter++) {
		strs[i++] = (*iter);
	}
	return strs;
}

void FreeSplitStrings(std::string *&parts)
{
	if(parts == NULL) {
		delete [] parts;
		parts = NULL;
	}
}

void StringReplace(std::string &str, const char *src, const char *dst)
{
	while(str.find(src) != -1) {
		str.replace(str.find(src), strlen(src), dst);
	}
}

