// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//
//  文 件 名:  main.cpp
//  作    者:  Jiaxing Shao
//  版 本 号:  1.0
//  创建时间:  2020年08月03日
//  Compiler:  g++
//  描    述:  
// =====================================================================================

#include <iostream>
#include <string>
using namespace std;

#include "time.h"
#include "common.h"

int main()
{
	std::string str1 = get_current_time();
	printf("%s\n", str1.c_str());
	char buf[8] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
	std::string str2 = get_hex_string(buf, 8);
	printf("%s\n", str2.c_str());
	std::string str3 = get_config_string("test", "value");
	printf("%s\n", str3.c_str());
	return 0;
}

