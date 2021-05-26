// =====================================================================================
//	Copyright (C) 2018 by Jiaxing Shao.All rights reserved.
//
//	文 件 名:  bytearray.cpp
//	作    者:  Jiaxing Shao, 13315567369@163.com
//	版 本 号:  1.0
//	创建时间:  2018年06月08日 14时24分25秒
//	Compiler:  g++
//	描    述:  
// =====================================================================================

#include "bytearray.h"

byte_array_t *byte_array_create(int size)
{
    byte_array_t *byte_array = new byte_array_t;
	byte_array->buffer = new unsigned char[size];
	memset(byte_array->buffer, 0, size);
	byte_array->total = size;
	byte_array->size = 0;
	byte_array->head = 0;
	byte_array->tail = 0;
	return byte_array;
}

void byte_array_free(byte_array_t *byte_array)
{
	if(byte_array == nullptr)
		return;

    byte_array_t *tmp_byte_array = byte_array;
	byte_array = nullptr;

	if(tmp_byte_array != nullptr) {
		if(tmp_byte_array->buffer != nullptr) {
			delete [] tmp_byte_array->buffer;
			tmp_byte_array->buffer = nullptr;
		}
		delete tmp_byte_array;
		tmp_byte_array = nullptr;
	}
}

bool byte_array_push(byte_array_t *byte_array, const char *buf, int len)
{
	if(byte_array == nullptr)
		return false;

	if(byte_array->total - byte_array->size < len) {
		return false;
	}
	
	if(byte_array->total - byte_array->tail >= len) {
		memcpy(byte_array->buffer + byte_array->tail, buf, len);
		byte_array->tail = (byte_array->tail + len) % byte_array->total;
	} else {
		int llen = byte_array->total - byte_array->tail;
		int rlen = len - llen;
		memcpy(byte_array->buffer + byte_array->tail, buf, llen);
		memcpy(byte_array->buffer, buf + llen, rlen);
		byte_array->tail = rlen;
	}
	byte_array->size += len;
	return true;
}

bool byte_array_pop(byte_array_t *byte_array, char *buf, int len, bool keeped)
{
	if(byte_array == nullptr)
		return false;

	if(byte_array->size < len) {
		return false;
	}
		
	if(byte_array->total - byte_array->head >= len) {
		memcpy(buf, byte_array->buffer + byte_array->head, len);
		if(!keeped){
			byte_array->head = (byte_array->head + len) % byte_array->total;
		}
	} else {
		int llen = byte_array->total - byte_array->head;
		int rlen = len - llen;
		memcpy(buf, byte_array->buffer + byte_array->head, llen);
		memcpy(buf + llen, byte_array->buffer, rlen);
		if(!keeped){
			byte_array->head = rlen;
		}
	}

	if(!keeped){
		byte_array->size -= len;
	}
	return true;
}

void byte_array_clean(byte_array_t *byte_array)
{
	byte_array->size = 0;
	byte_array->head = 0;
	byte_array->tail = 0;
}

int byte_array_len(byte_array_t *byte_array)
{
    if (byte_array == nullptr)
        return -1;
    int length = byte_array->size;
    return length;
}
