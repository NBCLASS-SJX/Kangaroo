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

#ifndef _COMMON_H_H_H
#define _COMMON_H_H_H

#include "config.h"
#include <string>

/** sleep millisecond */
void msleep(uint8_t msec);

/** safe sprintf */
int sprintf_safe(char *dest, int size, const char *fmt, ...);

/** get current format date */
std::string get_current_time();

/** get config properties to string */
std::string get_config_prop_string(const char *value_name, const char *default_value, const char *filename);

/** get config properties to string */
std::string get_config_prop_string(const char *value_name, const char *default_value);

/** get config properties to int */
int get_config_prop_int(const char *value_name, const char *default_value, const char *filename);

/** get config proprties to int */
int get_config_prop_int(const char *value_name, const char *default_value);

/** buffer to hex string */
std::string get_hex_string(const char *buf, unsigned int len);

/** string split, return string pointer */
std::string *get_split_strings(std::string msg, const char *separator, int &count);

/** string pointer free */
void split_strings_free(std::string *&strings);

/** returns a string whose value is this string, with any leading and trailing whitespace removed */
void string_trim(std::string &msg, const char ch);

/* string replace */
void string_replace(std::string &msg, const char *src, const char *dst);

#endif

