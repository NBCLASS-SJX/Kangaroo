// =====================================================================================
//	Copyright (C) 2018 by Jiaxing Shao.All rights reserved.
//
//	文 件 名:  bytearray.h
//	作    者:  Jiaxing Shao, 13315567369@163.com
//	版 本 号:  1.0
//	创建时间:  2018年06月08日 14时10分10秒
//	Compiler:  g++
//	描    述:  
// =====================================================================================

#ifndef _BYTEARRAY_H_H_H
#define _BYTEARRAY_H_H_H

#define KGR_BYTES_ARRAY_SIZE (1024*1024)

#include <iostream>
#include <cstring>
#include <string>

typedef struct {
	unsigned char *buffer;
	int size;
	int total;
	int head;
	int tail;
} byte_array_t;

/* create byte_array */
byte_array_t *byte_array_create(int size = KGR_BYTES_ARRAY_SIZE);

/* free a byte_array */
void byte_array_free(byte_array_t *byte_array);

/* push size len bytes data to byte_array tail */
bool byte_array_push(byte_array_t *byte_array, const char *buf, int len);

/* pop size len bytes data from byte_array head */
bool byte_array_pop(byte_array_t *byte_array, char *buf, int len, bool keeped = false);

/* clean data of byte_array */
void byte_array_clean(byte_array_t *byte_array);

/* get byte_array size */
int byte_array_len(byte_array_t *byte_array);

#endif

