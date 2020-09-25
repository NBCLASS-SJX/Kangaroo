// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//
//  文 件 名:  common.h
//  作    者:  Jiaxing Shao
//  版 本 号:  1.0
//  创建时间:  2020年08月03日
//  Compiler:  g++
//  描    述:  
// =====================================================================================

#ifndef KG_COMMON_H
#define KG_COMMON_H

#include "config.h"
#include <string>

int sprintf_safe(char *dest, int size, const char *fmt, ...);
std::string get_current_time();
std::string get_config_string(const char *value_name, const char *default_valuem);
std::string get_config_string(const char *value_name, const char *default_valuem, const char *filename);
void string_trim(std::string &str, const char ch);
std::string get_hex_string(const char *buf, unsigned int len);

#endif

