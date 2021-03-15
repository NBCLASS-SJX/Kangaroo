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
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <iomanip>

#if defined(ROO_PLATFORM_GNUC)
	#include <unistd.h>
	const static char config_filename[] = "./application.conf";
#elif defined(ROO_PLATFORM_WINDOWS)
	#include <windows.h>
	const static char config_filename[] = ".\\application.conf";
#endif

void msleep(uint8_t msec)
{
#if defined(ROO_PLATFORM_GNUC)
	usleep(msec*1000);
#elif defined(ROO_PLATFORM_WINDOWS)
	Sleep(msec);
#endif
}

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
#if defined(ROO_PLATFORM_GNUC)
	localtime_r(&timer, &local);
#elif defined(ROO_PLATFORM_WINDOWS)
	localtime_s($local, &timer);
#endif
	int size = sprintf_safe(format_time, 32, "%04d-%02d-%02d %02d:%02d:%02d", local.tm_year + 1900, local.tm_mon + 1, 
						local.tm_mday, local.tm_hour, local.tm_min, local.tm_sec);
	std::string string_time = format_time;
	return string_time;
}

datetime_t get_current_time()
{
	time_t timer = time(nullptr);
	struct tm local;
#if defined(ROO_PLATFORM_GNUC)
	localtime_r(&timer, &local);
#elif defined(ROO_PLATFORM_WINDOWS)
	localtime_s($local, &timer);
#endif
	datetime_t date;
	date.year = local.tm_year + 1900;
	date.month = local.tm_mon + 1;
	date.mday = local.tm_mday;
	date.hour = local.tm_hour;
	date.minute = local.tm_min;
	date.second = local.second;
	return date;
}

time_t time_to_second(unsigned short year, unsigned char month, unsigned char mday, unsigned char hour, unsigned char minute, unsigned char second)
{
	time_t timer;
	struct tm local;
	return timer;
}

std::string get_config_prop_string(const char *value_name, const char *default_value, const char *filename)
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

std::string get_config_prop_string(const char *value_name, const char *default_value)
{
	return get_config_prop_string(value_name, default_value, config_filename);
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
	return oss.str();
}

std::string *get_split_strings(std::string msg, const char *separator, int &count)
{
	std::list<std::string> string_list;
	int position = 0;
	count = 0;
	while(true) {
		int n = msg.find(separator, position);
		std::string tmp_string = msg.substr(position, n - position);
		if(tmp_string.length() != 0) {
			string_list.push_back(tmp_string); count += 1;
		}
		if(n == -1) break;
		pos = n + separator.length();
	}
	std::string *strings = new std::string[count];
	std::list<std::string>::iterator iter;
	int i = 0;
	for(iter = string_list.begin(); iter != string_list.end(); iter++) {
		strings[i++] = (*iter);
	}
	return strings;
}

void split_strings_free(std::string *&strings)
{
	if(strings == nullptr) {
		delete [] strings; strings = nullptr;
	}
}

void string_trim(std::string &msg, const char ch)
{
	while(msg.length() > 0) {
		if(msg[0] != ch) break;
		msg.erase(0, 1);
	}
	
	while(msg.length() > 0) {
		if(msg[msg.length() - 1] != ch) break;
		msg.erase(msg.length()-1, msg.length());
	}
}

void string_replace(std::string &msg, const char *src, const char *dst)
{
	while(msg.find(src) != -1) {
		msg.replace(msg.find(src), strlen(src), dst);
	}
}

#if defined(ROO_PLATFORM_GNUC)
void run_daemon()
{
	pid_t pid = fork();
	if(pid < 0) {
		exit(1);
	} else if(pid > 0) {
		exit(0);
	}

	setsid();
	sig_process(SIGHUP, SIG_IGN);
	sig_process(SIGTERM, SIG_IGN);
	sig_process(SIGPIPE, sig_handler);
}
#endif
